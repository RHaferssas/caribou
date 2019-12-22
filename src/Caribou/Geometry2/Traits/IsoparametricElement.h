#ifndef CARIBOU_GEOMETRY_TRAITS_ISOPARAMETRIC_ELEMENT_H
#define CARIBOU_GEOMETRY_TRAITS_ISOPARAMETRIC_ELEMENT_H

#include <Caribou/config.h>
#include <Caribou/Traits.h>
#include <Caribou/Geometry2/Traits/Element.h>

namespace caribou::geometry {
    namespace internal {
        template<class T>
        using element_canonical_dimension_t = decltype( T::CanonicalDimension );
    }


    template<class T, class Enable = void>
    struct is_an_isoparametric_element : std::false_type {};

    /**
     * Checks if a class is an isoparametric Element.
     *
     * For a given class to be accepted as an isoparametric element, it must be an Element, and it must contains the
     * following class concept (public members and functions).
     *
     * <code>
     *   struct T {
     *     static constexpr UNSIGNED_INTEGER_TYPE  CanonicalDimension = N2;
     *     Vec<CanonicalDimension> N(const Vec<CanonicalDimension> & local_coordinates) const = 0;
     *     Mat<NumberOfNodesAtCompileTime, CanonicalDimension> dN(const Vec<CanonicalDimension> & local_coordinates) const = 0;
     *   };
     * </code>
     *
     * with
     *   CanonicalDimension           Dimension of the canonical version of the element. For example, the canonical
     *                                dimension of a triangle is 2.
     */
    template< class T>
    struct is_an_isoparametric_element<T, CLASS_REQUIRES(
            caribou::internal::is_detected_convertible_v<UNSIGNED_INTEGER_TYPE, internal::element_canonical_dimension_t, T>
            // todo(jnbrunet): TO BE COMPLETED
    )> : std::true_type {};

    template< class T>
    constexpr bool is_an_isoparametric_element_v = is_an_element<T>::value;
}

#endif //CARIBOU_GEOMETRY_TRAITS_ISOPARAMETRIC_ELEMENT_H
