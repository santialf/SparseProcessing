#include <gtest/gtest.h>

#include "converter/convertFormats.hpp"
#include "formats/coo.hpp"
#include "reader/mtxReader.hpp"

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

TEST(COOTest, ReadsMtxHeaderWithCases) {
  const std::string mtx =
      "%%MatRixmarkeT maTRix CoorDINate REAL geNeraL\n"
      "2 2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_NO_THROW({ auto coo = readMtxToCOO<double>(path); });
}

TEST(COOTest, ReadsInvalidMtxHeaderSwappedInfo) {
  const std::string mtx =
      "%%MatrixMarket matrix real coordinate general\n"
      "2 2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxHeaderMissingInfo) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate general\n"
      "2 2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxHeaderMisspell) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate reall general\n"
      "2 2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxDimensionsMissingParameter) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxDimensionsExtraParameter) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2 2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxBinaryWithValues) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate binary general\n"
      "2 2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxGeneralWithoutValues) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2 2\n"
      "1 1\n"
      "2 2\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxGeneralWithExtraValues) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2 2\n"
      "1 1 1.0 1\n"
      "2 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxWithMissingLines) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2 2\n"
      "1 1 1.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxWithExtraLines) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2 2\n"
      "1 1 1.0\n"
      "2 2 2.0\n"
      "2 1 3.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsInvalidMtxWithBadCharacters) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2 2\n"
      "1 1 B.0\n"
      "1 2 2.0\n";

  auto path = writeTempMtx(mtx);

  EXPECT_THROW({ auto coo = readMtxToCOO<double>(path); }, std::runtime_error);
}

TEST(COOTest, ReadsRealGeneralMatrix) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real general\n"
      "2 2 2\n"
      "1 1 1.0\n"
      "2 2 -2.0\n";

  auto path = writeTempMtx(mtx);
  auto coo = readMtxToCOO<double>(path);

  EXPECT_EQ(coo.nnz(), 2);
  EXPECT_EQ(coo.vals()[0], 1.0);
  EXPECT_EQ(coo.vals()[1], -2.0);
}

TEST(COOTest, ReadsComplexMatrix) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate complex general\n"
      "2 2 2\n"
      "1 1 1.0 2\n"
      "2 2 2.0 -6.3\n";

  auto path = writeTempMtx(mtx);
  auto coo = readMtxToCOO<std::complex<double>>(path);

  EXPECT_EQ(coo.nnz(), 2);

  EXPECT_DOUBLE_EQ(coo.vals()[0].real(), 1.0);
  EXPECT_DOUBLE_EQ(coo.vals()[0].imag(), 2.0);
  EXPECT_DOUBLE_EQ(coo.vals()[1].real(), 2.0);
  EXPECT_DOUBLE_EQ(coo.vals()[1].imag(), -6.3);
}

TEST(COOTest, ReadsRealSymmetricMatrix) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real symmetric\n"
      "5 5 5\n"
      "2 1 1.0\n"
      "4 1 2.0\n"
      "3 2 3.0\n"
      "5 3 4.0\n"
      "5 4 5.0\n";

  auto path = writeTempMtx(mtx);
  auto coo = readMtxToCOO<double>(path);

  EXPECT_EQ(coo.nnz(), 10);

  EXPECT_EQ(coo.vals()[0], coo.vals()[1]);
  EXPECT_EQ(coo.vals()[2], coo.vals()[3]);
  EXPECT_EQ(coo.vals()[4], coo.vals()[5]);
  EXPECT_EQ(coo.vals()[6], coo.vals()[7]);
  EXPECT_EQ(coo.vals()[8], coo.vals()[9]);
}

TEST(COOTest, ReadsBinarySymmetricMatrix) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate pattern symmetric\n"
      "5 5 5\n"
      "2 1\n"
      "4 1\n"
      "3 2\n"
      "5 3\n"
      "5 4\n";

  auto path = writeTempMtx(mtx);
  auto coo = readMtxToCOO<double>(path);

  EXPECT_EQ(coo.nnz(), 10);

  EXPECT_EQ(coo.rowIdx()[0], coo.colIdx()[1]);
  EXPECT_EQ(coo.rowIdx()[2], coo.colIdx()[3]);
  EXPECT_EQ(coo.rowIdx()[4], coo.colIdx()[5]);
  EXPECT_EQ(coo.rowIdx()[6], coo.colIdx()[7]);
  EXPECT_EQ(coo.rowIdx()[8], coo.colIdx()[9]);

  EXPECT_EQ(coo.colIdx()[0], coo.rowIdx()[1]);
  EXPECT_EQ(coo.colIdx()[2], coo.rowIdx()[3]);
  EXPECT_EQ(coo.colIdx()[4], coo.rowIdx()[5]);
  EXPECT_EQ(coo.colIdx()[6], coo.rowIdx()[7]);
  EXPECT_EQ(coo.colIdx()[8], coo.rowIdx()[9]);
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