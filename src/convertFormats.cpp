#include "convertFormats.hpp"

namespace mtx::convert {

template<typename valueType>
CSR<valueType> COOToCSR(const COO<valueType>& coo)
{
    auto row_ptr = std::make_unique<size_t[]>(coo.nrows() + 1);
    auto col_idx = std::make_unique<size_t[]>(coo.nnz());
    auto vals   = std::make_unique<valueType[]>(coo.nnz());

    for (int i = 0; i < coo.nnz(); i++)
    {
        col_idx[i] = coo.colIdx()[i];
        vals[i] = coo.vals()[i];
        row_ptr[coo.rowIdx()[i] + 1]++;
    }

    for (int i = 0; i < coo.nrows(); i++)
    {
        row_ptr[i + 1] += row_ptr[i];
    }

    return CSR<valueType>(CSR<valueType>::adopt, row_ptr.release(), col_idx.release(), vals.release(),
        coo.nrows(), coo.ncols(), coo.nnz());
}

template<typename valueType>
CSC<valueType> COOToCSC(const COO<valueType>& coo)
{
    auto row_idx = std::make_unique<size_t[]>(coo.nnz());
    auto col_ptr = std::make_unique<size_t[]>(coo.ncols() + 1);
    auto vals   = std::make_unique<valueType[]>(coo.nnz());

    for (int i = 0; i < coo.nnz(); i++)
    {
        row_idx[i] = coo.rowIdx()[i];
        vals[i] = coo.vals()[i];
        col_ptr[coo.colIdx()[i] + 1]++;
    }

    for (int i = 0; i < coo.ncols(); i++)
    {
        col_ptr[i + 1] += col_ptr[i];
    }

    return CSC<valueType>(CSC<valueType>::adopt, row_idx.release(), col_ptr.release(), vals.release(),
        coo.nrows(), coo.ncols(), coo.nnz());
}

} // namespace mtx::convert