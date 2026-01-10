project(executable LANGUAGES C CXX)

add_library(my_lib
    coo.cpp
    csr.cpp
    mtxReader.cpp
    mmio.c
)

add_executable(executable
    main.cpp
)

target_link_libraries(executable
    my_lib
)