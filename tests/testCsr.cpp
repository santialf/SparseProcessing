#include <gtest/gtest.h>

#include "converter/convertFormats.hpp"
#include "formats/coo.hpp"
#include "formats/csr.hpp"
#include "reader/mtxReader.hpp"

using namespace mtx;
using namespace mtx::convert;
using namespace mtx::io;

TEST(CSRTest, COOToCSRProducesCorrectRowPtr) {
  size_t rows[] = {0, 1, 1};
  size_t cols[] = {0, 0, 1};
  double vals[] = {1, 2, 3};

  COO<size_t, double> coo(rows, cols, vals, 2, 2, 3,
                          COO<size_t, double>::Order::RowMajor);
  auto csr = COOToCSR(coo);

  EXPECT_EQ(csr.rowPtr()[0], 0);
  EXPECT_EQ(csr.rowPtr()[1], 1);
  EXPECT_EQ(csr.rowPtr()[2], 3);
}

TEST(CSRTest, COOToCSRKeepsDimensions) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();
  auto csr = COOToCSR(coo);

  EXPECT_EQ(csr.nrows(), 5);
  EXPECT_EQ(csr.ncols(), 5);
  EXPECT_EQ(csr.nnzs(), 10);
}

TEST(CSRTest, COOToCSRInvalidCOONotSorted) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);

  EXPECT_THROW({ auto csr = COOToCSR(coo); }, std::invalid_argument);
}

TEST(CSRTest, COOToCSRProducesCorrectArrays) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();
  auto csr = COOToCSR(coo);

  // expected CSR data
  const std::vector<size_t> expectedRowPtr = {0, 2, 4, 6, 8, 10};
  const std::vector<size_t> expectedColIdx = {1, 3, 0, 2, 1, 4, 0, 4, 2, 3};
  const std::vector<double> expectedVals = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  // verify rowPtr
  for (size_t i = 0; i < expectedRowPtr.size(); ++i) {
    EXPECT_EQ(csr.rowPtr()[i], expectedRowPtr[i])
        << "Mismatch in rowPtr at index " << i;
  }

  // verify colIdx
  for (size_t i = 0; i < expectedColIdx.size(); ++i) {
    EXPECT_EQ(csr.colIdx()[i], expectedColIdx[i])
        << "Mismatch in colIdx at index " << i;
  }

  // verify vals
  for (size_t i = 0; i < expectedVals.size(); ++i) {
    EXPECT_DOUBLE_EQ(csr.vals()[i], expectedVals[i])
        << "Mismatch in vals at index " << i;
  }
}