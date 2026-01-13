# SparseProcessing


# Compile:
mkdir build && cd build

cmake ..

make


# Run:
./build/src/executable <mtx file>

# Repository objective:
- read sparse matrix in .mtx format
- convert sparse matrix into compressed formats (CSR, CSC, COO, BlockedEll)
- reorder sparse matrix (RCM, SlashBurn, Metis, BOBA, Nested Dissection)
- output should be reordered matrix in mtx format
