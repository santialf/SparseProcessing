#include "minDegree.hpp"

#include <algorithm>
#include <vector>

using namespace mtx;

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
std::vector<IndexType> minDegree(const CSR<IndexType, ValueType>& csr) {
  std::vector<IndexType> perm(csr.nrows());
  std::vector<IndexType> node_degrees(csr.nrows());

  for (IndexType i = 0; i < csr.nrows(); ++i) {
    node_degrees[i] = getNodeDegree(csr, i);
    perm[i] = i;
  }

  std::sort(perm.begin(), perm.end(), [&](IndexType a, IndexType b) {
    return node_degrees[a] < node_degrees[b];
  });

  return perm;
}
}  // namespace mtx::reorderings