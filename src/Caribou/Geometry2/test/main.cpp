#include <gtest/gtest.h>

#include "Concepts.h"
#include "segments.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
