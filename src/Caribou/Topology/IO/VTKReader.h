#ifndef CARIBOU_TOPOLOGY_IO_VTKREADER_H
#define CARIBOU_TOPOLOGY_IO_VTKREADER_H

#include <string>
#include <Caribou/config.h>

namespace caribou::topology::io {
    template<UNSIGNED_INTEGER_TYPE Dimension>
    class VTKReader {
    public:
        static VTKReader Read(const std::string & filepath) {
            return VTKReader(filepath);
        }

        VTKReader & addDomain(const std::string & name, unsigned int vtkCellType);

    private:
        VTKReader(const std::string &filepath);

        const std::string p_filepath;
    };

    extern template class VTKReader<1>;
    extern template class VTKReader<2>;
    extern template class VTKReader<3>;
}

#endif //CARIBOU_TOPOLOGY_IO_VTKREADER_H
