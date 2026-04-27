#include "rcm.hpp"

#include <algorithm>
#include <queue>
#include <utility>
#include <vector>

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
IndexType getNodeDegree(const CSR<IndexType, ValueType>& csr, IndexType node) {
  return csr.rowPtr()[node + 1] - csr.rowPtr()[node];
}

// Function to find the farthest node with smallest degree at max level using
// BFS
template <typename IndexType, typename ValueType>
IndexType findFarthestNode(const CSR<IndexType, ValueType>& csr,
                           IndexType root) {
  std::vector<char> visited(csr.nrows(), 0);
  std::queue<std::pair<IndexType, IndexType>> bfs_queue;
  bfs_queue.push({root, 0});
  visited[root] = 1;
  IndexType farthest = root;
  IndexType max_level = 0;
  IndexType min_degree = getNodeDegree(csr, root);

  while (!bfs_queue.empty()) {
    auto [node, level] = bfs_queue.front();
    bfs_queue.pop();

    if (level > max_level) {
      max_level = level;
      farthest = node;
      min_degree = getNodeDegree(csr, node);
    } else if (level == max_level) {
      IndexType degree = getNodeDegree(csr, node);
      if (degree < min_degree) {
        min_degree = degree;
        farthest = node;
      }
    }

    for (IndexType i = csr.rowPtr()[node]; i < csr.rowPtr()[node + 1]; ++i) {
      IndexType neighbor = csr.colIdx()[i];
      if (!visited[neighbor]) {
        visited[neighbor] = 1;
        bfs_queue.push({neighbor, level + 1});
      }
    }
  }
  return farthest;
}

// Function to find a peripheral node by finding the farthest node from the
// farthest node
template <typename IndexType, typename ValueType>
IndexType findPeripheralNode(const CSR<IndexType, ValueType>& csr,
                             IndexType start) {
  IndexType farthest = findFarthestNode(csr, start);
  return findFarthestNode(csr, farthest);
}

// RCM reordering: reorder nodes by BFS from a peripheral node, sorting
// neighbors by degree
template <typename IndexType, typename ValueType>
std::vector<IndexType> rcm(const CSR<IndexType, ValueType>& csr) {
  std::vector<IndexType> perm;
  std::vector<char> visited_nodes(csr.nrows(), 0);
  std::queue<IndexType> visit_queue;
  IndexType node_counter = 0;

  for (IndexType i = 0; i < csr.nrows(); i++) {
    if (visited_nodes[i] == 0) {
      IndexType peripheral_node_id = findPeripheralNode(csr, i);
      visit_queue.push(peripheral_node_id);
      visited_nodes[peripheral_node_id] = 1;

      // go through all of the nodes in a connected component
      while (!visit_queue.empty()) {
        IndexType current_node = visit_queue.front();
        std::vector<IndexType> neighbors;

        // visit neighbors of the current node
        for (IndexType j = csr.rowPtr()[current_node];
             j < csr.rowPtr()[current_node + 1]; j++) {
          IndexType neighborId = csr.colIdx()[j];

          if (visited_nodes[neighborId] == 0) {
            neighbors.push_back(neighborId);
            visited_nodes[neighborId] = 1;
          }
        }

        // sort neighbors by degree
        std::sort(neighbors.begin(), neighbors.end(),
                  [&](IndexType a, IndexType b) {
                    return getNodeDegree(csr, a) < getNodeDegree(csr, b);
                  });

        // push neighbors to queue
        for (IndexType neighbor : neighbors) {
          visit_queue.push(neighbor);
        }

        // place current node in the permutation vector
        perm.push_back(current_node);
        visit_queue.pop();
        node_counter++;
      }
    }
  }

  std::reverse(perm.begin(), perm.end());
  return perm;
}
}  // namespace mtx::reorderings