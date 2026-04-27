#include "slashBurn.hpp"

#include <algorithm>
#include <limits>
#include <numeric>
#include <queue>
#include <vector>

#include "../formats/csr.hpp"

using namespace mtx;

namespace mtx::reorderings {

// SlashBurn reordering: reorder nodes by burn time to group communities
template <typename IndexType, typename ValueType>
std::vector<IndexType> slashBurn(const mtx::CSR<IndexType, ValueType>& csr) {
  // first step: find strongest connected component and its size
  // second step: in that component, find the node with highest degree and
  // remove it third step: repeat until all nodes are removed, keeping track of
  // the order fourth step: every iteration put the removed node at the start of
  // the permutation vector, repeat until all nodes are removed. Nodes outside
  // of the largest component are added at the end of the permutation vector in
  // any order every iteration.

  std::vector<IndexType> perm;
  std::vector<char> visited_nodes(csr.nrows(), 0);
  std::vector<IndexType> node_degrees(csr.nrows(), 0);
  std::iota(node_degrees.begin(), node_degrees.end(), 0);
  for (IndexType i = 0; i < csr.nrows(); i++) {
    node_degrees[i] = getNodeDegree(csr, i);
  }

  // continue with an implementation of slash burn until all nodes are visited
  while (perm.size() < csr.nrows()) {
    // find the largest connected component
    std::vector<IndexType> component;
    std::queue<IndexType> visit_queue;
    IndexType max_component_size = 0;
    IndexType max_component_start = 0;
    for (IndexType i = 0; i < csr.nrows(); i++) {
      if (visited_nodes[i] == 0) {
        IndexType component_size = 0;
        visit_queue.push(i);
        visited_nodes[i] = 1;

        while (!visit_queue.empty()) {
          IndexType current_node = visit_queue.front();
          component_size++;

          for (IndexType j = csr.rowPtr()[current_node];
               j < csr.rowPtr()[current_node + 1]; j++) {
            IndexType neighborId = csr.colIdx()[j];

            if (visited_nodes[neighborId] == 0) {
              visit_queue.push(neighborId);
              visited_nodes[neighborId] = 1;
            }
          }

          visit_queue.pop();
        }

        if (component_size > max_component_size) {
          max_component_size = component_size;
          max_component_start = i;
        }
      }
    }

    // find the node with the highest degree in the largest component
    IndexType max_degree_node = max_component_start;
    for (IndexType i = 0; i < csr.nrows(); i++) {
      if (visited_nodes[i] == 1 &&
          node_degrees[i] > node_degrees[max_degree_node]) {
        max_degree_node = i;
      }
    }

    // remove the node with the highest degree from the component
    visited_nodes[max_degree_node] = 2;
    perm.push_back(max_degree_node);
  }

  return perm;
}  // namespace mtx::reorderings