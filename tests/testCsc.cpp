#include <gtest/gtest.h>

#include "converter/convertFormats.hpp"
#include "formats/coo.hpp"
#include "formats/csc.hpp"
#include "reader/mtxReader.hpp"

using namespace mtx;
using namespace mtx::convert;
using namespace mtx::io;

TEST(CSCTest, COOToCSCProducesCorrectColPtr) {
  size_t rows[] = {0, 1, 1};
  size_t cols[] = {0, 0, 1};
  double vals[] = {1, 2, 3};

  COO<size_t, double> coo(rows, cols, vals, 2, 2, 3,
                          COO<size_t, double>::Order::ColMajor);
  auto csc = COOToCSC(coo);

  EXPECT_EQ(csc.colPtr()[0], 0);
  EXPECT_EQ(csc.colPtr()[1], 2);
  EXPECT_EQ(csc.colPtr()[2], 3);
}

TEST(CSCTest, COOToCSCKeepsDimensions) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByCol();
  auto csc = COOToCSC(coo);

  EXPECT_EQ(csc.nrows(), 5);
  EXPECT_EQ(csc.ncols(), 5);
  EXPECT_EQ(csc.nnzs(), 10);
}

TEST(CSCTest, COOToCSCInvalidCOONotSorted) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);

  EXPECT_THROW({ auto csc = COOToCSC(coo); }, std::invalid_argument);
}

TEST(CSCTest, COOToCSCProducesCorrectArrays) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByCol();
  auto csc = COOToCSC(coo);

  // expected CSC data
  const std::vector<size_t> expectedColPtr = {0, 2, 4, 6, 8, 10};
  const std::vector<size_t> expectedRowIdx = {1, 3, 0, 2, 1, 4, 0, 4, 2, 3};
  const std::vector<double> expectedVals = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  // verify rowPtr
  for (size_t i = 0; i < expectedColPtr.size(); ++i) {
    EXPECT_EQ(csc.colPtr()[i], expectedColPtr[i])
        << "Mismatch in rowPtr at index " << i;
  }

  // verify colIdx
  for (size_t i = 0; i < expectedRowIdx.size(); ++i) {
    EXPECT_EQ(csc.rowIdx()[i], expectedRowIdx[i])
        << "Mismatch in colIdx at index " << i;
  }

  // verify vals
  for (size_t i = 0; i < expectedVals.size(); ++i) {
    EXPECT_DOUBLE_EQ(csc.vals()[i], expectedVals[i])
        << "Mismatch in vals at index " << i;
  }
}