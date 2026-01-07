#include <iostream>
#include <vector>
#include <algorithm>

extern "C" 
{
#include "mmio.h"
}

struct matrixStructure 
{
    int num_rows;
    int num_cols;
    int nz;
};

struct COO
{
    int row;
    int col;
    double val;
};

struct CSR 
{
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
    std::vector<double> values;
};

COO readMtxLine(FILE* f, MM_typecode matcode)
{
    int row, col;
    double val;
    if (mm_is_pattern(matcode)) {
        if (fscanf(f, "%d %d", &row, &col) != 2) {
            std::cerr << "Error reading MTX entry at index\n";
            exit(1);
        }
        val = 1;
    } 
    else if (mm_is_complex(matcode)) {
        // TO BE IMPLEMENTED
        std::cerr << "Matrix with complex numbers to be implemented...\n";
        exit(1);
    }
    else {
        if (fscanf(f, "%d %d %lf", &row, &col, &val) != 3) {
            std::cerr << "Error reading MTX entry at index\n";
            exit(1);
        }
    }
    return COO{row - 1, col - 1, val};
}

void readMtxCoordinates(FILE* f, MM_typecode matcode, std::vector<COO>& entries, int nz)
{
    for (int i = 0; i < nz; i++) {
        COO coordinate = readMtxLine(f, matcode);
        entries.push_back(coordinate);

        if (mm_is_symmetric(matcode) && coordinate.row != coordinate.col){
            entries.push_back({coordinate.col, coordinate.row, coordinate.val});
        }
    }
}

void printCOO(const std::vector<COO>& entries)
{
    for (size_t i = 0; i < entries.size(); ++i) {
        std::cout << "Entry " << i << ": "
                  << entries[i].row << " "
                  << entries[i].col << " "
                  << entries[i].val
                  << "\n";
    }
}

void sortCOO(std::vector<COO>& entries)
{
    std::sort(entries.begin(), entries.end(),
        [](const COO& a, const COO& b) {
            if (a.row != b.row)
                return a.row < b.row;
            return a.col < b.col;
        });
}

void convertCOOToCSR(const std::vector<COO>& entries, std::vector<CSR>& csr) 
{

}

int main(int argc, char* argv[]) 
{
    // TODO:
    // confirm if its being properly read
    // implement reading of complex values
    // Find best structure for CSR
    // convert COO to CSR

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <mtx file>\n";
        return 1;
    }

    // Open mtx file
    FILE *f;
    if ((f = fopen(argv[1], "r")) == NULL)
    {
        std::cerr << "Could not locate the matrix file. Please make sure the pathname is valid.\n";
        exit(1);
    }

    // Read mtx header
    MM_typecode matcode;
    if (mm_read_banner(f, &matcode))
    {
        std::cerr << "Could not process Matrix Market banner.\n";
        exit(1);
    }

    if (!mm_is_matrix(matcode) || !mm_is_coordinate(matcode)) {
        std::cerr << "Input must be a sparse Matrix Market matrix (coordinate format).\n";
        exit(1);
    }

    // Read mtx dimensions
    matrixStructure mtx;
    if (mm_read_mtx_crd_size(f, &mtx.num_rows, &mtx.num_cols, &mtx.nz))
    {
        std::cerr << "Could not read matrix dimensions.\n";
        exit(1);
    }

    std::vector<COO> entries;
    readMtxCoordinates(f, matcode, entries, mtx.nz);
    sortCOO(entries);

    std::vector<CSR> csr;
    convertCOOToCSR(entries, csr);

    std::cout << "mtx rows: " << mtx.num_rows << "\n";
    std::cout << "mtx cols: " << mtx.num_cols << "\n";
    std::cout << "mtx nz: " << mtx.nz << "\n";
    printCOO(entries);

    // build CSR

    return 0;
}
