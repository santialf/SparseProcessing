include(FetchContent)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

add_executable(TEST_coo
    testCoo.cpp
)

add_executable(TEST_bell
    testBell.cpp
)

add_executable(TEST_csr
    testCsr.cpp
)

add_executable(TEST_csc
    testCsc.cpp
)

target_link_libraries(TEST_coo
    PRIVATE
        my_lib
        GTest::gtest
        GTest::gtest_main
)

target_link_libraries(TEST_bell
    PRIVATE
        my_lib
        GTest::gtest
        GTest::gtest_main
)

target_link_libraries(TEST_csr
    PRIVATE
        my_lib
        GTest::gtest
        GTest::gtest_main
)

target_link_libraries(TEST_csc
    PRIVATE
        my_lib
        GTest::gtest
        GTest::gtest_main
)