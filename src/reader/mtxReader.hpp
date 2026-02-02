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

template <typename IndexType>
struct MtxStructure {
  IndexType num_rows;
  IndexType num_cols;
  IndexType num_entries;
  IndexType num_nnzs;
  MtxSymmetry symmetry;
  MtxValueType type;
  MtxStorage storage;
};

std::string toLower(std::string s);
template <typename IndexType>
void parseMtxStorage(MtxStructure<IndexType> &, std::string);
template <typename IndexType>
void parseMtxSymmetry(MtxStructure<IndexType> &, std::string);
template <typename IndexType>
void parseMtxType(MtxStructure<IndexType> &, std::string);
template <typename IndexType>
MtxStructure<IndexType> parseMtx(std::ifstream &);

template <typename IndexType>
bool readCOOLine(std::ifstream &, IndexType &, IndexType &);
template <typename IndexType>
bool readCOOLine(std::ifstream &, IndexType &, IndexType &,
                 std::complex<double> &);
template <typename IndexType, typename ValueType>
bool readCOOLine(std::ifstream &, IndexType &, IndexType &, ValueType &);
template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> readCOO(std::ifstream &,
                                  const MtxStructure<IndexType> &);
template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> readMtxToCOO(const std::string &);

template <typename IndexType, typename ValueType>
IndexType countNnzs(std::ifstream &, const MtxStructure<IndexType> &);

std::ifstream openFile(const std::filesystem::path &);

}  // namespace mtx::io

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif
