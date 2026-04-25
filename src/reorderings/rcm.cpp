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

// Function to find the farthest node from a given root using BFS
template <typename IndexType, typename ValueType>
IndexType findFarthestNode(const CSR<IndexType, ValueType>& csr,
                           IndexType root) {
  std::vector<char> visited(csr.nrows(), 0);
  std::queue<IndexType> bfs_queue;
  bfs_queue.push(root);
  visited[root] = 1;
  IndexType farthest = root;

  while (!bfs_queue.empty()) {
    IndexType node = bfs_queue.front();
    bfs_queue.pop();
    farthest = node;

    for (IndexType i = csr.rowPtr()[node]; i < csr.rowPtr()[node + 1]; ++i) {
      IndexType neighbor = csr.colIdx()[i];
      if (!visited[neighbor]) {
        visited[neighbor] = 1;
        bfs_queue.push(neighbor);
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

template <typename IndexType, typename ValueType>
std::vector<IndexType> rcm(const CSR<IndexType, ValueType>& csr) {
  std::vector<IndexType> perm;
  std::vector<bool> visited_nodes(csr.nrows(), false);
  std::queue<IndexType> visit_queue;
  IndexType node_counter = 0;

  for (IndexType i = 0; i < csr.nrows(); i++) {
    if (!visited_nodes[i]) {
      IndexType peripheral_node_id = findPeripheralNode(csr, i);
      visit_queue.push(peripheral_node_id);
      visited_nodes[peripheral_node_id] = true;

      // go through all of the nodes in a connected component
      while (!visit_queue.empty()) {
        IndexType current_node = visit_queue.front();
        std::vector<IndexType> neighbors;

        // visit neighbors of the current node
        for (IndexType j = csr.rowPtr()[current_node];
             j < csr.rowPtr()[current_node + 1]; j++) {
          IndexType neighborId = csr.colIdx()[j];

          if (!visited_nodes[neighborId]) {
            neighbors.push_back(neighborId);
            visited_nodes[neighborId] = true;
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