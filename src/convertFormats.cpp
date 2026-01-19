#include "convertFormats.hpp"

namespace mtx::convert {

template<typename valueType>
CSR<valueType> COOToCSR(const COO<valueType>& coo)
{
    CSR<valueType> csr(coo.num_rows, coo.num_cols);

    csr.row_ptr.assign(csr.num_rows + 1, 0);

    for (int i=0; i<coo.values.size(); i++)
    {
        csr.col_indices.push_back(coo.col_indices[i]);
        csr.values.push_back(coo.values[i]);
        csr.row_ptr[coo.row_indices[i] + 1]++;
    }

    for (int i = 0; i < csr.num_rows; i++)
    {
        csr.row_ptr[i + 1] += csr.row_ptr[i];
    }

    return csr;
}

} // namespace mtx::convert