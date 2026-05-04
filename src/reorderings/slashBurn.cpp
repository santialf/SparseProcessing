#include "slashBurn.hpp"

#include <algorithm>
#include <numeric>
#include <queue>
#include <vector>

// TODO: update degree of nodes after burning, currently we only compute degree
// once at the start
//  implement the two optimizations mentioned in the paper:
//  1) greedy burn: update degree of nodes as you pick the top k nodes and pick
//  the next node with the highest degree 2) hub ordering: order spokes by node
//  ID k to which is connected, so that nodes connected to the same hub are
//  placed close together in the ordering, which can improve locality and reduce
//  bandwidth

namespace mtx::reorderings {

// Function to find connected components in the subgraph induced by the given
// nodes
template <typename IndexType, typename ValueType>
std::vector<std::vector<IndexType>> findComponents(
    const mtx::CSR<IndexType, ValueType>& csr,
    const std::vector<IndexType>& nodes) {
  std::vector<std::vector<IndexType>> components;
  std::vector<char> visited(csr.nrows(), 0);
  std::vector<char> in_nodes(csr.nrows(), 0);

  for (IndexType node : nodes) {
    in_nodes[node] = 1;
  }

  for (IndexType node : nodes) {
    if (!visited[node]) {
      std::vector<IndexType> component;
      std::queue<IndexType> q;
      q.push(node);
      visited[node] = 1;
      component.push_back(node);

      while (!q.empty()) {
        IndexType current = q.front();
        q.pop();
        for (IndexType j = csr.rowPtr()[current]; j < csr.rowPtr()[current + 1];
             ++j) {
          IndexType neighbor = csr.colIdx()[j];
          if (in_nodes[neighbor] && !visited[neighbor]) {
            visited[neighbor] = 1;
            q.push(neighbor);
            component.push_back(neighbor);
          }
        }
      }
      components.push_back(std::move(component));
    }
  }

  return components;
}

// Recursive function to burn a component and place nodes within the current
// window
template <typename IndexType, typename ValueType>
void burnComponent(const mtx::CSR<IndexType, ValueType>& csr,
                   std::vector<IndexType> component,
                   std::vector<IndexType>& perm, IndexType left,
                   IndexType right, const IndexType k) {
  if (component.empty() || left >= right) {
    return;
  }

  // sort component nodes by degree
  std::sort(component.begin(), component.end(), [&](IndexType a, IndexType b) {
    return getNodeDegree(csr, a) > getNodeDegree(csr, b);
  });

  // burn top k nodes
  std::vector<IndexType> burned;
  for (size_t i = 0; i < k; ++i) {
    burned.push_back(component[i]);
    perm[left++] = component[i];
  }

  std::vector<IndexType> remaining;
  std::set_difference(component.begin(), component.end(), burned.begin(),
                      burned.end(), std::back_inserter(remaining));

  if (remaining.empty()) {
    return;
  }

  auto sub_components = findComponents(csr, remaining);
  std::sort(
      sub_components.begin(), sub_components.end(),
      [](const std::vector<IndexType>& a, const std::vector<IndexType>& b) {
        return a.size() > b.size();
      });

  for (size_t idx = 1; idx < sub_components.size(); ++idx) {
    for (IndexType node : sub_components[idx]) {
      perm[--right] = node;
    }
  }

  if (!sub_components.empty()) {
    burnComponent(csr, std::move(sub_components[0]), perm, left, right, k);
  }
}

// SlashBurn reordering
template <typename IndexType, typename ValueType>
std::vector<IndexType> slashBurn(const mtx::CSR<IndexType, ValueType>& csr,
                                 const IndexType k) {
  std::vector<IndexType> perm(csr.nrows());
  std::vector<IndexType> all_nodes(csr.nrows());
  std::iota(all_nodes.begin(), all_nodes.end(), 0);

  auto components = findComponents(csr, all_nodes);
  std::sort(
      components.begin(), components.end(),
      [](const std::vector<IndexType>& a, const std::vector<IndexType>& b) {
        return a.size() > b.size();
      });

  IndexType right = csr.nrows();
  for (size_t idx = 1; idx < components.size(); ++idx) {
    for (IndexType node : components[idx]) {
      perm[--right] = node;
    }
  }

  if (!components.empty()) {
    burnComponent(csr, std::move(components[0]), perm, IndexType{0}, right, k);
  }

  return perm;
}

}  // namespace mtx::reorderings