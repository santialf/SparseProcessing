#include <iostream>
#include <vector>
#include <algorithm>

#include "csr.hpp"

void convertCOOToCSR(const std::vector<COO>& coo, CSR& csr, const int nrows)
{
    csr.row_ptr.assign(nrows + 1, 0);

    for (const auto& e : coo) 
    {
        csr.col_idx.push_back(e.col);
        csr.values.push_back(e.val);
        csr.row_ptr[e.row + 1]++;
    }

    for (int i = 0; i < nrows; i++)
    {
        csr.row_ptr[i + 1] += csr.row_ptr[i];
    }
}

void printCSR(const CSR& csr)
{
    std::cout << "row_ptr: ";
    for (int v : csr.row_ptr) 
    {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "col_idx: ";
    for (int v : csr.col_idx) 
    {
        std::cout << v << " ";
    }   
    std::cout << "\n";

    std::cout << "values : ";
    for (double v : csr.values) 
    {
        std::cout << v << " ";
    }
    std::cout << "\n";
}