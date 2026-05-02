#pragma once

#include <cstdlib>
#include <cstring>
#include <vector>

#include "common.hpp"

namespace mtx::reorderings {

template <typename IndexType, typename ValueType>
std::vector<IndexType> slashBurn(const CSR<IndexType, ValueType>&);
}  // namespace mtx::reorderings

#ifdef _HEADER_ONLY
#include "slashBurn.cpp"
#endif
