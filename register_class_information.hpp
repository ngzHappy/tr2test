#pragma once

#include <map>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <type_traits>

/**************************************/
#include <tr2/type_traits>
/**************************************/

namespace _18_11_18_private {

    template<typename K, typename T>
    using map = std::map<K, T>;

    class ExportRegisterClassInformation {
    public:
        using type_index = std::pair<std::size_t, std::type_index>;
        typedef void*(*static_type_cast_function)(void *);
        using static_cast_map = map<type_index, static_type_cast_function>;
        static const static_cast_map * register_up_cast_map(const type_index &, static_cast_map &&);
    protected:
        template< template<typename ...> class I, typename ... T >
        inline static std::size_t ppp_class_size(const I<T ...> &) {
            return (sizeof...(T)) + 1 ;
        }
        template<typename U>
        inline static type_index ppp_get_type_index() {
            using this_class_type_ = std::remove_cv_t< std::remove_reference_t<U> >;
            typename type_index::second_type varIndex{ typeid(this_class_type_) };
            std::hash<typename type_index::second_type> varIndexHash;
            return type_index{ varIndexHash(varIndex) ,varIndex };
        }
        template<typename From,typename To>
        inline static void ppp_insert_static_cast_function(static_cast_map * map){
            /*private or protected 继承...???*/
            if constexpr( std::is_convertible_v<From * ,To * > ){
                map->emplace( ppp_get_type_index<To>() , [](void * arg)->void *{
                    return static_cast<To *>( reinterpret_cast<From *>( arg ) );
                } );
            }else{
                return;
            }
        }
        template< typename U, template<typename ...> class I, typename ... T >
        inline static static_cast_map * ppp_create_class_up_cast(static_cast_map * map, const I<T ...> &) {
            using this_class_type_ = std::remove_cv_t< std::remove_reference_t<U> >;
            ( ppp_insert_static_cast_function< this_class_type_ ,
                    std::remove_cv_t< std::remove_reference_t< T > >
                    >(map) , ... );
            ppp_insert_static_cast_function< this_class_type_ , this_class_type_ >( map );
            return map;
        }
    };

    template<typename T>
    class RegisterClassInformation : public ExportRegisterClassInformation {
    public:
        using type_index = ExportRegisterClassInformation::type_index;
        using static_cast_map = ExportRegisterClassInformation::static_cast_map;
        static const int class_deeth/*继承树的深度*/;
        static const type_index class_index/*当前类的type_index*/;
        static const static_cast_map * class_up_cast_map/*向上类型转换表*/;
    private:
        inline static int register_class_depth();
        inline static type_index register_class_index();
        inline static const static_cast_map * register_static_up_cast_map();
    };

    template<typename T>
    const int RegisterClassInformation<T>::class_deeth =
        RegisterClassInformation<T>::register_class_depth();

    template<typename T>
    const typename RegisterClassInformation<T>::type_index RegisterClassInformation<T>::class_index =
        RegisterClassInformation<T>::register_class_index();

    template<typename T>
    const typename RegisterClassInformation<T>::static_cast_map * RegisterClassInformation<T>::class_up_cast_map =
        RegisterClassInformation<T>::register_static_up_cast_map();

    template<typename T>
    inline int RegisterClassInformation<T>::register_class_depth() {
        using this_class_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
        return ppp_class_size( typename std::tr2::bases<this_class_type_>::type{} );
    }

    template<typename T>
    inline const typename RegisterClassInformation<T>::static_cast_map * RegisterClassInformation<T>::register_static_up_cast_map() {
        using this_class_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
        static_cast_map varUpCastMap;
        ppp_create_class_up_cast<this_class_type_>(&varUpCastMap,
                                                   typename std::tr2::bases<this_class_type_>::type{});
        return RegisterClassInformation::register_up_cast_map(
            RegisterClassInformation::register_class_index(),
            std::move(varUpCastMap));
    }

    template<typename T>
    inline typename RegisterClassInformation<T>::type_index RegisterClassInformation<T>::register_class_index() {
        using this_class_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
        return ppp_get_type_index< this_class_type_ >();
    }

}/*namespace _18_11_18_private*/

template<typename T>
class RegisterClassInformation : private _18_11_18_private::RegisterClassInformation<
    std::remove_cv_t< std::remove_reference_t<T> > > {
    using super_type = _18_11_18_private::RegisterClassInformation<
        std::remove_cv_t< std::remove_reference_t<T> > >;
public:
    using type_index = typename super_type::type_index;
    using type_up_cast_map = typename super_type::static_cast_map;
    static inline const int & get_class_deeth() {
        return super_type::class_deeth;
    }
    static inline const type_index & get_class_index() {
        return super_type::class_index;
    }
    static inline const type_up_cast_map & get_class_up_cast_map() {
        return *(super_type::class_up_cast_map);
    }
};















