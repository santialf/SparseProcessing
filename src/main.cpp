#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "csr.hpp"
#include "coo.hpp"
#include "mtxReader.hpp"
#include "convertFormats.hpp"

using valueType = double;

using namespace mtx;
using namespace mtx::convert;
using namespace mtx::io;

int main(int argc, char* argv[]) 
{
    // TODO:
    // add unit tests
    // move functions to outside of user scope?
    // add blocked ell (feb?)
    // add matrix features (feb)
    
    if (argc != 2) 
    {
        std::cerr << "Error: expected 1 argument\n"
          << "Usage: " << argv[0] << " <mtx file>\n";
        return EXIT_FAILURE;
    }

    std::filesystem::path path{argv[1]};
    auto coo = readMtxToCOO<valueType>(path);

    coo.sortByCol();
    auto csc = COOToCSC(coo);
    coo.sortByRow();
    auto csr = COOToCSR(coo);
    auto coo_from_csr = CSRToCOO(csr);
    auto coo_from_csc = CSCToCOO(csc);

    std::cout << "COO:\n"; 
    coo.print();
    std::cout << "CSR:\n"; 
    csr.print();
    std::cout << "CSC:\n"; 
    csc.print();

    return 0;
}
