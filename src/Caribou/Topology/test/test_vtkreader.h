#ifndef CARIBOU_TOPOLOGY_TEST_VTKREADER_H
#define CARIBOU_TOPOLOGY_TEST_VTKREADER_H

#include <Caribou/Topology/IO/VTKReader.h>

using namespace caribou::topology;

TEST(VTKReader, Read) {
    auto reader = io::VTKReader<3>::Read ("/home/Documents/Doctorat/scenes/IBM_analysis/nl_cylinder.vtk");
}

#endif //CARIBOU_TOPOLOGY_TEST_VTKREADER_H
