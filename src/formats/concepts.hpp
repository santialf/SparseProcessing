#pragma once

#include <complex>
#include <concepts>
#include <type_traits>

template <typename IndexType>
concept SparseIndex = std::unsigned_integral<IndexType>;

template <typename ValueType>
concept SparseValue = std::is_arithmetic_v<ValueType> ||
    std::is_same_v<ValueType, std::complex<int>> ||
    std::is_same_v<ValueType, std::complex<float>> ||
    std::is_same_v<ValueType, std::complex<double>> ||
    std::is_same_v<ValueType, std::complex<long double>>;
