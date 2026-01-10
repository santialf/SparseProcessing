#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "coo.hpp"

struct CSR 
{
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
    std::vector<double> values;
};

void convertCOOToCSR(const std::vector<COO>& coo, CSR& csr, const int nrows);
void printCSR(const CSR& csr);
