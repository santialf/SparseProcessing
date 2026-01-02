#include <iostream>

extern "C" {
#include "mmio.h"
}

int main(int argc, char* argv[]) {

    // TODO:
    // create structure to store matrix information
    // properly read matrix information into structure
    // fill-in missing information

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " matrix.mtx\n";
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
    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    // Read mtx dimensions
    int ret_code, A_num_rows, A_num_cols, nz;
    if ((ret_code = mm_read_mtx_crd_size(f, &A_num_rows, &A_num_cols, &nz)) != 0)
    {
        printf("Could not read matrix dimensions.\n");
        exit(1);
    }

    return 0;
}
