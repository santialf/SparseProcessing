#include <iostream>

extern "C" {
#include "mmio.h"
}

int main(int argc, char* argv[]) {

    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int A_num_rows, A_num_cols, nz;
    int *I_complete, *J_complete;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " matrix.zip\n";
        return 1;
    }

    if ((f = fopen(argv[1], "r")) == NULL)
    {
        printf("Could not locate the matrix file. Please make sure the pathname is valid.\n");
        exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    if ((ret_code = mm_read_mtx_crd_size(f, &A_num_rows, &A_num_cols, &nz)) != 0)
    {
        printf("Could not read matrix dimensions.\n");
        exit(1);
    }

    return 0;
}
