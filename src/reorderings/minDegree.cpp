#include "minDegree.hpp"

#include <algorithm>
#include <vector>

#include "../formats/csr.hpp"
#include "rcm.hpp"

using namespace mtx;

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
IndexType getNodeDegreeMinDegree(const CSR<IndexType, ValueType>& csr,
                                 IndexType node) {
  return csr.rowPtr()[node + 1] - csr.rowPtr()[node];
}

// SlashBurn reordering: reorder nodes by burn time to group communities
template <typename IndexType, typename ValueType>
std::vector<IndexType> minDegree(const mtx::CSR<IndexType, ValueType>& csr) {
  std::vector<IndexType> perm(csr.nrows());
  std::vector<IndexType> node_degrees(csr.nrows());

  for (IndexType i = 0; i < csr.nrows(); ++i) {
    node_degrees[i] = getNodeDegreeMinDegree(csr, i);
    perm[i] = i;
  }

  std::sort(perm.begin(), perm.end(), [&](IndexType a, IndexType b) {
    return node_degrees[a] < node_degrees[b];
  });

  return perm;
}
}  // namespace mtx::reorderings