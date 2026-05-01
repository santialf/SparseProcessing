#pragma once

#include <cstdlib>
#include <cstring>
#include <vector>

#include "formats/concepts.hpp"
#include "formats/csr.hpp"

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
IndexType getNodeDegreeMinDegree(const CSR<IndexType, ValueType>&, IndexType);

template <typename IndexType, typename ValueType>
std::vector<IndexType> minDegree(const CSR<IndexType, ValueType>&);
}  // namespace mtx::reorderings

#ifdef _HEADER_ONLY
#include "minDegree.cpp"
#endif
