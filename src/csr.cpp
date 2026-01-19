#include "csr.hpp"

namespace mtx {

template<typename valueType>
void CSR<valueType>::print()
{
    std::cout << "row_ptr: ";
    for (size_t v : row_ptr) 
    {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "col_idx: ";
    for (size_t v : col_indices) 
    {
        std::cout << v << " ";
    }   
    std::cout << "\n";

    std::cout << "values : ";
    for (valueType v : values) 
    {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

template<typename valueType>
void CSR<valueType>::clear() 
{
    row_ptr.clear();
    col_indices.clear();
    values.clear();
}

} // namespace mtx