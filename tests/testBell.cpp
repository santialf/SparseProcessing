#include <gtest/gtest.h>

#include "bell.hpp"
#include "convertFormats.hpp"
#include "coo.hpp"
#include "mtxReader.hpp"

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

TEST(BELLTest, ConstructionKeepsDimensions) {
  size_t rows[] = {0, 1};
  size_t cols[] = {1, 0};
  double vals[] = {3.0, 4.0};

  COO<double> coo(rows, cols, vals, 2, 2, 2);

  EXPECT_EQ(coo.nrows(), 2);
  EXPECT_EQ(coo.ncols(), 2);
  EXPECT_EQ(coo.nnz(), 2);
}