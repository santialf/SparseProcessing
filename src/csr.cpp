#include <iostream>
#include <vector>
#include <algorithm>

#include "csr.hpp"

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