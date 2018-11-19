#pragma once 

#include <cstdint>
#include <utility>
#include <algorithm>
#include <type_traits>

namespace sstd {
    namespace type_traits {

        /*获得第一个类型*/
        template<typename > class get_first;
        /*获得第N个类型*/
        template<std::size_t, typename > class get_from_index;
        /*判断是否存在类型*/
        template<typename/*types*/, typename/*type*/> class has_class;
        /*如果不存在，则追加一个类*/
        template<typename/*types*/, typename/*type*/> class unique_append;
        /*删除重复类*/
        template<typename/*types*/> class unique;
        /*合并多个*/
        template<typename ... > class cat;
        /*合并之后删除重复类*/
        template<typename ... > class unique_cat;
        /*将tree展开成list*/
        template<typename > class tree_to_list;
        /*替换外包装*/
        template<typename A, template<typename ... >class B>class replace;
        template<
            template<typename ... >class A, typename ...AT,
            template<typename ... >class B
        > class replace<A<AT...>, B> {
        public:
            using type = B<AT ...>;
        };

        template<typename ...TName>
        class class_wrap {
        public:
            constexpr static inline std::size_t size() {
                return sizeof...(TName);
            }
        };

        using zero_void_type = class_wrap<>;

        template<>
        class class_wrap<zero_void_type> {
        public:
            constexpr static inline std::size_t size() {
                return 0;
            }
        };

        template<template<typename ...> class A, typename ATF, typename ATS, typename ... AT>
        class get_first< A< ATF, ATS, AT ...> > {
        public:
            using first_type = ATF;
            using second_type = ATS;
            using tail = class_wrap<ATS, AT...>;
            constexpr const static bool has_first = true;
            constexpr const static bool has_second = true;
        };

        template<template<typename > class A, typename ATF>
        class get_first< A<ATF> > {
        public:
            using first_type = ATF;
            using second_type = zero_void_type;
            using tail = zero_void_type;
            constexpr const static bool has_first = true;
            constexpr const static bool has_second = false;
        };

        template< template<typename ...> class A >
        class get_first< A<> > {
        public:
            using first_type = zero_void_type;
            using second_type = zero_void_type;
            using tail = zero_void_type;
            constexpr const static bool has_first = false;
            constexpr const static bool has_second = false;
        };

        template<>
        class get_first< zero_void_type > {
        public:
            using first_type = zero_void_type;
            using second_type = zero_void_type;
            using tail = zero_void_type;
            constexpr const static bool has_first = false;
            constexpr const static bool has_second = false;
        };

        template< template<typename ...> class A, typename ... AT  >
        class get_from_index<0, A<AT...>> {
        public:
            using type = typename get_first<A<AT...>>::first_type;
        };

        template<std::size_t N, template<typename ...> class A, typename ... AT  >
        class get_from_index<N, A<AT...>> {
            using tail = typename get_first<A<AT...>>::tail;
        public:
            using type = std::conditional_t<
                (N < sizeof...(AT)),
                typename get_from_index<N - 1, tail>::type,
                zero_void_type>;
        };

        template<template<typename > class A, typename AT, typename B>
        class has_class< A<AT>, B > {
        public:
            const constexpr static bool value = std::is_same_v<AT, B>;
        };

        template<typename B>
        class has_class< zero_void_type, B > {
        public:
            const constexpr static bool value = false;
        };

        template<template<typename ... > class A, typename AT0, typename AT1, typename ... AT, typename B>
        class has_class<A<AT0, AT1, AT...>, B> {
            using tail = typename get_first< A< AT0, AT1, AT... > >::tail;
        public:
            const constexpr static bool value = std::is_same_v<AT0, B> || has_class<tail, B>::value;
        };

        template<template <typename ...> class  A, typename ... AT, typename B>
        class unique_append<A<AT...>, B> {
        public:
            using type = std::conditional_t<has_class<A<AT...>, B>::value, class_wrap<AT...>, class_wrap<AT..., B> >;
        };

        template<template <typename ...> class  A, typename ... AT >
        class unique_append<A<AT...>, zero_void_type> {
        public:
            using type = class_wrap<AT...>;
        };

