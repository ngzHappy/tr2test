#include "sstd_type_traits.hpp"
#include "register_class_information.hpp"
#include <tuple>

void test_sstd_type_traints() {

    {
        using t1 = sstd::type_traits::class_wrap<>;
        static_assert(std::is_same_v< sstd::type_traits::get_first< t1 >::first_type, sstd::type_traits::zero_void_type >);
    }

    {
        using t1 = std::tuple<int>;
        static_assert(std::is_same_v< sstd::type_traits::get_first< t1 >::first_type, int >);
    }

    {
        using t1 = std::tuple<int, double, float, sstd::type_traits::zero_void_type>;
        static_assert(std::is_same_v< sstd::type_traits::get_from_index<0, t1 >::type, int >);
        static_assert(std::is_same_v< sstd::type_traits::get_from_index<1, t1 >::type, double >);
        static_assert(std::is_same_v < sstd::type_traits::get_from_index<2, t1 >::type, float >);
        static_assert(std::is_same_v < sstd::type_traits::get_from_index<3, t1 >::type, sstd::type_traits::zero_void_type>);
        static_assert(std::is_same_v < sstd::type_traits::get_from_index<4, t1 >::type, sstd::type_traits::zero_void_type>);
    }

    {
        using t1 = std::tuple<int, double, float, sstd::type_traits::zero_void_type>;
        static_assert(true == sstd::type_traits::has_class< t1, int >::value);
        static_assert(false == sstd::type_traits::has_class< t1, int * >::value);
    }

    {
        using t1 = sstd::type_traits::class_wrap<int>;
        using t2 = sstd::type_traits::class_wrap<int, double>;
        static_assert(std::is_same_v< sstd::type_traits::unique_append<t1, int>::type, t1  >);
        static_assert(std::is_same_v< sstd::type_traits::unique_append<t1, double>::type, t2  >);
        static_assert(std::is_same_v<
            sstd::type_traits::unique_append<
            sstd::type_traits::zero_void_type,
            sstd::type_traits::zero_void_type >::type,
            sstd::type_traits::zero_void_type>);
    }

    {
        using t1 = sstd::type_traits::class_wrap<int, int, sstd::type_traits::zero_void_type>;
        using t2 = sstd::type_traits::class_wrap<int>;
        static_assert(std::is_same_v<sstd::type_traits::unique<t1>::type, t2>);
    }

    {
        using t1 = std::tuple<int>;
        using t2 = std::tuple<double>;
        using t3 = sstd::type_traits::zero_void_type;
        using ta = sstd::type_traits::cat<t2, t3, t1>::type;
        static_assert(std::is_same_v<ta, sstd::type_traits::class_wrap<double, int>>);
    }

    {
        using t1 = sstd::type_traits::class_wrap<int>;
        using t2 = std::tuple<int>;
        static_assert(std::is_same_v< sstd::type_traits::replace<t1, std::tuple>::type, t2>);
    }

}

class A {
};

class B : public A {
};

class C : public B {
};

class D {
};

class E {
};

class F : public D, public E {
};

class G : public C, public F {
};

template<>
class sstd_bases<A> {
public:
    using supers = sstd::type_traits::class_wrap<>;
    using type = sstd::type_traits::tree_to_list< sstd_bases >::type;
};

template<>
class sstd_bases<B> {
public:
    using supers = sstd::type_traits::class_wrap< sstd_bases<A> >;
    using type = sstd::type_traits::tree_to_list_t< sstd_bases >;
};

template<>
class sstd_bases<C> {
public:
    using supers = sstd::type_traits::class_wrap< sstd_bases<B> >;
    using type = sstd::type_traits::tree_to_list_t< sstd_bases >;
};

template<>
class sstd_bases<D> {
public:
    using supers = sstd::type_traits::class_wrap<>;
    using type = sstd::type_traits::tree_to_list< sstd_bases >::type;
};

template<>
class sstd_bases<E> {
public:
    using supers = sstd::type_traits::class_wrap<>;
    using type = sstd::type_traits::tree_to_list< sstd_bases >::type;
};

template<>
class sstd_bases<F> {
public:
    using supers = sstd::type_traits::class_wrap< sstd_bases<E>, sstd_bases<D> >;
    using type = sstd::type_traits::tree_to_list< sstd_bases >::type;
};


template<>
class sstd_bases<G> {
public:
    using supers = sstd::type_traits::class_wrap< sstd_bases<C>, sstd_bases<F> >;
    using type = sstd::type_traits::tree_to_list< sstd_bases >::type;
};

void test_supers() {

    static_assert(std::is_same_v< sstd_bases<B>::type, sstd::type_traits::class_wrap< A > >);
    static_assert(std::is_same_v< sstd_bases<C>::type, sstd::type_traits::class_wrap< B, A > >);
    static_assert(std::is_same_v< sstd_bases<G>::type, sstd::type_traits::class_wrap< C, F, B, E, D, A > >);
}


















