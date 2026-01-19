project(executable LANGUAGES C CXX)

add_library(my_lib INTERFACE)

target_include_directories(my_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(my_lib
    INTERFACE _HEADER_ONLY
)

add_executable(executable
    main.cpp
)

target_link_libraries(executable
    PRIVATE
        my_lib
)