#pragma once

#include <cassert>
#include <complex>
#include <filesystem>
#include <fstream>

#include "formats/coo.hpp"

namespace mtx::io {

enum class MtxSymmetry { general, symmetric, skewed, hermitian };

enum class MtxValueType { pattern, integer, real, complex };

enum class MtxStorage { sparse, dense };

struct MtxStructure {
  size_t num_rows;
  size_t num_cols;
  size_t num_entries;
  size_t num_nnzs;
  MtxSymmetry symmetry;
  MtxValueType type;
  MtxStorage storage;
};

std::string toLower(std::string s);
void parseMtxStorage(MtxStructure &, std::string);
void parseMtxSymmetry(MtxStructure &, std::string);
void parseMtxType(MtxStructure &, std::string);
MtxStructure parseMtx(std::ifstream &);

template <typename IndexType>
bool readCOOLine(std::ifstream &, IndexType &, IndexType &);
template <typename IndexType>
bool readCOOLine(std::ifstream &, IndexType &, IndexType &,
                 std::complex<double> &);
template <typename IndexType, typename ValueType>
bool readCOOLine(std::ifstream &, IndexType &, IndexType &, ValueType &);
template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> readCOO(std::ifstream &, const MtxStructure &);
template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> readMtxToCOO(const std::string &);

template <typename IndexType, typename ValueType>
IndexType countNnzs(std::ifstream &, const MtxStructure &);

std::ifstream openFile(const std::filesystem::path &);

}  // namespace mtx::io

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif
