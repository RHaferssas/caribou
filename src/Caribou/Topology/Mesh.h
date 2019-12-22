#ifndef CARIBOU_TOPOLOGY_MESH_H
#define CARIBOU_TOPOLOGY_MESH_H

#include <Caribou/config.h>
#include <Caribou/Topology/BaseMesh.h>

#include <array>

namespace caribou::topology {

    struct BaseDomain;

    template <UNSIGNED_INTEGER_TYPE Dimension>
    class Mesh : public BaseMesh {
    public:

        using WorldCoordinate = std::array<FLOATING_POINT_TYPE, Dimension>;

        /*!
        * Get the position coordinates of a node from its index.
        */
        [[nodiscard]] virtual inline auto position(UNSIGNED_INTEGER_TYPE index) const -> WorldCoordinate = 0;

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
