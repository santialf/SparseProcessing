#include <iostream>

#include "converter/convertFormats.hpp"
#include "features/mtxFeatures.hpp"
#include "formats/bell.hpp"
#include "formats/coo.hpp"
#include "formats/csc.hpp"
#include "formats/csr.hpp"
#include "reader/mtxReader.hpp"

using ValueType = double;
using IndexType = size_t;

using namespace mtx;
using namespace mtx::convert;
using namespace mtx::io;

int main(int argc, char *argv[]) {
  // TODO:
  // add meaningful coments
  // add matrix features (feb)
  // add reordering functions (march)
  // move functions to outside of user scope?

  if (argc != 2) {
    std::cerr << "Error: expected 1 argument\n"
              << "Usage: " << argv[0] << " <mtx file>\n";
    return EXIT_FAILURE;
  }

  auto coo = readMtxToCOO<IndexType, ValueType>(argv[1]);

  coo.sortByCol();
  auto csc = COOToCSC<IndexType, ValueType>(coo);
  coo.sortByRow();
  auto csr = COOToCSR<IndexType, ValueType>(coo);
  auto coo_from_csr = CSRToCOO<IndexType, ValueType>(csr);
  auto coo_from_csc = CSCToCOO<IndexType, ValueType>(csc);
  auto bell = COOToBELL<IndexType, ValueType>(coo, 8);

  std::cout << "COO:\n";
  coo.print();
  std::cout << "CSR:\n";
  csr.print();
  std::cout << "CSC:\n";
  csc.print();
  std::cout << "BELL:\n";
  bell.print();

  double density = features::density(coo);
  std::cout << "Density: " << density << "\n";
  double sparsity = features::sparsity(coo);
  std::cout << "Sparsity: " << sparsity << "\n";

  return 0;
}
