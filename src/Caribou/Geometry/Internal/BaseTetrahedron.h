#ifndef CARIBOU_GEOMETRY_INTERNAL_BASETETRAHEDRON_H
#define CARIBOU_GEOMETRY_INTERNAL_BASETETRAHEDRON_H

#include <Caribou/config.h>

namespace caribou::geometry::internal {

template<int NumberOfNodes, typename CanonicalElementType, typename TetrahedronType>
struct BaseTetrahedron : public CanonicalElementType
{
    using CanonicalElement = CanonicalElementType;
    using LocalCoordinates = typename CanonicalElement::LocalCoordinates;
    using WorldCoordinates = Eigen::Matrix<FLOATING_POINT_TYPE, 3, 1>;

    template<int nRows, int nColumns, int Options=Eigen::RowMajor>
    using Matrix = Eigen::Matrix<FLOATING_POINT_TYPE, nRows, nColumns, Options>;

    template<int nRows, int nColumns>
    using Map = Eigen::Map<const Matrix<nRows, nColumns>>;

    template<int nRows, int Options=0>
    using Vector = Eigen::Matrix<FLOATING_POINT_TYPE, nRows, 1, Options>;

    template<int nRows>
    using MapVector = Eigen::Map<const Vector<nRows>>;


    BaseTetrahedron()
        : p_nodes(Map<NumberOfNodes, 3>(&CanonicalElementType::nodes[0][0]))
    {}

    /*!
     * Construct the element from an array of float.
     * The array will be read with a row-major format (ex: [x0 y0 z0 x1 y1 z1 x2 y2 z2 ...])
     */
    constexpr explicit BaseTetrahedron(const FLOATING_POINT_TYPE data [NumberOfNodes*3]) : p_nodes(data) {}

    template <
        typename ...Nodes,
        REQUIRES(NumberOfNodes == sizeof...(Nodes)+1)
    >
    BaseTetrahedron(const WorldCoordinates & first_node, Nodes&&...remaining_nodes)
    {
        construct_from_nodes<0>(first_node, std::forward<Nodes>(remaining_nodes)...);
    }

    BaseTetrahedron(const Matrix<NumberOfNodes, 3, Eigen::RowMajor> & m)
        : p_nodes(m)
    {}

    /**
     * Get the number of nodes of the element
     */
    inline constexpr
    auto
    number_of_nodes() const {
        return NumberOfNodes;
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

    /** Compute the jacobian matrix evaluated at local position {u,v,w}
    * (see interpolation::CanonicalElement::Jacobian for more details).
    * */
    inline
    auto
    jacobian (const LocalCoordinates & coordinates) const
    {
        return CanonicalElementType::Jacobian(coordinates, self().nodes());
    }

    /**
     * Compute the transformation of a local position {u,v,w} to its world position {x,y,z}
     */
    inline
    WorldCoordinates
    T(const LocalCoordinates & coordinates) const
    {
        return CanonicalElementType::interpolate(coordinates, self().nodes());
    }

    /**
     * Get the ith triangle face.
     */
    inline
    auto
    face(UNSIGNED_INTEGER_TYPE index) const
    {
        using FaceType = typename TetrahedronType::FaceType;
        const auto & face_indices = CanonicalElementType::faces[index];

        Matrix<FaceType::NumberOfNodes, 3, Eigen::RowMajor> m;
        for (std::size_t i = 0; i < FaceType::NumberOfNodes; ++i)
            m.row(i) = node(face_indices[i]);

        return FaceType(m);
    }

    /** Get the center position */
    inline
    WorldCoordinates
    center() const
    {
        return T(LocalCoordinates(1/4., 1/4., 1/4.));
    }

    /**
     * Extract the frame positioned at the first node of the tetrahedron by computing the cross product of the unit
     * vectors of its adjacent edges.
     *
     * This function will return a matrix of the form:
     * | ux vx wx |
     * | uy vy wy |
     * | uz vz wz |
     *
     * Where [ux uy uz], [vx, vy, vz] and [wx, wy, wz] are orthogonal unitary vectors representing the u, v and w frame
     * in the current tetrahedron. If the tetrahedron is regular and not rotated, this matrix is the Identity matrix.
     * If it is regular but rotated, rotating the tetrahedron by the transposed of this frame should align the u,v,w
     * axis to the x,y,z world frame (identity matrix).
     */
    inline
    Matrix<3, 3>
    frame() const
    {
        // u-axis
        const auto u = (node(1) - node(0)).normalized();

        // v-axis
        auto v = (node(2) - node(0)).normalized();

        // w-axis
        const WorldCoordinates w = u.cross(v).normalized();

        // v-axis (recompute the v-axis in case u and v aren't orthogonal
        v = w.cross(u).normalized();

        Matrix<3, 3> m;
        m << u, v, w;

        return m;
    }

    /**
     * Compute an integral approximation by gauss quadrature on the hexahedron of the given evaluation function.
     *
     * @example
     * \code{.cpp}
     * // Integrate the polynomial 1 + 2x + 2xy + 3*z on an hexahedron.
     * float result = Hexahedron(x1, x2, x3, x4, x5, x6, x7, x8).gauss_integrate(
     *   [] (const Hexahedron & hexa, const Hexahedron::LocalCoordinates & coordinates) -> float {
     *     const auto & xi   = coordinates[0];
     *     const auto & eta  = coordinates[1];
     *     const auto & zeta = coordinates[2];
     *     return 1 + 2*xi + 2*xi*eta + 3*zeta;
     *   }
     * );
     * \endcode
     *
     * @tparam EvaluateFunctionType Callback function reference type. See f parameter.
     *
     * @param f
     * Callback function of the signature
     *
     *     ValueType f (const Hexahedron & hexa, const LocalCoordinates & coordinates);
     *
     * Where hexa is a reference to the current hexahadron on which we integrate, and the coordinates u, v and w
     * forms the local position of a sample point on which we want to get the evaluation value.
     *
     * @return The value of the integral computed on this hexahedron.
     *
     */
    template <typename ValueType, typename EvaluateFunctor>
    inline
    ValueType
    gauss_quadrature(EvaluateFunctor f) const
    {
        const auto p0 = MapVector<3>(CanonicalElementType::gauss_nodes[0]);
        const auto w0 = CanonicalElementType::gauss_weights[0];
        const auto detJ0 = jacobian(p0).determinant();
        const auto eval0 = f(self(), p0);
        ValueType result = eval0 * w0 * detJ0;

        for (std::size_t i = 1; i < CanonicalElementType::number_of_gauss_nodes; ++i) {
            const auto p = MapVector<3>(CanonicalElementType::gauss_nodes[i]);
            const auto w = CanonicalElementType::gauss_weights[i];
            const auto detJ = jacobian(p).determinant();
            const auto eval = f(self(), p);
            result += eval * w * detJ;
        }

        return result;
    }

private:
    const TetrahedronType & self() const {
        return (static_cast<const TetrahedronType &> (*this));
    }
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

protected:
    Matrix<NumberOfNodes, 3> p_nodes;
};

} // namespace caribou::geometry::internal

#endif //CARIBOU_GEOMETRY_INTERNAL_BASETETRAHEDRON_H
