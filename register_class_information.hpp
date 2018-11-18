#pragma once

#include <map>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <type_traits>

/**************************************/
#if __has_include(<tr2/type_traits>)
#include <tr2/type_traits>
#else
#define NO_TR2_TYPE_TRAITS (1u)
#endif
/**************************************/

#if defined(NO_TR2_TYPE_TRAITS)
template<typename T>
class sstd_bases{
public:
    using type = std::tuple< T & > ;
};
#else
template<typename T>
class sstd_bases{
public:
    using type = typename std::tr2::bases<T>::type ;
};
#endif

namespace _18_11_18_private {

    template<typename K, typename T>
    using map = std::map<K, T>;

    class ExportRegisterClassInformation {
    public:
        using type_index = std::pair<std::size_t, std::type_index>;
        typedef void*(*static_type_cast_function)(void *);
        using static_cast_map = map<type_index, static_type_cast_function>;
        static const static_cast_map * register_up_cast_map(const type_index &, static_cast_map &&);
        static const static_cast_map * find_up_cast_map(const type_index &);
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
        template<typename From_,typename To_>
        inline static void ppp_insert_static_cast_function(static_cast_map * map){
            using From = std::remove_cv_t< std::remove_reference_t<From_> >;
            using To = std::remove_cv_t< std::remove_reference_t<To_> >;
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
    class RegisterClassInformation : private ExportRegisterClassInformation {
    public:
        using type_index = ExportRegisterClassInformation::type_index;
        using static_cast_map = ExportRegisterClassInformation::static_cast_map;
        static const int class_deepth/*继承树的深度*/;
        static const type_index class_index/*当前类的type_index*/;
        static const static_cast_map * class_up_cast_map/*向上类型转换表*/;
    private:
        inline static int register_class_depth();
        inline static type_index register_class_index();
        inline static const static_cast_map * register_static_up_cast_map();
    };

    template<typename T>
    const int RegisterClassInformation<T>::class_deepth =
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
        return ppp_class_size( typename sstd_bases<this_class_type_>::type{} );
    }

    template<typename T>
    inline const typename RegisterClassInformation<T>::static_cast_map * RegisterClassInformation<T>::register_static_up_cast_map() {
        using this_class_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
        static_cast_map varUpCastMap;
        ppp_create_class_up_cast<this_class_type_>(&varUpCastMap,
                                                   typename sstd_bases<this_class_type_>::type{});
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
    static inline const int & get_class_deepth() {
        return super_type::class_deepth;
    }
    static inline const type_index & get_class_index() {
        return super_type::class_index;
    }
    static inline const type_up_cast_map * get_class_up_cast_map() {
        return super_type::class_up_cast_map;
    }
};

template<typename T>
static inline const _18_11_18_private::ExportRegisterClassInformation::static_cast_map * get_dynamic_class_up_cast_map(T * arg){
    if(arg == nullptr){
        return nullptr;
    }
    using sstd_this_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
    static_assert( true == std::is_polymorphic_v<sstd_this_type_> ,"T must have virtual function" );
    using map_ = _18_11_18_private::ExportRegisterClassInformation::static_cast_map ;
    using index_type_ = map_::value_type::first_type::second_type;
    std::hash< index_type_ > varHash/*create the index hash ...*/;
    index_type_ varIndex{ typeid( *arg ) }/*get the index ...*/;
    return _18_11_18_private::ExportRegisterClassInformation::find_up_cast_map( { varHash(varIndex) , varIndex } ) ;
}

template<typename To,typename From>
static inline To * dynamic_class_pointer_cast(From * arg){
    if(arg == nullptr){
        return nullptr;
    }
    using sstd_this_type_ = std::remove_cv_t< std::remove_reference_t<From> >;
    static_assert( true == std::is_polymorphic_v<sstd_this_type_> ,"From must have virtual function" );
    const static RegisterClassInformation<To> varToInformation/*force create to class information */;
    const static RegisterClassInformation<sstd_this_type_> varFromInformation/*force create from class information*/;
    auto varCastMap = get_dynamic_class_up_cast_map( arg );
    if(varCastMap == nullptr){
        return nullptr;
    }
    auto varPos = varCastMap->find( varToInformation.get_class_index() );
    if(varPos == varCastMap->end()){
        return nullptr;
    }
    using sstd_to_type_ = std::remove_cv_t< std::remove_reference_t<To> >;
    return static_cast<sstd_to_type_ *>( ( varPos->second )( const_cast<void *>( dynamic_cast< const void * >( arg ) ) ) );
    (void)varFromInformation;
}












