# SparseProcessing


# Compile:
mkdir build && cd build

cmake ..

make


# Run:
./build/src/executable matrices/< mtx file >


# Run unit tests:
./build/tests/TEST_coo

# Repository objective:
- read sparse matrix in .mtx format
- convert sparse matrix into compressed formats (CSR, CSC, COO, BlockedEll)
- extract matrix features
- reorder sparse matrix (RCM, SlashBurn, Metis, BOBA, Nested Dissection)
- output should be reordered matrix in mtx format
