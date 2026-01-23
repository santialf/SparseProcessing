#include "convertFormats.hpp"

namespace mtx::convert {

template<typename valueType>
CSR<valueType> COOToCSR(const COO<valueType>& coo)
{
    auto rowPtr = std::make_unique<size_t[]>(coo.nrows() + 1);
    auto colIdx = std::make_unique<size_t[]>(coo.nnz());
    auto vals   = std::make_unique<valueType[]>(coo.nnz());

    for (int i = 0; i < coo.nnz(); i++)
    {
        colIdx[i] = coo.col()[i];
        vals[i] = coo.val()[i];
        rowPtr[coo.row()[i] + 1]++;
    }

    for (int i = 0; i < coo.nrows(); i++)
    {
        rowPtr[i + 1] += rowPtr[i];
    }

    return CSR<valueType>(CSR<valueType>::adopt, rowPtr.release(), colIdx.release(), vals.release(),
        coo.nrows(), coo.ncols(), coo.nnz());
}

} // namespace mtx::convert