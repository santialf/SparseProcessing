#include <iostream>

#include "converter/convertFormats.hpp"
#include "formats/bell.hpp"
#include "formats/coo.hpp"
#include "formats/csc.hpp"
#include "formats/csr.hpp"
#include "reader/mtxReader.hpp"

using ValueType = double;

using namespace mtx;
using namespace mtx::convert;
using namespace mtx::io;

int main(int argc, char *argv[]) {
  // TODO:
  // add meaningful coments
  // organize files and functions
  // add matrix features (feb)
  // add reordering functions (march)
  // move functions to outside of user scope?

  if (argc != 2) {
    std::cerr << "Error: expected 1 argument\n"
              << "Usage: " << argv[0] << " <mtx file>\n";
    return EXIT_FAILURE;
  }

  std::filesystem::path path{argv[1]};
  auto coo = readMtxToCOO<ValueType>(path);

  coo.sortByCol();
  auto csc = COOToCSC(coo);
  coo.sortByRow();
  auto csr = COOToCSR(coo);
  auto coo_from_csr = CSRToCOO(csr);
  auto coo_from_csc = CSCToCOO(csc);
  auto bell = COOToBELL(coo, 2);

  std::cout << "COO:\n";
  coo.print();
  std::cout << "CSR:\n";
  csr.print();
  std::cout << "CSC:\n";
  csc.print();
  std::cout << "BELL:\n";
  bell.print();

  return 0;
}
