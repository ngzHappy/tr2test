#pragma once

#include <map>
#include <set>
#include <mutex>
#include <atomic>
#include <string>
#include <memory>
#include <utility>
#include <cstddef>
#include <cstdint>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <string_view>
#include <forward_list>
#include "sstd_register_class_information.hpp"

namespace _18_11_18_private {
    /*分配内存的容器...*/
    using string = std::string;
    template<typename T>
    using forward_list = std::forward_list<T>;
    template<typename K>
    using set = std::set<K>;
    /*将对象包装成一object或者named_object*/
    template<typename T, typename U>
    class alignas((alignof(T) > alignof(void *)) ? alignof(T) : alignof(void *)) object_wrap final : public U{
        T mmm_data;
        static_assert(false == std::is_reference_v<T>);
        static_assert(false == std::is_array_v<T>);
    public:
        inline void * get_data() const override {
            return get_type_data();
        }
        inline T * get_type_data() const {
            return const_cast<T *>(&mmm_data);
        }
        template<typename A0, typename ... A,
            typename = std::enable_if_t<
            true == std::is_constructible_v<T, A0&&, A&&...>> >
            inline object_wrap(A0&&a0, A && ... args) : mmm_data(std::forward<A0>(a0),
                std::forward<A>(args)...) {
        }
        template<typename A0, typename ... A,
            typename = decltype(nullptr),
            typename = std::enable_if_t<
            false == std::is_constructible_v<T, A0&&, A&&...>> >
            inline object_wrap(A0&&a0, A && ... args) : mmm_data{ std::forward<A0>(a0),
                std::forward<A>(args)... } {
        }
        inline object_wrap() : mmm_data{} {
        }
    };
    class object {
    public:
        virtual void * get_data() const = 0;
        virtual ~object() noexcept;
        object(const object &) = delete;
        object(object &&) = delete;
        object&operator=(const object &) = delete;
        object&operator=(object&&) = delete;
        object() noexcept;
    };
    class named_object : public object {
        using string_type = object_wrap<_18_11_18_private::string, object>;
        string_type * mmm_name{ nullptr };
    public:
        named_object() noexcept;
        virtual ~named_object() noexcept;
        void set_name(const std::string_view &);
        std::string_view get_name() const;
    };
}/*namespace*/

template<typename T>
class sstd_register_virtual_basic;
class sstd_virtual_basic {
protected:
    using sstd_type_index_t_1 = _18_11_18_private::ExportRegisterClassInformation::sstd_type_index_t;
    using type_index_t = sstd_type_index_t_1::second_type;
    using private_object_t = _18_11_18_private::object;
public:
    using sstd_type_index_t = sstd_type_index_t_1;
    /*获得类的index_type*/
    template<typename T>
    inline static const sstd_type_index_t & get_sstd_type_index() noexcept {
        return _18_11_18_private::ExportRegisterClassInformation::ppp_get_type_index<T>();
    }
    /*获得当前类的mutex，此函数线程安全*/
    std::shared_ptr< std::recursive_mutex > sstd_get_class_mutex() const noexcept /*此函数线程安全*/;
private:
    using mutex_t_t = _18_11_18_private::object_wrap<std::shared_ptr< std::recursive_mutex >, private_object_t>;
    using maped_named_objects_t = _18_11_18_private::map< std::string_view, private_object_t * >;
    using mmm_objects_in_this_t = _18_11_18_private::forward_list< std::unique_ptr< private_object_t > >;
    using maped_named_objects_t_t = _18_11_18_private::object_wrap<maped_named_objects_t, private_object_t>;
    using mmm_objects_in_this_t_t = _18_11_18_private::object_wrap<mmm_objects_in_this_t, private_object_t>;
    using items_map_t = _18_11_18_private::ExportRegisterClassInformation::static_cast_map;
    maped_named_objects_t_t * mmm_named_objects{ nullptr }/*用于存储命名动态对象*/;
    mmm_objects_in_this_t_t * mmm_objects_in_this{ nullptr }/*用于存储动态对象*/;
    mutable std::atomic< mutex_t_t * > mmm_mutex{ nullptr }/*用于存储一个mutex*/;
public:
    virtual ~sstd_virtual_basic();
    /*获得当前对象的ID*/
    virtual const sstd_type_index_t & sstd_get_type_index() const = 0;
    /*获得当前对象向上转换函数表*/
    virtual const items_map_t * sstd_get_super_objects_map() const = 0;
    /*获得当前对象指针*/
    virtual void * sstd_get_this_pointer() const = 0;
    /*获得virtual_basic对象指针*/
    inline sstd_virtual_basic * sstd_get_virtual_basic() const {
        return const_cast<sstd_virtual_basic *>(this);
    }
    /*获得static_cast函数*/
    virtual void * sstd_find_object(const sstd_type_index_t & k) const;
    /*在类内部创建对象*/
    template<typename T, typename ... U>
    inline T * sstd_create_object_in_this_class(U && ...);
    /*在类内部创建命名对象*/
    template<typename T, typename ... U>
    inline T * sstd_create_named_object_in_this_class(std::string_view, U && ...);
    /*查找命名对象*/
    void * sstd_find_named_object(const std::string_view &) const noexcept;
    sstd_virtual_basic() noexcept;
public:
    sstd_virtual_basic(const sstd_virtual_basic &) = delete;
    sstd_virtual_basic(sstd_virtual_basic&&) = delete;
    sstd_virtual_basic&operator=(const sstd_virtual_basic &) = delete;
    sstd_virtual_basic&operator=(sstd_virtual_basic&&) = delete;
private:
    template<typename T>
    friend class sstd_register_virtual_basic;
    maped_named_objects_t * get_named_objects() noexcept;
    mmm_objects_in_this_t * get_objects_list() noexcept;
    void destory_objects_in_this() noexcept;
    void insert_object(std::unique_ptr< private_object_t > &&) noexcept;
    void insert_named_object(const std::string_view &, std::unique_ptr< private_object_t > &&) noexcept;
};