        template<
            template <typename ...> class A,
            template <typename ...> class B,
            typename ... AT
        > class unique_append<A<AT...>, B<AT...>> {
        public:
            using type = class_wrap<AT...>;
        };

        namespace _private {

            template<typename, typename>
            class unique_detail;

            template<
                template<typename ...> class A, typename ... AT,
                template<typename > class B, typename BT >
            class unique_detail<A<AT...>, B<BT>> {
            public:
                using type = typename unique_append<A<AT...>, BT>::type;
            };

            template<
                template<typename ...> class A, typename ... AT,
                template<typename ... > class B, typename BT0,
                typename BT1, typename ... BT >
                class unique_detail<A<AT...>, B<BT0, BT1, BT...>> {
                using type0 = typename unique_append<A<AT...>, BT0>::type;
                public:
                    using type = typename unique_detail<type0, class_wrap<BT1, BT...>>::type;
            };

        }/*_private*/

        template<template<typename> class T, typename T0>
        class unique<T<T0>> {
        public:
            using type = class_wrap<T0>;
        };

        template<template<typename...> class T, typename T0, typename T1, typename ... TT>
        class unique<T<T0, T1, TT...>> {
        public:
            using type = typename _private::unique_detail<
                zero_void_type,
                class_wrap<T0, T1, TT...> >::type;
        };

        namespace _private {
            template<typename T1, typename T2>
            class cat2;

            template<template<typename ...> class A,
                template<typename ...> class B,
                typename ... AT, typename ... BT
            >class cat2<A<AT ...>, B<BT ...>> {
            public:
                using type = class_wrap< AT ..., BT ... >;
            };

        }/*_private*/

        template<template<typename ...> class A, typename ... TName>
        class cat<A<TName...>> {
        public:
            using type = class_wrap<TName...>;
        };

        template<template<typename ...> class A0, typename ...T0,
            template<typename ...> class A1, typename ...T1 >
        class cat<A0<T0...>, A1<T1...>> {
        public:
            using type = typename _private::cat2< class_wrap<T0...>, class_wrap<T1...> >::type;
        };

        template<typename T1, typename T2, typename T3, typename ... TName>
        class cat<T1, T2, T3, TName...> {
        public:
            using type = typename cat< typename cat<T1, T2>::type, typename cat<T3, TName...>::type >::type;
        };

        template<typename ... TName>
        class unique_cat {
            using cat_type = typename cat<TName...>::type;
        public:
            using type = typename unique<cat_type>::type;
        };

        namespace _private_helper {

            /*检查一个类型是否定义了 typename T::supers */
            template<typename TName, typename = void>
            class has_supers_help {
            public:
                using supers = zero_void_type;
                using type = supers;
            };

            template<typename TName>
            class has_supers_help<TName, std::enable_if_t<sizeof(typename TName::supers)>> {
            public:
                using supers = typename TName::supers;
                using type = supers;
            };

            template<std::size_t N, typename TName, bool = (N < TName::size())
            >class tree_to_list_helper {
            public:
                using type = TName;
            };

            template<std::size_t N, typename TName
            >class tree_to_list_helper<N, TName, true > {
                static_assert(N < 256, "this may be a error");
                using TypeN = typename get_from_index<N, TName>::type/*get_from_index*/;
                using TypeNSupers = typename has_supers_help</*HasSupers*/TypeN>::type/*HasSupers*/;
                using TypeNext = typename unique_cat< TName, TypeNSupers/*unique_cat*/>::type/*unique_cat*/;
            public:
                using type =
                    typename tree_to_list_helper< N + 1, TypeNext/*TreeToListHelper*/>::type/*TreeToListHelper*/;
            };

        }/*_private_helper*/

        template<typename TName
        >class tree_to_list {
            using _supers = typename unique_cat<
                zero_void_type,
                typename _private_helper::has_supers_help<TName>::type
            >::type;
        public:
            using type = typename _private_helper::tree_to_list_helper< 0, _supers  >::type;
        };

    }/*namespace type_traits*/
}/*namespace sstd*/






