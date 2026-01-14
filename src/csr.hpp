#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "coo.hpp"

class CSR {
public:
    size_t num_rows;
    size_t num_cols;

    std::vector<size_t> row_ptr;
    std::vector<size_t> col_indices;
    std::vector<double> values;

    CSR(size_t rows_, size_t cols_)
    : num_rows(rows_), num_cols(cols_) {}

    void clear() {
        row_ptr.clear();
        col_indices.clear();
        values.clear();
    }
};

void convertCOOToCSR(const COO& coo, CSR& csr, const int nrows);
void printCSR(const CSR& csr);
