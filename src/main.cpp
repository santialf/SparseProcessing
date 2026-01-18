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

using valueType = double;

int main(int argc, char* argv[]) 
{
    // TODO:
    // confirm if its being properly read
    // add unit tests
    // add representative set of matrices
    // convert FILE to streams
    // add csc
    // add blocked ell
    // add converts to other formats (ex: csr to coo, csr to csc, coo to csc, ...)

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

    auto coo = readMtxToCOO<valueType>(f);
    coo.sort();
    auto csr = convertCOOToCSR(coo);

    std::cout << "mtx rows: " << coo.num_rows << "\n";
    std::cout << "mtx cols: " << coo.num_cols << "\n";
    coo.print();
    csr.print();

    return 0;
}
