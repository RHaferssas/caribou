#include <Caribou/Topology/UnstructuredMesh.h>
#include <Caribou/Topology/BaseDomain.h>

namespace caribou::topology {

    template <UNSIGNED_INTEGER_TYPE Dimension>
    BaseDomain * UnstructuredMesh<Dimension>::do_add_domain(std::string name, std::unique_ptr<BaseDomain> domain)
    {
        if (p_domains.find(name) != p_domains.end()) {
            throw std::domain_error(std::string("A domain with the name ") + name + " already exists.");
        }
        auto res = p_domains.insert({name, std::move(domain)});
        if (res.second) {
            return res.first->second.get();
        } else {
            return static_cast<BaseDomain*> (nullptr);
        }
    }

    template class UnstructuredMesh<1>;
    template class UnstructuredMesh<2>;
    template class UnstructuredMesh<3>;
}