#include "register_class_information.hpp"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qwidget.h>

class A{
public:
    virtual ~A(){  }
};

class C{
public:
    virtual ~C(){}
};

class P{};

class B : private P , public C, public virtual A {};

class M{
};

template<template< typename ... > class I,typename ... T>
inline static std::size_t size_( const I<T ...> & ){
    return sizeof...(T);
}

#include <iostream>

int main(int argc,char ** argv){

    QApplication varApp{ argc,argv };

   /*this should output 1*/
   std::cout <<  size_ ( std::tr2::bases< B >::type{} ) << std::endl ;

   RegisterClassInformation<A> varRA;
   RegisterClassInformation<B> varRB;
   RegisterClassInformation<M> varRM;
   /*this should output 4*/
   std::cout << "class deepth : " << varRB.get_class_deeth() << std::endl ;
   /*this should output true*/
   std::hash< std::type_index > varHash;
   std::cout <<  "class typeid hash : " << ( varRB.get_class_index().first == varHash( typeid(B) ) ) << std::endl ;
   /*this this not defined ... */
   std::cout << "class name : " << varRB.get_class_index().second.name() << std::endl ;

   const auto & varClassUpCastMap = varRB.get_class_up_cast_map();
   {
        auto varPos = varClassUpCastMap.find( varRA.get_class_index() );
        if( varPos == varClassUpCastMap.end() ){
            std::cout << "???x1" <<std::endl ;
        }else{
            B b ;
            A * a = reinterpret_cast<A *>( (varPos ->second)( & b ) ) ;
            A * a1 = &b;
            /*this should output true...*/
            std::cout << "cast to A : " << (a1 == a) <<std::endl ;
        }
   }

   {
       auto varPos = varClassUpCastMap.find( varRM.get_class_index() );
       if( varPos == varClassUpCastMap.end() ){
            std::cout << "B can not convert to M" <<std::endl ;
       }else{
            std::cout << "???x2" << std::endl ;
       }
   }

    QWidget varWidget ;
    varWidget.show();

    return varApp.exec() ;

}

















