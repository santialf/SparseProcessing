#pragma once

#include "../formats/concepts.hpp"
#include "../formats/csr.hpp"

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
inline IndexType getNodeDegree(const CSR<IndexType, ValueType>& csr,
                               IndexType row) {
  return csr.rowPtr()[row + 1] - csr.rowPtr()[row];
}

}  // namespace mtx::reorderings
