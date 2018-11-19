#pragma once

#include <map>
#include <memory>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <type_traits>
#include "sstd_type_traits.hpp"

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
    using type = sstd::type_traits::class_wrap<> ;
};
#else
template<typename T>
class sstd_bases{
    using _000_type = typename std::tr2::bases<T>::type;
    using _001_type = typename sstd::type_traits::replace<_000_type, sstd::type_traits::class_wrap>::type;
public:
    using type = _001_type ;
};
#endif

namespace _18_11_18_private {/*私有部分...*/
    
    template<typename K, typename T>
    using map = std::map<K, T>;

    class ExportRegisterClassInformation {
    public:
        using sstd_type_index_t = std::pair<std::size_t, std::type_index>;
        typedef void*(*static_type_cast_function)(void *);
        using static_cast_map = map<sstd_type_index_t, static_type_cast_function>;
        static const static_cast_map * register_up_cast_map(const sstd_type_index_t &, static_cast_map &&);
        static const static_cast_map * find_up_cast_map(const sstd_type_index_t &);
    public:
        template< template<typename ...> class I, typename ... T >
        inline static std::size_t ppp_class_size(const I<T ...> &) {
            return (sizeof...(T)) + 1 ;
        }
        template<typename U>
        inline static const sstd_type_index_t & ppp_get_type_index() {
            using sstd_this_type_ = std::remove_cv_t<
                std::remove_reference_t<U> >;
            const static sstd_type_index_t varAns = []() {
                sstd_type_index_t::second_type varIndex_{ typeid(sstd_this_type_) };
                std::hash< sstd_type_index_t::second_type > varHash_;
                return sstd_type_index_t{ varHash_(varIndex_),varIndex_ };
            }();
            return varAns;
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
    class sstd_register_class_information : private ExportRegisterClassInformation {
    public:
        using sstd_type_index_t = ExportRegisterClassInformation::sstd_type_index_t;
        using static_cast_map = ExportRegisterClassInformation::static_cast_map;
        static const int sstd_m_class_deepth/*继承树的深度*/;
        static const sstd_type_index_t sstd_m_class_index/*当前类的type_index*/;
        static const static_cast_map * sstd_m_class_up_cast_map/*向上类型转换表*/;
    private:
        inline static int register_class_depth();
        inline static sstd_type_index_t register_class_index();
        inline static const static_cast_map * register_static_up_cast_map();
    };

    template<typename T>
    const int sstd_register_class_information<T>::sstd_m_class_deepth =
        sstd_register_class_information<T>::register_class_depth();

    template<typename T>
    const typename sstd_register_class_information<T>::sstd_type_index_t sstd_register_class_information<T>::sstd_m_class_index =
        sstd_register_class_information<T>::register_class_index();

    template<typename T>
    const typename sstd_register_class_information<T>::static_cast_map * sstd_register_class_information<T>::sstd_m_class_up_cast_map =
        sstd_register_class_information<T>::register_static_up_cast_map();

    template<typename T>
    inline int sstd_register_class_information<T>::register_class_depth() {
        using this_class_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
        return static_cast<int>(ppp_class_size( typename sstd_bases<this_class_type_>::type{} ));
    }

    template<typename T>
    inline const typename sstd_register_class_information<T>::static_cast_map * sstd_register_class_information<T>::register_static_up_cast_map() {
        using this_class_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
        static_cast_map varUpCastMap;
        ppp_create_class_up_cast<this_class_type_>(&varUpCastMap,
                                                   typename sstd_bases<this_class_type_>::type{});
        return sstd_register_class_information::register_up_cast_map(
            sstd_register_class_information::register_class_index(),
            std::move(varUpCastMap));
    }

    template<typename T>
    inline typename sstd_register_class_information<T>::sstd_type_index_t sstd_register_class_information<T>::register_class_index() {
        using this_class_type_ = std::remove_cv_t< std::remove_reference_t<T> >;
        return ppp_get_type_index< this_class_type_ >();
    }

}/*namespace _18_11_18_private*/

template<typename T>
class sstd_register_class_information : private _18_11_18_private::sstd_register_class_information<
    std::remove_cv_t< std::remove_reference_t<T> > > {
    using sstd_super_type = _18_11_18_private::sstd_register_class_information<
        std::remove_cv_t< std::remove_reference_t<T> > >;
public:
    using sstd_type_index_t = typename sstd_super_type::sstd_type_index_t;
    using sstd_type_up_cast_map = typename sstd_super_type::static_cast_map;
    static inline const int & get_class_deepth() {
        return sstd_super_type::sstd_m_class_deepth;
    }
    static inline const sstd_type_index_t & get_class_index() {
        return sstd_super_type::sstd_m_class_index;
    }
    static inline const sstd_type_up_cast_map * get_class_up_cast_map() {
        return sstd_super_type::sstd_m_class_up_cast_map;
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
    const static sstd_register_class_information<To> varToInformation/*force create to class information */;
    const static sstd_register_class_information<sstd_this_type_> varFromInformation/*force create from class information*/;
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












