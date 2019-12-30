#include <gtest/gtest.h>
#include <Caribou/Topology/test/grid.h>
#include <Caribou/Topology/test/test_unstructured_grid.h>

#ifdef CARIBOU_WITH_VTK
#include <Caribou/Topology/test/test_vtkreader.h>
#endif


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
