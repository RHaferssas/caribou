#ifndef CARIBOU_GEOMETRY_SEGMENT_H
#define CARIBOU_GEOMETRY_SEGMENT_H

#include <Caribou/config.h>

namespace caribou::geometry {

    /**
     * Segment with Lagrange polynomials of degree 1 (P1)
     *
     *
     * P1 : 0-----+-----1 --> u
     *
     */
    template<UNSIGNED_INTEGER_TYPE _Dimension>
    class Segment {
    public:
        static constexpr UNSIGNED_INTEGER_TYPE CanonicalDimension = 1
        static constexpr UNSIGNED_INTEGER_TYPE Dimension = _Dimension;
        static constexpr UNSIGNED_INTEGER_TYPE NumberOfNodesAtCompileTime = 2;

        inline constexpr auto number_of_nodes() const {
            return NumberOfNodesAtCompileTime;
        }


    private:

    };

}

#endif //CARIBOU_GEOMETRY_SEGMENT_H
