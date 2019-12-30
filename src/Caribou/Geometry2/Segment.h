#ifndef CARIBOU_GEOMETRY_SEGMENT_H
#define CARIBOU_GEOMETRY_SEGMENT_H

#include <Caribou/config.h>
#include <Eigen/Core>

namespace caribou::geometry {

    template <UNSIGNED_INTEGER_TYPE Dim>
    using Vector = Eigen::Matrix<FLOATING_POINT_TYPE, Dim, 1>;

    template<typename T> struct traits;

    template<typename Derived>
    class Element {
        using WorldCoordinates = typename traits<Derived>::WorldCoordinates;
    };


    template<UNSIGNED_INTEGER_TYPE _Dimension, UNSIGNED_INTEGER_TYPE _Order = 1>
    class Segment;

    template<UNSIGNED_INTEGER_TYPE _Dimension, UNSIGNED_INTEGER_TYPE _Order>
    struct traits<Segment<_Dimension, _Order>> {
        static constexpr UNSIGNED_INTEGER_TYPE Dimension = _Dimension;
        static constexpr UNSIGNED_INTEGER_TYPE CanonicalDimension = 1;
        static constexpr UNSIGNED_INTEGER_TYPE NumberOfNodesAtCompileTime = 2;
        using WorldCoordinates = Vector<Dimension>;
        using LocalCoordinates = Vector<CanonicalDimension>;
    };

    template<UNSIGNED_INTEGER_TYPE _Dimension>
    class Segment <_Dimension, 1> : public Element<Segment <_Dimension, 1>> {
    public:
        inline constexpr auto number_of_nodes() const {
            return traits<Segment<_Dimension>>::NumberOfNodesAtCompileTime;
        }


    private:

    };

}

#endif //CARIBOU_GEOMETRY_SEGMENT_H
