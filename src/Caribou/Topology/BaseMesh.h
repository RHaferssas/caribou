#ifndef CARIBOU_TOPOLOGY_BASEMESH_H
#define CARIBOU_TOPOLOGY_BASEMESH_H

#include <Caribou/config.h>

#include <string>
#include <memory>

namespace caribou::topology {

    struct BaseDomain;

    /**
     * A mesh is a discrete view of a space by the mean of nodes. It can contain one or many domains, which are
     * responsible for maintaining the topological relation between the nodes of a subspace of the mesh.
     */
    struct BaseMesh {
        /*!
         * Get the dimension of the mesh, ie, the number of coordinates of a point.
         */
        [[nodiscard]] virtual inline auto dimension() const -> UNSIGNED_INTEGER_TYPE = 0;

        /*!
         * Get the number of nodes of the mesh.
         */
        [[nodiscard]] virtual inline auto number_of_nodes() const -> UNSIGNED_INTEGER_TYPE = 0;

        /*!
         * Get the number of domains of the mesh.
         */
        [[nodiscard]] virtual inline auto number_of_domains() const -> UNSIGNED_INTEGER_TYPE = 0;

        /*!
         * Adds a domain to the mesh.
         */
        template<typename Domain, REQUIRES(std::is_base_of_v<BaseDomain, Domain>)>
        inline auto add(const std::string & name) -> Domain * {
            auto domain_ptr = new Domain(static_cast<const BaseMesh *>(this));
            this->do_add_domain(name, std::unique_ptr<BaseDomain>(dynamic_cast<BaseDomain *>(domain_ptr)));
            return dynamic_cast<Domain*>(domain_ptr);
        }

    private:
        /*!
         * Adds a domain to the mesh.
         */
        virtual auto do_add_domain(std::string name, std::unique_ptr<BaseDomain> domain) -> BaseDomain * = 0;
    };
}

#endif //CARIBOU_TOPOLOGY_BASEMESH_H
