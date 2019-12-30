#ifndef CARIBOU_GEOMETRY_TRIANGLE_H
#define CARIBOU_GEOMETRY_TRIANGLE_H

#include <Caribou/config.h>
#include <Caribou/Traits.h>
#include <Caribou/Geometry/Interpolation/Triangle.h>
#include <Caribou/Geometry/Internal/BaseTriangle.h>
#include <Eigen/Dense>

namespace caribou::geometry {

template <size_t Dim, typename CanonicalElementType = interpolation::Triangle3>
struct Triangle : public internal::BaseTriangle<Dim, CanonicalElementType, Triangle<Dim, CanonicalElementType>>
{
    static constexpr UNSIGNED_INTEGER_TYPE NumberOfNodesAtCompileTime = CanonicalElementType::NumberOfNodes;
    static constexpr INTEGER_TYPE NumberOfNodes = CanonicalElementType::NumberOfNodes;

    using LocalCoordinates = typename CanonicalElementType::LocalCoordinates;
    using WorldCoordinates = Eigen::Matrix<FLOATING_POINT_TYPE, Dim, 1>;

    template<int nRows, int nColumns, int Options=Eigen::RowMajor>
    using Matrix = Eigen::Matrix<FLOATING_POINT_TYPE, nRows, nColumns, Options>;

    static_assert(Dim == 2 or Dim == 3, "Only 2D and 3D triangles are supported.");

    /*!
     * Construct the triangle from an array of float.
     * The array will be read with a row-major format (ex: [x0 y0 z0 x1 y1 z1 x2 y2 z2])
     */
    constexpr explicit Triangle(const FLOATING_POINT_TYPE data [NumberOfNodesAtCompileTime*Dim]) : p_nodes(data) {}

    template <
            typename ...Nodes,
            REQUIRES(NumberOfNodes == sizeof...(Nodes)+1)
    >
    Triangle(const WorldCoordinates & first_node, Nodes&&...remaining_nodes)
    {
        construct_from_nodes<0>(first_node, std::forward<Nodes>(remaining_nodes)...);
    }

    /**
     * Get the number of nodes of the element
     */
    inline constexpr
    auto
    number_of_nodes() const {
        return NumberOfNodesAtCompileTime;
    }

    inline
    auto
    node(UNSIGNED_INTEGER_TYPE index) const
    {
        return p_nodes.row(index).transpose();
    }

    inline
    auto
    node(UNSIGNED_INTEGER_TYPE index)
    {
        return p_nodes.row(index).transpose();
    }

    /** Get a reference to the set of nodes */
    inline
    const Matrix<NumberOfNodes, Dim> &
    nodes() const
    {
        return p_nodes;
    }

    /** Compute the center position **/
    auto
    center() const noexcept
    {
        return T(LocalCoordinates({1/3., 1/3.}));
    }

    /**
     * Compute the transformation of a local position {u,v} to its world position {x,y,z}
     */
    inline
    WorldCoordinates
    T(const LocalCoordinates & coordinates) const
    {
        return CanonicalElementType::interpolate(coordinates, nodes());
    }

    /** Compute the jacobian matrix evaluated at local position {u,v} */
    Matrix<Dim, 2>
    jacobian (const LocalCoordinates & coordinates) const
    {
        return CanonicalElementType::Jacobian(coordinates, p_nodes);
    }

    /** Compute the surface area **/
    FLOATING_POINT_TYPE
    area() const noexcept
    {
        auto n1 = node(0);
        auto n2 = node(1);
        auto n3 = node(2);

        if constexpr (Dim == 2) {
            Matrix<3, 3> m;
            m << n1[0], n2[0], n3[0],
                n1[1], n2[1], n3[1],
                1. ,   1. ,   1. ;

            return 1 / 2. * std::abs(m.determinant());
        } else {
            auto v1 = n3 - n1;
            auto v2 = n2 - n1;

            return v1.cross(v2).norm() / 2.;
        }
    }

private:
    template <size_t index, typename ...Nodes, REQUIRES(sizeof...(Nodes) >= 1)>
    inline
    void construct_from_nodes(const WorldCoordinates & first_node, Nodes&&...remaining_nodes) {
        p_nodes.row(index) = first_node;
        construct_from_nodes<index+1>(std::forward<Nodes>(remaining_nodes)...);
    }

    template <size_t index>
    inline
    void construct_from_nodes(const WorldCoordinates & last_node) {
        p_nodes.row(index) = last_node;
    }

private:
    Matrix<NumberOfNodes, Dim> p_nodes;
};

} // namespace caribou::geometry
#endif //CARIBOU_GEOMETRY_TRIANGLE_H
