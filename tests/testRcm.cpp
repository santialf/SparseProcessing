#include <gtest/gtest.h>

#include "converter/convertFormats.hpp"
#include "formats/coo.hpp"
#include "formats/csr.hpp"
#include "reader/mtxReader.hpp"
#include "reorderings/common.hpp"
#include "reorderings/rcm.hpp"

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

TEST(RCMTest, RCMPermutation) {
  const std::string mtx =
      "%%MatrixMarket matrix coordinate real symmetric\n"
      "14 14 30\n"
      "1 1 1.57088\n"
      "4 1 -94.2528\n"
      "5 1 0.78544\n"
      "2 2 1.25664e7\n"
      "6 2 -6.2832e6\n"
      "3 3 0.6088062015503876\n"
      "7 3 -0.3044031007751938\n"
      "4 4 15080.447999999997\n"
      "8 4 -7540.223999999998\n"
      "9 4 94.2528\n"
      "5 5 3.14176\n"
      "8 5 -94.2528\n"
      "9 5 0.78544\n"
      "6 6 1.25664e7\n"
      "10 6 -6.2832e6\n"
      "7 7 0.6088062015503876\n"
      "11 7 -0.3044031007751938\n"
      "8 8 15080.447999999997\n"
      "12 8 -7540.223999999998\n"
      "13 8 94.2528\n"
      "9 9 3.14176\n"
      "12 9 -94.2528\n"
      "13 9 0.78544\n"
      "10 10 1.25664e7\n"
      "11 11 0.6088062015503876\n"
      "12 12 15080.447999999997\n"
      "14 12 94.2528\n"
      "13 13 3.14176\n"
      "14 13 0.78544\n"
      "14 14 1.57088\n";

  auto path = writeTempMtx(mtx);
  auto coo = readMtxToCOO<size_t, double>(path);

  coo.sortByRow();
  auto csr = COOToCSR(coo);

  auto perm = mtx::reorderings::rcm(csr);

  const std::vector<size_t> expectedPerm = {10, 6,  2, 9, 5, 1, 13,
                                            12, 11, 8, 7, 4, 3, 0};

  EXPECT_EQ(perm.size(), expectedPerm.size());
  for (size_t i = 0; i < expectedPerm.size(); ++i) {
    EXPECT_EQ(perm[i], expectedPerm[i])
        << "Mismatch in RCM permutation at index " << i;
  }
}
