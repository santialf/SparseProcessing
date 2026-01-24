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
    // build proper error handling
    // add logging system
    // confirm if its being properly read
    // add unit tests
    // add blocked ell (feb?)
    // add matrix features (feb)
    

    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <mtx file>\n";
        return 1;
    }

    std::filesystem::path path{argv[1]};
    auto coo = readMtxToCOO<valueType>(path);

    coo.sortByCol();
    auto csc = COOToCSC(coo);
    coo.sortByRow();
    auto csr = COOToCSR(coo);
    auto coo_from_csr = CSRToCOO(csr);
    auto coo_from_csc = CSCToCOO(csc);

    coo.print();
    csr.print();
    csc.print();
    coo_from_csr.print();
    coo_from_csc.print();

    return 0;
}
