#ifndef CARIBOU_TOPOLOGY_BASEDOMAIN_H
#define CARIBOU_TOPOLOGY_BASEDOMAIN_H

#include <Caribou/config.h>

namespace caribou::topology {

    struct BaseMesh;

    /**
     * A domain is subspace of a mesh containing a set of points and the topological relation between them.
     *
     * In a domain, all the elements are of the same type. For example, a domain can not contain both hexahedrons and
     * tetrahedrons.
     */
    struct BaseDomain {
        friend struct BaseMesh;

        /**
         * Get the canonical dimension of the element contained in this domain, ie, the number of coordinates of a point
         * relative to the element basis. For example, a triangle has a canonical dimension of 2, even if it is inside
         * a 3 dimensions mesh : a point inside the triangle has coordinates (x,y,z) relative to the mesh's basis, and
         * coordinates (u, v) relative to the triangle's first node.
         */
        [[nodiscard]] virtual auto canonical_dimension() const -> UNSIGNED_INTEGER_TYPE= 0;

        /**
         * Get the number of elements contained in the domain.
         */
        [[nodiscard]] virtual auto number_of_elements() const -> UNSIGNED_INTEGER_TYPE= 0;

        /**
         * Get the mesh associated with this domain.
         */
        inline auto mesh() const -> const BaseMesh & {
            return *p_mesh;
        }

    private:
        explicit BaseDomain(const BaseMesh * mesh) : p_mesh(mesh) {
            if (!mesh) {
                throw std::runtime_error("Trying to instantiate a domain without mesh.");
            }
        }

        const BaseMesh * p_mesh;
    };
}

#endif //CARIBOU_TOPOLOGY_BASEDOMAIN_H
