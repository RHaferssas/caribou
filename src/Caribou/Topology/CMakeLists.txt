cmake_minimum_required(VERSION 3.1)
project(Caribou.Topology)

set(HEADER_FILES
    BaseMesh.h
    BaseDomain.h
    Domain.h
    Grid/Grid.h
    Grid/Internal/BaseGrid.h
    Grid/Internal/BaseMultidimensionalGrid.h
    Grid/Internal/BaseUnidimensionalGrid.h
    HashGrid.h
    Mesh.h
    UnstructuredMesh.h
)

set(SOURCE_FILES
    UnstructuredMesh.cpp
)

find_package(VTK COMPONENTS vtkCommonDataModel vtkIOLegacy QUIET)

if (VTK_FOUND)
    option(CARIBOU_WITH_VTK "Use the VTK for importing mesh files." ON)
else()
    option(CARIBOU_WITH_VTK "Use the VTK for importing mesh files." OFF)
endif()

if (CARIBOU_WITH_VTK)
    find_package(VTK COMPONENTS vtkCommonDataModel vtkIOLegacy REQUIRED)
    message (STATUS "VTK_VERSION: ${VTK_VERSION}")
    list(APPEND HEADER_FILES IO/VTKReader.h)
    list(APPEND SOURCE_FILES IO/VTKReader.cpp)
    if (VTK_VERSION VERSION_LESS "8.90.0")
        # old system
        include(${VTK_USE_FILE})
    endif()
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

target_link_libraries(${PROJECT_NAME} PUBLIC Caribou.Geometry)

if (CARIBOU_WITH_VTK)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CARIBOU_WITH_VTK)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${VTK_LIBRARIES})
    if (VTK_VERSION VERSION_GREATER_EQUAL "8.90.0")
        # new system
        vtk_module_autoinit(
                TARGETS ${PROJECT_NAME}
                MODULES ${VTK_LIBRARIES}
        )
    endif()
endif()

if (CARIBOU_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (CARIBOU_WITH_PYTHON_2 OR CARIBOU_WITH_PYTHON_3)
    add_subdirectory(Python)
endif()