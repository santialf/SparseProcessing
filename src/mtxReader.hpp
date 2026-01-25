#pragma once

#include <cassert>
#include <complex>
#include <filesystem>
#include <fstream>

#include "coo.hpp"

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

bool readCOOLine(std::ifstream &, size_t &, size_t &);
bool readCOOLine(std::ifstream &, size_t &, size_t &, std::complex<double> &);
template <typename ValueType>
bool readCOOLine(std::ifstream &, size_t &, size_t &, ValueType &);
template <typename ValueType>
COO<ValueType> readCOO(std::ifstream &, const MtxStructure &);
template <typename ValueType>
COO<ValueType> readMtxToCOO(const std::filesystem::path &);

template <typename ValueType>
size_t countNnzs(std::ifstream &, const MtxStructure &);

std::ifstream openFile(const std::filesystem::path &);

}  // namespace mtx::io

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif
