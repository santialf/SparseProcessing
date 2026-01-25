#include "convertFormats.hpp"
#include "coo.hpp"
#include "mtxReader.hpp"
#include <gtest/gtest.h>

using namespace mtx;
using namespace mtx::convert;
using namespace mtx::io;

static std::filesystem::path writeTempMtx(const std::string &content) {
  auto path = std::filesystem::temp_directory_path() / "test_matrix.mtx";
  std::ofstream ofs(path);
  ofs << content;
  ofs.close();
  return path;
}

TEST(COOTest, ConstructionKeepsDimensions) {
  size_t rows[] = {0, 1};
  size_t cols[] = {1, 0};
  double vals[] = {3.0, 4.0};

  COO<double> coo(rows, cols, vals, 2, 2, 2);

  EXPECT_EQ(coo.nrows(), 2);
  EXPECT_EQ(coo.ncols(), 2);
  EXPECT_EQ(coo.nnz(), 2);
}

TEST(COOTest, SortByRowProducesRowMajorOrder) {
  size_t rows[] = {2, 0, 1};
  size_t cols[] = {0, 1, 0};
  int vals[] = {3, 1, 2};

  COO<int> coo(rows, cols, vals, 3, 2, 3);
  coo.sortByRow();

  const size_t *r = coo.rowIdx();
  const size_t *c = coo.colIdx();

  EXPECT_TRUE((r[0] < r[1]) || (r[0] == r[1] && c[0] <= c[1]));
}

TEST(ConvertTest, COOToCSRProducesCorrectRowPtr) {
  size_t rows[] = {0, 1, 1};
  size_t cols[] = {0, 0, 1};
  double vals[] = {1, 2, 3};

  COO<double> coo(rows, cols, vals, 2, 2, 3, COO<double>::Order::RowMajor);
  auto csr = COOToCSR(coo);

  EXPECT_EQ(csr.rowPtr()[0], 0);
  EXPECT_EQ(csr.rowPtr()[1], 1);
  EXPECT_EQ(csr.rowPtr()[2], 3);
}

TEST(MtxReaderTest, ReadsRealGeneralMatrix) {
  const std::string mtx = "%%MatrixMarket matrix coordinate real general\n"
                          "2 2 2\n"
                          "1 1 1.0\n"
                          "2 2 2.0\n";

  auto path = writeTempMtx(mtx);
  auto coo = readMtxToCOO<double>(path);

  EXPECT_EQ(coo.nnz(), 2);
  EXPECT_EQ(coo.vals()[0], 1.0);
}

// tests where everything is passing
/*
-COO
    -square matrix
    -rectangular matrix
    -pattern
    -general
    -symmetric
    -
-CSR
-CSC
 */
// tests with edge cases
// testing indexation
// testing correct values
// testing robustness (diff ordering, extra)