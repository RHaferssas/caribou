#ifndef CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H
#define CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H

#include <Caribou/config.h>
#include <Caribou/Traits.h>
#include <Caribou/Topology/Mesh.h>

#include <vector>
#include <string>
#include <algorithm>

namespace caribou::topology {


    template <UNSIGNED_INTEGER_TYPE _Dimension>
    class UnstructuredMesh : public Mesh<UnstructuredMesh<_Dimension>> {
    public:
        friend class Mesh<UnstructuredMesh<_Dimension>>;

        using Parent = Mesh<UnstructuredMesh<_Dimension>>;

        template<typename Element>
        using Domain = Domain<UnstructuredMesh<_Dimension>, Element>;

        static constexpr UNSIGNED_INTEGER_TYPE Dimension = _Dimension;
        using WorldCoordinate = std::array<FLOATING_POINT_TYPE, Dimension>;

        /*!
         * Default constructor.
         * Initializes an empty unstructured mesh.
         */
        UnstructuredMesh() = default;

        /**
         * Construct the unstructured mesh with a set of point positions from a position vector
         * @param positions
         */
        template<class T>
        explicit UnstructuredMesh(const std::vector<WorldCoordinate> & positions) : p_positions(positions) {}

        /*!
         * Get the number of nodes of the mesh.
         */
        [[nodiscard]] inline auto number_of_nodes() const -> UNSIGNED_INTEGER_TYPE final {return p_positions.size();};

        /*!
         * Adds a node to the mesh.
         * @param position The position coordinates of the node.
         */
        inline void add_node(const WorldCoordinate position) {
            p_positions.push_back(position);
        }

    private:
        /*!
         * \copydoc caribou::topology::Mesh::position
         */
        [[nodiscard]]
        inline auto _position(UNSIGNED_INTEGER_TYPE index) const -> WorldCoordinate {
            return p_positions[index];
        }

        /*!
         * \copydoc caribou::topology::Mesh::positions(IndicesRange && indices, PositionsRange && positions)
         */
        template <typename IndicesRange, typename PositionsRange>
        inline void _positions(const IndicesRange & indices, PositionsRange & positions) const {
            std::transform(indices.cbegin(), indices.cend(), positions.begin(), [this](const auto & index) {
                return this->p_positions[index];
            });
        }

        /*!
         * \copydoc caribou::topology::Mesh::positions(std::initializer_list<IntegerType> && indices, PositionsRange && positions)
         */
        template <typename IntegerType, typename PositionsRange>
        inline void _positions(const std::initializer_list<IntegerType> & indices, PositionsRange & positions) const {
            std::transform(indices.begin(), indices.end(), positions.begin(), [this](const auto & index) {
                return this->p_positions[index];
            });
        }

    private:
        std::vector<WorldCoordinate> p_positions;
    };

    extern template class UnstructuredMesh<1>;
    extern template class UnstructuredMesh<2>;
    extern template class UnstructuredMesh<3>;
}

#endif //CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H
