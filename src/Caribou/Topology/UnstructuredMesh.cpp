#include <Caribou/Topology/UnstructuredMesh.h>
#include <Caribou/Topology/BaseDomain.h>

namespace caribou::topology {

    template class UnstructuredMesh<1>;
    template class UnstructuredMesh<2>;
    template class UnstructuredMesh<3>;
}