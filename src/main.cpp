#include <iostream>
#include <vector>
#include <algorithm>

#include "csr.hpp"
#include "coo.hpp"
#include "mtxReader.hpp"

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
    // change structs into classes
    // add representative set of matrices
    // change COO struct to SoA instead of AoS
    // use enum in mtx reader?

    //VIP:
    //fix converting coo to class, fix sorting
    //fix how reading from mtx to coo class

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

    // Read mtx dimensions
    matrixStructure mtx;
    if (mm_read_mtx_crd_size(f, &mtx.num_rows, &mtx.num_cols, &mtx.entries))
    {
        std::cerr << "Could not read matrix dimensions.\n";
        exit(1);
    }

    COO entries(mtx.num_rows, mtx.num_cols);
    readMtxCoordinates(f, matcode, entries, mtx);
    sortCOO(entries);

    CSR csr(mtx.num_rows, mtx.num_cols);
    convertCOOToCSR(entries, csr, mtx.num_rows);

    std::cout << "mtx rows: " << mtx.num_rows << "\n";
    std::cout << "mtx cols: " << mtx.num_cols << "\n";
    std::cout << "mtx nz: " << mtx.nz << "\n";
    printCOO(entries);
    printCSR(csr);

    return 0;
}
