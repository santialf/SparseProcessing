#include <gtest/gtest.h>

#include "converter/convertFormats.hpp"
#include "formats/bell.hpp"
#include "formats/coo.hpp"
#include "reader/mtxReader.hpp"

using namespace mtx;
using namespace mtx::convert;
using namespace mtx::io;

TEST(BELLTest, ConstructionPadsDimensions) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();
  auto bell = COOToBELL<size_t, double>(coo, 2);

  EXPECT_EQ(bell.nrows(), 6);
  EXPECT_EQ(bell.ncols(), 6);
  EXPECT_EQ(bell.ellCols(), 4);
  EXPECT_EQ(bell.blockSize(), 2);
}

TEST(BELLTest, ConstructionCreatesBellPointers) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();
  auto bell = COOToBELL<size_t, double>(coo, 2);

  // expected BELL data
  const std::vector<int> expectedColBlockIdx = {0, 1, 0, 2, 1, -1};
  const std::vector<double> expectedVals = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0,
                                            1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0};

  // verify colBlockIdx
  for (size_t i = 0; i < expectedColBlockIdx.size(); ++i) {
    EXPECT_EQ(bell.colBlockIdx()[i], expectedColBlockIdx[i])
        << "Mismatch in colBlockIdx at index " << i;
  }

  // verify vals
  for (size_t i = 0; i < expectedVals.size(); ++i) {
    EXPECT_DOUBLE_EQ(bell.vals()[i], expectedVals[i])
        << "Mismatch in vals at index " << i;
  }
}

TEST(BELLTest, BlockSizeSameAsDimensions) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();
  auto bell = COOToBELL<size_t, double>(coo, 5);

  // expected BELL data
  const std::vector<int> expectedColBlockIdx = {0};
  const std::vector<double> expectedVals = {0, 1, 0, 1, 0, 1, 0, 1, 0,
                                            0, 0, 1, 0, 0, 1, 1, 0, 0,
                                            0, 1, 0, 0, 1, 1, 0};

  EXPECT_EQ(bell.ellCols(), 5);

  // verify colBlockIdx
  for (size_t i = 0; i < expectedColBlockIdx.size(); ++i) {
    EXPECT_EQ(bell.colBlockIdx()[i], expectedColBlockIdx[i])
        << "Mismatch in colBlockIdx at index " << i;
  }

  // verify vals
  for (size_t i = 0; i < expectedVals.size(); ++i) {
    EXPECT_DOUBLE_EQ(bell.vals()[i], expectedVals[i])
        << "Mismatch in vals at index " << i;
  }
}

TEST(BELLTest, BlockSizeBiggerThanDimensions) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();
  auto bell = COOToBELL<size_t, double>(coo, 8);

  // expected BELL data
  const std::vector<int> expectedColBlockIdx = {0};
  const std::vector<double> expectedVals = {
      0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  EXPECT_EQ(bell.ellCols(), 8);

  // verify colBlockIdx
  for (size_t i = 0; i < expectedColBlockIdx.size(); ++i) {
    EXPECT_EQ(bell.colBlockIdx()[i], expectedColBlockIdx[i])
        << "Mismatch in colBlockIdx at index " << i;
  }

  // verify vals
  for (size_t i = 0; i < expectedVals.size(); ++i) {
    EXPECT_DOUBLE_EQ(bell.vals()[i], expectedVals[i])
        << "Mismatch in vals at index " << i;
  }
}

TEST(BELLTest, BlockSizeBiggerThan64) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();

  EXPECT_THROW((COOToBELL<size_t, double>(coo, 65)), std::invalid_argument);
}

TEST(BELLTest, BlockSize0) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();

  EXPECT_THROW((COOToBELL<size_t, double>(coo, 0)), std::invalid_argument);
}

TEST(BELLTest, BlockSizeNegative) {
  size_t rows[] = {1, 3, 2, 4, 4, 0, 0, 1, 2, 3};
  size_t cols[] = {0, 0, 1, 2, 3, 1, 3, 2, 4, 4};
  double vals[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  COO<size_t, double> coo(rows, cols, vals, 5, 5, 10);
  coo.sortByRow();

  EXPECT_THROW((COOToBELL<size_t, double>(coo, -1)), std::invalid_argument);
}