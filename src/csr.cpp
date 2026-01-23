#include "csr.hpp"

namespace mtx {

template<typename valueType>
void CSR<valueType>::print()
{
    std::cout << "rows: " << nrows_ << " " 
              << "cols: " << ncols_ << " " 
              << "nnzs: " << nnz_ << "\n";

    std::cout << "rowPtr: ";
    for (size_t i=0; i<nrows_ + 1; i++)
    {
        std::cout << rowPtr_[i] << " ";
    }
    std::cout << "\n";
    std::cout << "colIdx: ";
    for (size_t i=0; i<nnz_; i++)
    {
        std::cout << colIdx_[i] << " ";
    }
    std::cout << "\n";
    std::cout << "vals: ";
    for (size_t i=0; i<nnz_; i++)
    {
        std::cout << vals_[i] << " ";
    }
}

} // namespace mtx