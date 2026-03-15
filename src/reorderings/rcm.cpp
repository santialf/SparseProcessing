#include "rcm.hpp"

#include <queue>

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
IndexType findPeripheralNodeId(const CSR<IndexType, ValueType>& csr,
                               std::vector<IndexType> visited_nodes) {
  return 0;
}

template <typename IndexType, typename ValueType>
std::vector<IndexType> rcm(const CSR<IndexType, ValueType>& csr) {
  std::vector<IndexType> perm(csr.nrows()), visited_nodes;
  std::queue<IndexType> visit_queue;

  IndexType peripheral_node_id = findPeripheralNodeId(csr, visited_nodes);
  // place peripheral node in the queue
  // while loop iterating over the queue
  // place neighboring nodes in the queue by order of minimum degree
  // if queue is empty but there are still nodes to visit start over with
  // finding peripheral node

  return perm;
}
}  // namespace mtx::reorderings