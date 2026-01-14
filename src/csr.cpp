#include <iostream>
#include <vector>
#include <algorithm>

#include "csr.hpp"

void convertCOOToCSR(const COO& coo, CSR& csr, const int nrows)
{
    csr.row_ptr.assign(nrows + 1, 0);

    for (int i=0; i<coo.values.size(); i++) 
    {
        csr.col_indices.push_back(coo.col_indices[i]);
        csr.values.push_back(coo.values[i]);
        csr.row_ptr[coo.row_indices[i] + 1]++;
    }

    for (int i = 0; i < nrows; i++)
    {
        csr.row_ptr[i + 1] += csr.row_ptr[i];
    }
}

void CSR::print()
{
    std::cout << "row_ptr: ";
    for (int v : row_ptr) 
    {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "col_idx: ";
    for (int v : col_indices) 
    {
        std::cout << v << " ";
    }   
    std::cout << "\n";

    std::cout << "values : ";
    for (double v : values) 
    {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void CSR::clear() 
{
    row_ptr.clear();
    col_indices.clear();
    values.clear();
}