template<typename T, typename ... U>
inline T * sstd_virtual_basic::sstd_create_object_in_this_class(U && ...args) {
    using sstd_this_type_1 = std::remove_cv_t< T >;
    using sstd_this_object_1 = _18_11_18_private::object;
    using sstd_this_wrap_1 = _18_11_18_private::object_wrap<sstd_this_type_1, sstd_this_object_1>;
    std::unique_ptr varAnsUnique = std::make_unique<sstd_this_wrap_1>(std::forward<U>(args)...);
    auto varAns = reinterpret_cast<T *>(varAnsUnique->get_data());
    this->insert_object(std::move(varAnsUnique));
    return varAns;
}

template<typename T, typename ... U>
inline T * sstd_virtual_basic::sstd_create_named_object_in_this_class(std::string_view name, U && ... args) {
    using sstd_this_type_1 = std::remove_cv_t< T >;
    using sstd_this_object_1 = _18_11_18_private::named_object;
    using sstd_this_wrap_1 = _18_11_18_private::object_wrap<sstd_this_type_1, sstd_this_object_1>;
    std::unique_ptr varAnsUnique = std::make_unique<sstd_this_wrap_1>(std::forward<U>(args)...);
    auto varAns = reinterpret_cast<T *>(varAnsUnique->get_data());
    varAnsUnique->set_name(name);
    name = varAnsUnique->get_name();
    this->insert_named_object(name, std::move(varAnsUnique));
    return varAns;
}

/*typeid*/
template<typename T>
inline static const sstd_virtual_basic::sstd_type_index_t & sstd_get_type_index() noexcept {
    return sstd_virtual_basic::get_sstd_type_index<T>();
}

/*dynamic_cast*/
template<typename T, typename U = sstd_virtual_basic * >
inline static T * sstd_find_object(const U & arg) {
    if (arg) {
        return reinterpret_cast<T *>(
            arg->sstd_find_object(::sstd_get_type_index<T>()));
    }
    return nullptr;
}

