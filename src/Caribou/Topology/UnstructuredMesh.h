#ifndef CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H
#define CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H

#include <Caribou/config.h>
#include <Caribou/Traits.h>
#include <Caribou/Topology/Mesh.h>

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

namespace caribou::topology {


    template <UNSIGNED_INTEGER_TYPE Dimension>
    class UnstructuredMesh : public Mesh<Dimension> {
    public:

        using WorldCoordinate = typename Mesh<Dimension>::WorldCoordinate;

        /*!
         * Default constructor.
         * Initializes an empty unstructured mesh.
         */
        UnstructuredMesh() {};

        /**
         * Construct the unstructured mesh with a set of point positions from a position vector
         * @param positions
         */
        template<class T>
        UnstructuredMesh(const std::vector<WorldCoordinate> & positions) : p_positions(positions) {}


        /*!
         * \copydoc caribou::topology::BaseMesh::number_of_domains
         */
        [[nodiscard]]
        inline auto number_of_domains() const -> UNSIGNED_INTEGER_TYPE final {
            return p_domains.size();
        }

        /*!
         * \copydoc caribou::topology::Mesh::position
         */
        [[nodiscard]]
        inline auto position(UNSIGNED_INTEGER_TYPE index) const -> WorldCoordinate final {
            return p_positions[index];
        }

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

        /*!
         * Get a reference to the position vector.
         */
        inline auto positions() const -> const std::vector<WorldCoordinate> & {
            return p_positions;
        };

        /*!
         * Fills in a position vector from the given indices vector.
         * @tparam IndicesRange A const iterable range-based object of int (must have the cbegin() and cend() members)
         * @tparam PositionsRange  An iterable range-based object of WorldCoordinate (must have the begin() and end() members)
         * @param indices [IN] The indices from which the positions vector will be filled
         * @param positions [OUT] The position coordinates of every nodes queried.
         *
         * Example:
         * \code{.cpp}
         * std::array<unsigned int, 4> indices = {{0, 4, 12, 42}};
         * std::array<UnstructuredMesh<3>::WorldCoordinate, 4> positions;
         * mesh.positions(indices, positions);
         *
         * std::cout << "Position of node # << 12 << " is " << positions[2] << std::endl;
         * \endcode
         *
         */
        template <typename IndicesRange, typename PositionsRange>
        inline void positions(const IndicesRange & indices, PositionsRange & positions) const {
            std::transform(indices.cbegin(), indices.cend(), positions.begin(), [this](const auto & index) {
                return this->p_positions[index];
            });
        }

        /*!
         * Fills in a position vector from the given indices vector.
         * @tparam IntegerType Integer type of the indices passed
         * @tparam PositionsRange  An iterable range-based object of WorldCoordinate (must have the begin() and end() members)
         * @param indices [IN] The indices from which the positions vector will be filled
         * @param positions [OUT] The position coordinates of every nodes queried.
         *
         * Example:
         * \code{.cpp}
         * std::array<UnstructuredMesh<3>::WorldCoordinate, 4> positions;
         * mesh.positions({0,4,12,43}, positions);
         *
         * std::cout << "Position of node # << 12 << " is " << positions[2] << std::endl;
         * \endcode
         *
         */
        template <typename IntegerType, typename PositionsRange>
        inline void positions(const std::initializer_list<IntegerType> & indices, PositionsRange & positions) const {
            std::transform(indices.begin(), indices.end(), positions.begin(), [this](const auto & index) {
                return this->p_positions[index];
            });
        }

    private:
        /*!
         * \copydoc caribou::topology::BaseMesh::add_domain
         * @throws std::domain_error when a domain is already assigned to this domain name
         */
        BaseDomain * do_add_domain(std::string name, std::unique_ptr<BaseDomain> domain) final;

        std::unordered_map<std::string, std::unique_ptr<BaseDomain>> p_domains;
        std::vector<WorldCoordinate> p_positions;
    };

    extern template class UnstructuredMesh<1>;
    extern template class UnstructuredMesh<2>;
    extern template class UnstructuredMesh<3>;
}

#endif //CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H
