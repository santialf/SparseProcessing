project(executable LANGUAGES C CXX)

add_library(my_lib
    coo.cpp
    csr.cpp
    convertFormats.cpp
    mtxReader.cpp
    mmio.c
)

target_include_directories(my_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(executable
    main.cpp
)

target_link_libraries(executable
    PRIVATE
        my_lib
)