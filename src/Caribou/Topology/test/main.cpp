#include <gtest/gtest.h>
#include <Caribou/Topology/test/grid.h>
#include <Caribou/Topology/test/mesh.h>


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
