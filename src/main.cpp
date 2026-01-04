#include <iostream>
#include <vector>

extern "C" {
#include "mmio.h"
}

struct matrixStructure {
    int num_rows;
    int num_cols;
    int nz;
};

struct COO {
    int row;
    int col;
    double val;
};

COO readMtxLine(FILE* f, MM_typecode matcode){
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
    }
    else {
        if (fscanf(f, "%d %d %lf", &row, &col, &val) != 3) {
            std::cerr << "Error reading MTX entry at index\n";
            exit(1);
        }
    }
    return COO{row - 1, col - 1, val};
}

void readMtxCoordinates(FILE* f, MM_typecode matcode, std::vector<COO>& entries, int nz){
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
                  << "row = " << entries[i].row
                  << ", col = " << entries[i].col
                  << ", val = " << entries[i].val
                  << "\n";
    }
}

int main(int argc, char* argv[]) {

    // TODO:
    // properly read matrix information into structure
    // fill-in missing information
    // if not matrix and not sparse, give error
    // confirm if its being properly printed
    // implement reading of complex values

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <mtx file>\n";
        return 1;
    }

    // Open mtx file
    FILE *f;
    if ((f = fopen(argv[1], "r")) == NULL)
    {
        printf("Could not locate the matrix file. Please make sure the pathname is valid.\n");
        exit(1);
    }

    // Read mtx header
    MM_typecode matcode;
    if (mm_read_banner(f, &matcode))
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    // Read mtx dimensions
    matrixStructure mtx;
    if (mm_read_mtx_crd_size(f, &mtx.num_rows, &mtx.num_cols, &mtx.nz))
    {
        printf("Could not read matrix dimensions.\n");
        exit(1);
    }

    std::vector<COO> entries;
    readMtxCoordinates(f, matcode, entries, mtx.nz);

    std::cout << "mtx rows: " << mtx.num_rows << "\n";
    std::cout << "mtx cols: " << mtx.num_cols << "\n";
    std::cout << "mtx nz: " << mtx.nz << "\n";
    printCOO(entries);

    return 0;
}
