#ifndef CARIBOU_TOPOLOGY_MESH_H
#define CARIBOU_TOPOLOGY_MESH_H

#include <Caribou/config.h>
#include <Caribou/Topology/BaseMesh.h>

#include <Eigen/Core>

namespace caribou::topology {

    struct BaseDomain;

    template <UNSIGNED_INTEGER_TYPE Dimension>
    class Mesh : public BaseMesh {
    public:
        /*!
         * Mesh should not be instanciated directly.
         */
        Mesh() = delete;

        /*!
        * Get the position coordinates of a node from its index.
        */
        [[nodiscard]] virtual inline auto position(UNSIGNED_INTEGER_TYPE index) const -> Eigen::Matrix<FLOATING_POINT_TYPE, Dimension, 1> = 0;

        /*!
         * \copydoc caribou::topology::BaseMesh::dimension
         */
        [[nodiscard]]
        inline auto dimension() const -> UNSIGNED_INTEGER_TYPE final {
            return Dimension;
        };
    };
}

#endif //CARIBOU_TOPOLOGY_MESH_H
