#include "convertFormats.hpp"

namespace mtx::convert {

template<typename valueType>
CSR<valueType> COOToCSR(const COO<valueType>& coo)
{
    if (!coo.isCooRowMajor())
    {
        throw std::invalid_argument("COO must be row-major to convert to CSR");
    }

    auto row_ptr = std::make_unique<size_t[]>(coo.nrows() + 1);
    auto col_idx = std::make_unique<size_t[]>(coo.nnz());
    auto vals   = std::make_unique<valueType[]>(coo.nnz());

    for (size_t i = 0; i < coo.nnz(); i++)
    {
        col_idx[i] = coo.colIdx()[i];
        vals[i] = coo.vals()[i];
        row_ptr[coo.rowIdx()[i] + 1]++;
    }

    for (size_t i = 0; i < coo.nrows(); i++)
    {
        row_ptr[i + 1] += row_ptr[i];
    }

    return CSR<valueType>(CSR<valueType>::adopt, row_ptr.release(), col_idx.release(), vals.release(),
        coo.nrows(), coo.ncols(), coo.nnz());
}

template<typename valueType>
CSC<valueType> COOToCSC(const COO<valueType>& coo)
{
    if (!coo.isCooColMajor()) 
    {
        throw std::invalid_argument("COO must be col-major to convert to CSR");
    }

    auto row_idx = std::make_unique<size_t[]>(coo.nnz());
    auto col_ptr = std::make_unique<size_t[]>(coo.ncols() + 1);
    auto vals   = std::make_unique<valueType[]>(coo.nnz());

    for (size_t i = 0; i < coo.nnz(); i++)
    {
        row_idx[i] = coo.rowIdx()[i];
        vals[i] = coo.vals()[i];
        col_ptr[coo.colIdx()[i] + 1]++;
    }

    for (size_t i = 0; i < coo.ncols(); i++)
    {
        col_ptr[i + 1] += col_ptr[i];
    }

    return CSC<valueType>(CSC<valueType>::adopt, row_idx.release(), col_ptr.release(), vals.release(),
        coo.nrows(), coo.ncols(), coo.nnz());
}

template<typename valueType>
COO<valueType> CSRToCOO(const CSR<valueType>& csr)
{
    auto row_idx = std::make_unique<size_t[]>(csr.nnz());
    auto col_idx = std::make_unique<size_t[]>(csr.nnz());
    auto vals   = std::make_unique<valueType[]>(csr.nnz());

    size_t k = 0;
    for (size_t i = 0; i < csr.nrows(); i++)
    {
        for (size_t j = csr.rowPtr()[i]; j < csr.rowPtr()[i + 1]; j++)
        {
            row_idx[k] = i;
            col_idx[k] = csr.colIdx()[j];
            vals[k]    = csr.vals()[j];
            k++;
        }
    }

    return COO<valueType>(COO<valueType>::adopt, row_idx.release(), col_idx.release(), vals.release(),
        csr.nrows(), csr.ncols(), csr.nnz());
}

template<typename valueType>
COO<valueType> CSCToCOO(const CSC<valueType>& csc)
{
    auto row_idx = std::make_unique<size_t[]>(csc.nnz());
    auto col_idx = std::make_unique<size_t[]>(csc.nnz());
    auto vals   = std::make_unique<valueType[]>(csc.nnz());

    size_t k = 0;
    for (size_t i = 0; i < csc.ncols(); i++)
    {
        for (size_t j = csc.colPtr()[i]; j < csc.colPtr()[i + 1]; j++)
        {
            col_idx[k] = i;
            row_idx[k] = csc.rowIdx()[j];
            vals[k]    = csc.vals()[j];
            k++;
        }
    }

    return COO<valueType>(COO<valueType>::adopt, row_idx.release(), col_idx.release(), vals.release(),
        csc.nrows(), csc.ncols(), csc.nnz());
}

} // namespace mtx::convert