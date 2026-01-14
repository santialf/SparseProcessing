#include <gtest/gtest.h>
#include "coo.hpp"
#include "mtxReader.hpp"

TEST(BasicTest, OneEqualsOne)
{
    EXPECT_EQ(1, 1);
}


// tests where everything is passing
// tests with edge cases
// testing indexation
// testing correct values
// testing robustness (diff ordering, extra)