#include <iostream>
#include <vector>
#include <algorithm>

#include "csr.hpp"
#include "coo.hpp"
#include "mtxReader.hpp"
#include "convertFormats.hpp"

extern "C" 
{
#include "mmio.h"
}

int main(int argc, char* argv[]) 
{
    // TODO:
    // confirm if its being properly read
    // implement reading of complex values
    // add unit tests
    // add representative set of matrices
    // use enum in mtx reader?
    // add template to coo and csr classes

    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <mtx file>\n";
        return 1;
    }

    // Open mtx file
    FILE *f;
    if ((f = fopen(argv[1], "r")) == NULL)
    {
        std::cerr << "Could not locate the matrix file. Please make sure the pathname is valid.\n";
        exit(1);
    }

    // Read mtx header
    MM_typecode matcode;
    if (mm_read_banner(f, &matcode))
    {
        std::cerr << "Could not process Matrix Market banner.\n";
        exit(1);
    }
    if (!mm_is_matrix(matcode) || !mm_is_coordinate(matcode)) {
        std::cerr << "Input must be a sparse Matrix Market matrix (coordinate format).\n";
        exit(1);
    }

    int num_rows, num_cols, num_entries;
    if (mm_read_mtx_crd_size(f, &num_rows, &num_cols, &num_entries))
    {
        std::cerr << "Could not read matrix dimensions.\n";
        exit(1);
    }

    COO coo(num_rows, num_cols);
    readMtxCoordinates(f, matcode, coo);
    coo.sort();

    CSR csr(num_rows, num_cols);
    convertCOOToCSR(coo, csr);

    std::cout << "mtx rows: " << num_rows << "\n";
    std::cout << "mtx cols: " << num_cols << "\n";
    coo.print();
    csr.print();

    return 0;
}
