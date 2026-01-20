#include "convertFormats.hpp"

namespace mtx::convert {

template<typename valueType>
CSR<valueType> COOToCSR(const COO<valueType>& coo)
{
    CSR<valueType> csr(coo.getRows(), coo.getCols());

    csr.row_ptr.assign(csr.getRows() + 1, 0);

    for (int i=0; i<coo.getNnz(); i++)
    {
        csr.col_indices.push_back(coo.colsIdx()[i]);
        csr.values.push_back(coo.vals()[i]);
        csr.row_ptr[coo.rowsIdx()[i] + 1]++;
    }

    for (int i = 0; i < csr.getRows(); i++)
    {
        csr.row_ptr[i + 1] += csr.row_ptr[i];
    }

    return csr;
}

} // namespace mtx::convert