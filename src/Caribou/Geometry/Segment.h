#ifndef CARIBOU_GEOMETRY_SEGMENT_H
#define CARIBOU_GEOMETRY_SEGMENT_H

#include <Caribou/config.h>
#include <Caribou/Traits.h>
#include <Caribou/Geometry/Interpolation/Segment.h>
#include <Caribou/Geometry/Internal/BaseSegment.h>

#include <Eigen/Core>

namespace caribou::geometry {

template <size_t Dim, typename CanonicalElementType = interpolation::Segment2>
struct Segment : public internal::BaseSegment<Dim, CanonicalElementType, Segment<Dim, CanonicalElementType>>
{
    static constexpr UNSIGNED_INTEGER_TYPE NumberOfNodesAtCompileTime = CanonicalElementType::NumberOfNodes;

    using Base = internal::BaseSegment<Dim, CanonicalElementType, Segment<Dim, CanonicalElementType>>;

    using LocalCoordinates = typename CanonicalElementType::LocalCoordinates;
    using WorldCoordinates = Eigen::Matrix<FLOATING_POINT_TYPE, Dim, 1>;

    template<int nRows, int nColumns, int Options=Eigen::RowMajor>
    using Matrix = Eigen::Matrix<FLOATING_POINT_TYPE, nRows, nColumns, Options>;

    using NodesContainer = typename Base::template NodesContainer<NumberOfNodesAtCompileTime>;

    static_assert(Dim == 1 or Dim == 2 or Dim == 3, "Only 1D, 2D and 3D segments are supported.");

    template <
        typename ...Nodes,
        REQUIRES(NumberOfNodesAtCompileTime == sizeof...(Nodes)+1)
    >
    Segment(const WorldCoordinates & first_node, Nodes&&...remaining_nodes)
    {
        construct_from_nodes<0>(first_node, std::forward<Nodes>(remaining_nodes)...);
    }

    template <
        typename ...Nodes,
        REQUIRES(NumberOfNodesAtCompileTime == sizeof...(Nodes)+1)
    >
    constexpr
    Segment(const FLOATING_POINT_TYPE & first_node, Nodes&&...remaining_nodes)
        : p_nodes {first_node, std::forward<Nodes>(remaining_nodes)...}
    {
        static_assert(Dim == 1 and "Constructor with floating point coordinate is only available for 1D segments.");
    }

    constexpr explicit
    Segment(const FLOATING_POINT_TYPE * data) : p_nodes(data)
    {}

    /**
     * Get the number of nodes of the element
     */
    inline constexpr
    auto
    number_of_nodes() const {
        return NumberOfNodesAtCompileTime;
    }

    /** Get the Node at given index */
    inline
    auto
    node(UNSIGNED_INTEGER_TYPE index) const
    {
        return p_nodes.row(index).transpose();
    }

    /** Get the Node at given index */
    inline
    auto
    node(UNSIGNED_INTEGER_TYPE index)
    {
        return p_nodes.row(index).transpose();
    }

    /** Get a reference to the set of nodes */
    inline
    const auto &
    nodes() const
    {
        return p_nodes;
    }

    /** Compute the center position **/
    inline
    WorldCoordinates
    center() const noexcept
    {
        return T(LocalCoordinates(0.));
    }

    /**
     * Compute the transformation of a local position {u} to its world position {x,y,z}
     */
    inline
    WorldCoordinates
    T(const LocalCoordinates & coordinates) const
    {
        return CanonicalElementType::interpolate(coordinates, nodes());
    }

    /** Compute the jacobian matrix evaluated at local position {u,v} */
    template <typename LocalCoordinates>
    Matrix<Dim, 1>
    jacobian (LocalCoordinates && coordinates) const
    {
        return CanonicalElementType::Jacobian(std::forward<LocalCoordinates>(coordinates), p_nodes);
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
    NodesContainer p_nodes;
};

} // namespace caribou::geometry

#endif //CARIBOU_GEOMETRY_SEGMENT_H
