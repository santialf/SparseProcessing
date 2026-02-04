#pragma once

namespace mtx::features {

namespace detail {

template <typename FormatType>
double totalEntries(const FormatType&);

}

template <typename FormatType>
double density(const FormatType&);

template <typename FormatType>
double sparsity(const FormatType&);

}  // namespace mtx::features

#ifdef _HEADER_ONLY
#include "mtxFeatures.cpp"
#endif
