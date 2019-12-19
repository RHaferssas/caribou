#include <Caribou/Topology/UnstructuredMesh.h>
#include <Caribou/Topology/BaseDomain.h>

#include <Eigen/Core>

namespace caribou::topology {

    template <UNSIGNED_INTEGER_TYPE Dimension>
    void UnstructuredMesh<Dimension>::add_domain(std::string name, std::unique_ptr<BaseDomain> domain)
    {
        if (p_domains.find(name) != p_domains.end()) {
            throw std::domain_error(std::string("A domain with the name ") + name + " already exists.");
        }
        p_domains.insert({name, std::move(domain)});
    }

    template <UNSIGNED_INTEGER_TYPE Dimension>
    auto UnstructuredMesh<Dimension>::position(UNSIGNED_INTEGER_TYPE index) const -> Eigen::Matrix<FLOATING_POINT_TYPE, Dimension, 1>
    {
        return p_positions.row(index);
    }

    template class UnstructuredMesh<1>;
    template class UnstructuredMesh<2>;
    template class UnstructuredMesh<3>;
}