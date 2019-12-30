#ifndef CARIBOU_TOPOLOGY_TEST_MESH_H
#define CARIBOU_TOPOLOGY_TEST_MESH_H

#include <Caribou/Topology/UnstructuredMesh.h>
#include <Caribou/Topology/Domain.h>
#include <Caribou/Geometry/Segment.h>
#include <Caribou/Geometry/Triangle.h>
#include <Caribou/Geometry/Tetrahedron.h>

#include <Eigen/Core>

using namespace caribou::topology;
using namespace caribou::geometry;

template <class Derived>
struct is_eigen : public std::is_base_of<Eigen::DenseBase<Derived>, Derived> {
};
template <class Derived,
        class = typename std::enable_if<is_eigen<Derived>::value>::type>
::std::ostream &operator<<(::std::ostream &o, const Derived &m) {
    o << "\n" << static_cast<const Eigen::DenseBase<Derived> &>(m);
    return o;
}

TEST(UnstructuredGrid, Mesh) {
    UnstructuredMesh<3> mesh;
    mesh.add_node({0,0,0});
    mesh.add_node({1,1,1});

    {
        std::array<unsigned int, 2> indices = {{1, 0}};
        std::array<UnstructuredMesh<3>::WorldCoordinate, 2> positions = {};
        mesh.get_positions(indices, positions);
        EXPECT_EQ(mesh.position(indices[0]), positions[0]);
        EXPECT_EQ(mesh.position(indices[1]), positions[1]);
    }

    {
        std::array<UnstructuredMesh<3>::WorldCoordinate, 2> positions = {};
        mesh.get_positions({1, 0}, positions);
        EXPECT_EQ(mesh.position(1), positions[0]);
        EXPECT_EQ(mesh.position(0), positions[1]);
    }
}

TEST(UnstructuredGrid, Segment) {
    UnstructuredMesh<3> mesh;
    mesh.add_node({0,0,0});
    mesh.add_node({1,1,1});

    auto domain = mesh.add_domain<Segment<3>>("segments");

    domain->add({0, 1});
    EXPECT_EQ(domain->number_of_elements(), 1);

    {
        const auto &indices = domain->element_indices(0);
        EXPECT_EQ(indices, UnstructuredMesh<3>::Domain<Segment<3>>::Indices({0, 1}));
    }

    EXPECT_ANY_THROW(mesh.add_domain<Segment<3>>("segments"));

    {
        const Segment<3> segment = domain->element(0);
        EXPECT_EQ(segment.center(), Segment<3>::WorldCoordinates(0.5, 0.5, 0.5));
    }
}

TEST(UnstructuredGrid, Triangle) {
    UnstructuredMesh<3> mesh;
    mesh.add_node({50,50,33});
    mesh.add_node({60, 50, 21});
    mesh.add_node({55, 55, -4});

    auto domain = mesh.add_domain<Triangle<3>>("triangles");

    domain->add({2, 1, 0});
    EXPECT_EQ(domain->number_of_elements(), 1);

    {
        const auto &indices = domain->element_indices(0);
        EXPECT_EQ(indices, UnstructuredMesh<3>::Domain<Triangle<3>>::Indices({2, 1, 0}));
    }

    EXPECT_ANY_THROW(mesh.add_domain<Triangle<3>>("triangles"));


    {
        const Triangle<3> triangle = domain->element(0);
        EXPECT_EQ(triangle.node(0), Triangle<3>::WorldCoordinates(55, 55, -4));
    }
}

TEST(UnstructuredGrid, Tetrahedron) {
    UnstructuredMesh<3> mesh;
    mesh.add_node({50,50,33});
    mesh.add_node({60, 50, 21});
    mesh.add_node({55, 55, -4});
    mesh.add_node({55, 55, -40});

    auto domain = mesh.add_domain<Tetrahedron<interpolation::Tetrahedron4>>("tetras");

    domain->add({3, 2, 1, 0});
    EXPECT_EQ(domain->number_of_elements(), 1);

    {
        const auto &indices = domain->element_indices(0);
        EXPECT_EQ(indices, UnstructuredMesh<3>::Domain<Tetrahedron<interpolation::Tetrahedron4>>::Indices({3, 2, 1, 0}));
    }

    EXPECT_ANY_THROW(mesh.add_domain<Tetrahedron<interpolation::Tetrahedron4>>("tetras"));


    {
        const auto tetra = domain->element(0);
        EXPECT_EQ(tetra.node(0), Tetrahedron<interpolation::Tetrahedron4>::WorldCoordinates(55, 55, -40));
    }
}
#endif //CARIBOU_TOPOLOGY_TEST_MESH_H