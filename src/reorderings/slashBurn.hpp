#pragma once

#include <cstdlib>
#include <cstring>
#include <vector>

#include "formats/concepts.hpp"
#include "formats/csr.hpp"

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
std::vector<IndexType> slashBurn(const CSR<IndexType, ValueType>&);
}  // namespace mtx::reorderings

#ifdef _HEADER_ONLY
#include "rcm.cpp"
#endif
