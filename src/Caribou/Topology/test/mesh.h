#ifndef CARIBOU_TOPOLOGY_TEST_MESH_H
#define CARIBOU_TOPOLOGY_TEST_MESH_H

#include <Caribou/Topology/UnstructuredMesh.h>
#include <Caribou/Topology/Domain.h>

#include <Caribou/Geometry/Segment.h>

using namespace caribou::topology;
using namespace caribou::geometry;

TEST(Mesh, UnstructuredGrid) {
    UnstructuredMesh<3> mesh;
    mesh.add_node({0,0,0});
    mesh.add_node({0.5,0.5,0.5});

    auto domain = mesh.add<Domain<Segment<3>>>("segments");

    domain->add({0, 1});
    EXPECT_EQ(domain->number_of_elements(), 1);

    {
        const auto &indices = domain->element_indices(0);
        EXPECT_EQ(indices, Domain<Segment<3>>::Indices({0, 1}));
    }

    EXPECT_ANY_THROW(mesh.add<Domain<Segment<3>>>("segments"));

    {
        std::array<unsigned int, 2> indices = {{1, 0}};
        std::array<UnstructuredMesh<3>::WorldCoordinate, 2> positions = {};
        mesh.positions(indices, positions);
        EXPECT_EQ(mesh.position(indices[0]), positions[0]);
        EXPECT_EQ(mesh.position(indices[1]), positions[1]);
    }

    {
        std::array<UnstructuredMesh<3>::WorldCoordinate, 2> positions = {};
        mesh.positions({1, 0}, positions);
        EXPECT_EQ(mesh.position(1), positions[0]);
        EXPECT_EQ(mesh.position(0), positions[1]);
    }
}

#endif //CARIBOU_TOPOLOGY_TEST_MESH_H
