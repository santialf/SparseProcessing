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

void readMtxCoordinates(FILE* f, std::vector<COO>& entries, const int &nz){
    int row, col;
    double val;
    for (int i = 0; i < nz; i++) {
        if (fscanf(f, "%d %d %lf", &row, &col, &val) != 3) {
            std::cerr << "Error reading MTX entry at index " << i << "\n";
            exit(1);
        }

        // Convert from 1-based to 0-based indexing
        entries.push_back({row - 1, col - 1, val});
    }
}

int main(int argc, char* argv[]) {

    // TODO:
    // create structure to store matrix information
    // properly read matrix information into structure
    // fill-in missing information

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
    readMtxCoordinates(f, entries, mtx.nz);
    

    std::cout << "mtx rows: " << mtx.num_rows << "\n";
    std::cout << "mtx cols: " << mtx.num_cols << "\n";
    std::cout << "mtx nz: " << mtx.nz << "\n";

    return 0;
}
