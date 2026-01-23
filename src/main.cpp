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
    // confirm if its being properly read
    // add unit tests
    // add csc
    // add blocked ell
    // add converts to other formats (ex: csr to coo, csr to csc, coo to csc, ...)
    // add logs
    // create new file for matrix features
    // build proper error handling

    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <mtx file>\n";
        return 1;
    }

    std::filesystem::path path{argv[1]};
    auto coo = readMtxToCOO<valueType>(path);
    
    coo.sortByCol();
    coo.sortByRow();
    //auto csr = COOToCSR(coo);

    coo.print();
    //csr.print();

    return 0;
}