template<typename T>
class sstd_register_virtual_basic : private sstd_register_class_information<T> {
protected:
    using sstd_type_index_t = sstd_virtual_basic::sstd_type_index_t;
    using sstd_super_t = sstd_register_class_information<T>;
    using items_map_t = typename sstd_super_t::sstd_type_up_cast_map;
protected:
    inline static const sstd_type_index_t & sstd_get_type_index() {
        using sstd_this_type_ = std::remove_cv_t< std::remove_reference_t< T > >;
        return sstd_virtual_basic::get_sstd_type_index<sstd_this_type_>();
    }
    inline const static items_map_t * sstd_get_super_objects_map() {
        return sstd_super_t::get_class_up_cast_map();
    }
    inline void * sstd_get_this_pointer() const {
        using sstd_this_type_ = std::remove_cv_t< std::remove_reference_t< T > >;
        return const_cast<sstd_this_type_*>( static_cast<const sstd_this_type_ *>(this) );
    }
    inline sstd_virtual_basic * sstd_get_virtual_basic() const {
        using sstd_this_type_ = std::remove_cv_t< std::remove_reference_t< T > >;
        return static_cast<const sstd_virtual_basic *>(
            static_cast<const sstd_this_type_ *>(this))->sstd_get_virtual_basic();
    }
    inline void * sstd_find_object(const sstd_type_index_t & k) const {
        auto varMap = this->sstd_get_super_objects_map();
        auto varCastPos = varMap->find(k);
        if (varCastPos == varMap->end()) {
            return nullptr;
        }
        return (varCastPos->second)( this->sstd_get_this_pointer() );
    }
    template<typename T111, typename ... U>
    inline T111 * sstd_create_object_in_this_class(U && ...args) {
        return this->sstd_get_virtual_basic()
            ->template sstd_create_object_in_this_class<T111>(std::forward<U>(args)...);
    }
    template<typename T111, typename ... U>
    inline T111 * sstd_create_named_object_in_this_class(const std::string_view & name, U && ...args) {
        return this->sstd_get_virtual_basic()
            ->template sstd_create_named_object_in_this_class<T111>(name, std::forward<U>(args)...);
    }
    inline void * sstd_find_named_object(const std::string_view & name) const {
        return this->sstd_get_virtual_basic()->sstd_find_named_object(name);
    }
    inline std::shared_ptr< std::recursive_mutex > sstd_get_class_mutex() const noexcept/*线程安全*/ {
        return this->sstd_get_virtual_basic()
            ->sstd_get_class_mutex();
    }
};

#ifndef SSTD_REGISTER_VIRTUAL_BASIC
#define SSTD_REGISTER_VIRTUAL_BASIC(_B_) private sstd_register_virtual_basic<_B_>,\
    public virtual sstd_virtual_basic
#endif

#ifndef SSTD_VIRTUAL_CLASS
#define SSTD_VIRTUAL_CLASS public : \
using sstd_type_index_t = sstd_virtual_basic::sstd_type_index_t; \
inline const sstd_type_index_t & sstd_get_type_index() const override { \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>::sstd_get_type_index(); \
} \
inline void * sstd_get_this_pointer() const override{ \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>::sstd_get_this_pointer(); \
} \
inline const _18_11_18_private::ExportRegisterClassInformation::static_cast_map * sstd_get_super_objects_map() const override { \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>::sstd_get_super_objects_map(); \
} \
inline void * sstd_find_object(const sstd_type_index_t & k) const override { \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>::sstd_find_object(k); \
} \
inline sstd_virtual_basic * sstd_get_virtual_basic() const { \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>::sstd_get_virtual_basic(); \
} \
inline void * sstd_find_named_object(const std::string_view & name) const { \
using sstd_this_type_ = std::remove_cv_t< \
    std::remove_reference_t< decltype(*this) > >; \
return sstd_register_virtual_basic<sstd_this_type_>::sstd_find_named_object(name); \
} \
template<typename T111, typename ... U> \
inline T111 * sstd_create_named_object_in_this_class(const std::string_view & name, U && ...args) { \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>:: \
    template sstd_create_named_object_in_this_class<T111>(name, std::forward<U>(args)...); \
} \
template<typename T111, typename ... U> \
inline T111 * sstd_create_object_in_this_class(U && ...args) { \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>:: \
    template sstd_create_object_in_this_class<T111>(std::forward<U>(args)...); \
} \
inline std::shared_ptr< std::recursive_mutex > sstd_get_class_mutex() const noexcept{ \
    using sstd_this_type_ = std::remove_cv_t< \
        std::remove_reference_t< decltype(*this) > >; \
    return sstd_register_virtual_basic<sstd_this_type_>:: \
        sstd_get_class_mutex(); \
} \
template<typename > \
friend class sstdg::sstd_register_virtual_basic;
/***************************************************************/
#endif
