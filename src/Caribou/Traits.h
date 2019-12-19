#ifndef CARIBOU_TRAITS_H
#define CARIBOU_TRAITS_H

#include <type_traits>

namespace caribou {

template<typename T> struct traits;
template<typename T> struct traits<const T> : traits<T> {};

/**
 * REQUIRES(condition)
 * Usage for function template parameter
 *
 * Same thing as std::enable_if, just easier to read for the user.
 */
# define REQUIRES(...)                                      \
  typename std::enable_if<(__VA_ARGS__), bool>::type = true

/**
* CLASS_REQUIRES(condition)
* Usage for function template parameter
*
* Same thing as std::enable_if, just easier to read for the user.
*/
# define CLASS_REQUIRES(...)                                      \
  typename std::enable_if<(__VA_ARGS__)>::type


namespace internal {
/**
 * Detector infrastructure
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf
 */
// primary template handles all types not supporting the archetypal Op:
        template<class Default, class// always void; supplied externally
                , template<class...> class Op, class... Args
        >
        struct detector {
            using value_t = std::false_type;
            using type = Default;
        };

// the specialization recognizes and handles only types supporting Op:
        template<class Default, template<class...> class Op, class... Args
        >
        struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
            using value_t = std::true_type;
            using type = Op<Args...>;
        };

// nonesuch
        struct nonesuch {
            nonesuch() = delete;

            ~nonesuch() = delete;

            nonesuch(nonesuch const &) = delete;

            void operator=(nonesuch const &) = delete;
        };

// is_detected
        template<template<class...> class Op, class... Args>
        using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

        template<template<class...> class Op, class... Args>
        constexpr bool is_detected_v = is_detected<Op, Args...>::value;

        template<template<class...> class Op, class... Args>
        using detected_t= typename detector<nonesuch, void, Op, Args...>::type;


// detected_or
        template<class Default, template<class...> class Op, class... Args>
        using detected_or= detector<Default, void, Op, Args...>;

        template<class Default, template<class...> class Op, class... Args>
        using detected_or_t= typename detected_or<Default, Op, Args...>::type;

// is_detected_exact
        template<class Expected, template<class...> class Op, class... Args>
        using is_detected_exact= std::is_same<Expected, detected_t<Op, Args...> >;

        template<class Expected, template<class...> class Op, class... Args>
        constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

// is_detected_convertible
        template<class To, template<class...> class Op, class... Args>
        using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

        template<class To, template<class...> class Op, class... Args>
        constexpr bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;
    }
} // namespace caribou
#endif //CARIBOU_TRAITS_H
