#include "rcm.hpp"

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
std::vector<IndexType> rcm(const CSR<IndexType, ValueType>& csr) {
  std::cout << "hello world\n";
  std::vector<IndexType> perm(0);
  return perm;
}
}  // namespace mtx::reorderings