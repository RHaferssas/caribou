#ifndef CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H
#define CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H

#include <Caribou/config.h>
#include <Caribou/Topology/Mesh.h>

#include <unordered_map>
#include <string>
#include <memory>

namespace caribou::topology {

    template <UNSIGNED_INTEGER_TYPE Dimension>
    class UnstructuredMesh : public Mesh<Dimension> {
    public:

        /*!
         * \copydoc caribou::topology::BaseMesh::number_of_domains
         */
        [[nodiscard]]
        inline auto number_of_domains() const -> UNSIGNED_INTEGER_TYPE final {
            return p_domains.size();
        }

        /*!
         * \copydoc caribou::topology::BaseMesh::add_domain
         * @throws std::domain_error when a domain is already assigned to this domain name
         */
        inline void add_domain(std::string name, std::unique_ptr<BaseDomain> domain) final;

        /*!
         * \copydoc caribou::topology::Mesh::position
         */
        [[nodiscard]]
        inline auto position(UNSIGNED_INTEGER_TYPE index) const -> Eigen::Matrix<FLOATING_POINT_TYPE, Dimension, 1> final;

        /*!
         * Get the number of nodes of the mesh.
         */
        [[nodiscard]] inline auto number_of_nodes() const -> UNSIGNED_INTEGER_TYPE final {return p_positions.rows();};

        /*!
         * Get a reference to the position vector.
         */
        inline auto positions() const -> const Eigen::Matrix<FLOATING_POINT_TYPE, Eigen::Dynamic, Dimension, Eigen::RowMajor> & {
            return p_positions;
        };

    private:
        std::unordered_map<std::string, std::unique_ptr<BaseDomain>> p_domains;
        Eigen::Matrix<FLOATING_POINT_TYPE, Eigen::Dynamic, Dimension, Eigen::RowMajor> p_positions;
    };

    extern template class UnstructuredMesh<1>;
}

#endif //CARIBOU_TOPOLOGY_UNSTRUCTUREDMESH_H
