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
  std::vector<IndexType> perm;
  std::queue<IndexType> visit_queue;

  // while (perm.size() < csr.nrows()) {
  IndexType peripheral_node_id = findPeripheralNodeId(csr, perm);
  visit_queue.push(peripheral_node_id);

  while (!visit_queue.empty()) {
    IndexType current_node = visit_queue.back();

    for (IndexType i = csr.rowPtr()[current_node];
         i < csr.rowPtr()[current_node + 1]; i++) {
      // if neighbor hasnt been visited
      // add neighbours by order of minimum degree
    }

    perm.push_back(current_node);
    visit_queue.pop();
  }
  //}

  return perm;
}
}  // namespace mtx::reorderings