#include <Caribou/Topology/IO/VTKReader.h>
#include <filesystem>

#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkCellData.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkSmartPointer.h>

namespace fs = std::filesystem;

namespace caribou::topology::io {
    template<UNSIGNED_INTEGER_TYPE Dimension>
    VTKReader<Dimension>::VTKReader(const std::string &filepath) : p_filepath(filepath) {
        // Make sure filepath is a valid path
        if (not fs::exists(filepath)) {
            throw std::invalid_argument("File '" + filepath + "' does not exists or cannot be read.");
        }

        // Get all data from the file
        auto p_reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
        p_reader->SetFileName(filepath.c_str());
        p_reader->Update();

        if(p_reader->IsFilePolyData()) {
            std::cout << "input is a polydata" << std::endl;
            vtkPolyData* output = p_reader->GetPolyDataOutput();
            std::cout << "input has " << output->GetNumberOfPoints() << " points." << std::endl;
        } else if (p_reader->IsFileUnstructuredGrid()) {
            std::cout << "input is an unstructed grid." << std::endl;
            vtkUnstructuredGrid * output = p_reader->GetUnstructuredGridOutput();
            std::cout << "input has " << output->GetNumberOfCells() << " cells." << std::endl;

            vtkSmartPointer <vtkCellTypes> types = vtkSmartPointer <vtkCellTypes>::New();
            output->GetCellTypes(types);
            std::cout << types->GetNumberOfTypes() << " types" << std::endl;
            for (unsigned int i = 0; i < types->GetNumberOfTypes(); ++i) {
                const auto type = types->GetCellType(i);
                auto cells = vtkSmartPointer <vtkIdTypeArray>::New();
                output->GetIdsOfCellsOfType(type, cells);
                std::cout << vtkCellTypes::GetClassNameFromTypeId(type) << " : " << cells->GetDataSize() << std::endl;
            }

            vtkIdType numberOfCellArrays = output->GetCellData()->GetNumberOfArrays();
            for (vtkIdType i = 0; i < numberOfCellArrays; i++) {
                const auto dataTypeID = output->GetCellData()->GetArray(i)->GetDataType();
                const auto dataTypeIDStr = output->GetCellData()->GetArray(i)->GetDataTypeAsString();
                std::cout << "Array " << i << ": "
                          << output->GetCellData()->GetArrayName(i)
                          << " (type: " << dataTypeIDStr << " - " << dataTypeID << ")" << std::endl;
            }

        } else {
            std::cout << "Unknown input type" << std::endl;
        }
    }

    template class VTKReader<1>;
    template class VTKReader<2>;
    template class VTKReader<3>;
}