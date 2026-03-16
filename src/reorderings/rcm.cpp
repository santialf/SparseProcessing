#include "rcm.hpp"

#include <queue>
#include <vector>

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
IndexType getDegree(const CSR<IndexType, ValueType>& csr,
                    IndexType current_node) {
  return csr.rowPtr()[current_node + 1] - csr.rowPtr()[current_node];
}

template <typename IndexType, typename ValueType>
IndexType findPeripheralNodeId(const CSR<IndexType, ValueType>& csr,
                               std::vector<IndexType> visited_nodes) {
  // TODO
  return 0;
}

template <typename IndexType, typename ValueType>
std::vector<IndexType> rcm(const CSR<IndexType, ValueType>& csr) {
  std::vector<IndexType> perm;
  std::vector<bool> visited_nodes(csr.nrows(), false);
  std::queue<IndexType> visit_queue;
  IndexType node_counter = 0;

  while (node_counter < csr.nrows()) {
    IndexType peripheral_node_id = findPeripheralNodeId(csr, perm);
    visit_queue.push(peripheral_node_id);

    // go through all of the nodes in a connected component
    while (!visit_queue.empty()) {
      IndexType current_node = visit_queue.back();
      std::vector<IndexType> neighbors;

      // visit neighbors of the current node
      for (IndexType i = csr.rowPtr()[current_node];
           i < csr.rowPtr()[current_node + 1]; i++) {
        IndexType neighborId = csr.colIdx()[i];

        if (!visited_nodes[neighborId]) {
          neighbors.push_back(neighborId);
          visited_nodes[neighborId] = true;
        }
      }

      // sort neighbors by degree
      std::sort(neighbors.begin(), neighbors.end(),
                [&](IndexType a, IndexType b) {
                  return getDegree(csr, a) < getDegree(csr, b);
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

  return perm;
}
}  // namespace mtx::reorderings