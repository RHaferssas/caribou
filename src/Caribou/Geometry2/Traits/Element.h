#ifndef CARIBOU_GEOMETRY_TRAITS_ELEMENT_H
#define CARIBOU_GEOMETRY_TRAITS_ELEMENT_H

#include <Caribou/config.h>
#include <Caribou/Traits.h>
#include <Caribou/Constants.h>

namespace caribou::geometry {
    namespace internal {

        template<class T>
        using element_dimension_t = decltype( T::Dimension );

        template<class T>
        using element_number_of_nodes_at_compile_time_t = decltype( T::NumberOfNodesAtCompileTime );

        template<class T>
        using element_number_of_nodes_t = decltype(std::declval<const T&>().number_of_nodes());

        template<class T, typename Arg>
        using element_node_t = decltype(std::declval<const T&>().node(std::declval<Arg>()));

        template <class T>
        using element_nodes_t = decltype(std::declval<const T&>().nodes());

    }


    template<class T, class Enable = void>
    struct is_an_element : std::false_type {};

    /**
     * Checks if a class is a geometrical Element.
     *
     * For a given class to be accepted as an Element, it must contains the following class concept (public members and
     * functions).
     *
     * <code>
     *   struct T {
     *     static constexpr INTEGER_TYPE           NumberOfNodesAtCompileTime = N1;
     *     static constexpr UNSIGNED_INTEGER_TYPE  Dimension = N3;
     *
     *     T (const FLOATING_POINT_TYPE [NumberOfNodesAtCompileTime*Dimension] positions) = 0; // Must be defined if NumberOfNodesAtCompileTime != Dynamic
     *     T (FLOATING_POINT_TYPE * positions, UNSIGNED_INTEGER_TYPE number_of_nodes) = 0; // Must be defined if NumberOfNodesAtCompileTime == Dynamic
     *     UNSIGNED_INTEGER_TYPE number_of_nodes () const = 0;
     *     Vector<Dimension> node (UNSIGNED_INTEGER_TYPE index) const = 0;
     *     Matrix<NumberOfNodesAtCompileTime, Dimension> nodes () const = 0;
     *   };
     * </code>
     *
     * with
     *   NumberOfNodesAtCompileTime   Known number of nodes in the elements at compile time. If the number is dynamic
     *                                (determined during runtime), set it to -1.
     *   Dimension                    Dimension of the element. For example, a triangle represented in a 3D world has
     *                                its dimension set to 3 (x, y, z), whereas its canonical dimension is
     *                                always 2 (u, v).
     */
    template< class T>
    struct is_an_element<T, CLASS_REQUIRES(
            caribou::internal::is_detected_convertible_v<INTEGER_TYPE,          internal::element_number_of_nodes_at_compile_time_t, T> and
            caribou::internal::is_detected_convertible_v<UNSIGNED_INTEGER_TYPE, internal::element_dimension_t, T> and
            (
                    (T::NumberOfNodesAtCompileTime == caribou::Dynamic and std::is_constructible_v<T, FLOATING_POINT_TYPE*, UNSIGNED_INTEGER_TYPE>) or
                    (T::NumberOfNodesAtCompileTime != caribou::Dynamic and std::is_constructible_v<T, FLOATING_POINT_TYPE[T::NumberOfNodesAtCompileTime*T::Dimension]>)
            ) and
            caribou::internal::is_detected_convertible_v<UNSIGNED_INTEGER_TYPE, internal::element_number_of_nodes_t, T> and
            caribou::internal::is_detected_v<internal::element_node_t, T, UNSIGNED_INTEGER_TYPE> and
            caribou::internal::is_detected_v<internal::element_nodes_t, T>
    )> : std::true_type {};

    template< class T>
    constexpr bool is_an_element_v = is_an_element<T>::value;
}

#endif //CARIBOU_GEOMETRY_TRAITS_ELEMENT_H
