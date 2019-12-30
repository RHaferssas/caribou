#ifndef CARIBOU_GEOMETRY_TEST_CONCEPTS_H
#define CARIBOU_GEOMETRY_TEST_CONCEPTS_H
#include <Caribou/config.h>
#include <Caribou/Traits.h>
#include <Caribou/Geometry2/Traits/Element.h>

#include <Eigen/Core>

using Mat33 = Eigen::Matrix<FLOATING_POINT_TYPE,3,3>;
using Vec3 = Eigen::Matrix<FLOATING_POINT_TYPE,3,1>;
using Vec8 = Eigen::Matrix<FLOATING_POINT_TYPE,8,1>;
using Mat83 = Eigen::Matrix<FLOATING_POINT_TYPE, 8, 3>;

struct ConstantElement {
    // Constants
    static constexpr UNSIGNED_INTEGER_TYPE CanonicalDimension = 3;
    static constexpr INTEGER_TYPE Dimension = 3;
    static constexpr INTEGER_TYPE NumberOfNodesAtCompileTime = 8;

    // Constructor
    ConstantElement(const FLOATING_POINT_TYPE[NumberOfNodesAtCompileTime]) {};

    // Functions
    inline UNSIGNED_INTEGER_TYPE number_of_nodes() const {return NumberOfNodesAtCompileTime;}
    inline Vec3  node(UNSIGNED_INTEGER_TYPE) const {return Vec3::Zero();};
    inline Mat83 nodes() const {return Mat83::Identity();};
};

struct ConstantIsoparametricElement : ConstantElement {
    using ConstantElement::ConstantElement;
    inline Vec8 N (const Vec3 & ) const { return Vec8::Zero(); }
};

static_assert(caribou::geometry::is_an_element_v<ConstantElement>);
static_assert(caribou::geometry::is_an_element_v<ConstantIsoparametricElement>);


struct DynamicElement {
    // Constants
    static constexpr UNSIGNED_INTEGER_TYPE CanonicalDimension = 3;
    static constexpr INTEGER_TYPE Dimension = 3;
    static constexpr INTEGER_TYPE NumberOfNodesAtCompileTime = caribou::Dynamic;

    // Constructor
    DynamicElement(FLOATING_POINT_TYPE*, UNSIGNED_INTEGER_TYPE) {};

    // Functions
    inline UNSIGNED_INTEGER_TYPE number_of_nodes() const {return NumberOfNodesAtCompileTime;}
    inline Vec3  node(UNSIGNED_INTEGER_TYPE) const {return Vec3::Zero();};
    inline Mat83 nodes() const {return Mat83::Identity();};
};

//static_assert(caribou::geometry::is_an_element_v<DynamicElement>);
#endif //CARIBOU_GEOMETRY_TEST_CONCEPTS_H
