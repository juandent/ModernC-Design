//
//  Typelist.cpp
//  ModernC++Design
//
//  Created by Juan Dent on 11/May/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

//#include "Typelist.hpp"
#include <type_traits>
#include <typeinfo>
#include <iostream>

#include "Typelist.h"

#include "Typelist.hpp"
#include "Conversion.hpp"


using namespace Loki;
using namespace Loki::TL;

typedef TYPELIST_4(char, unsigned char, signed char, int) CharTypes;

static_assert( std::is_same<CharTypes::Tail::Head, unsigned char>::value,"");



static_assert( std::is_same<int, TypeAt<CharTypes,3>::Result>::value,"");

std::type_info* charsRtti[Length<CharTypes>::value];



static_assert( IndexOf<CharTypes, signed char>::value == 2,"");



typedef Append<CharTypes, long>::Result CharPlusLong;

static_assert( std::is_same<CharPlusLong::Head, char>::value, "");
 
static_assert( IndexOf<CharPlusLong, int>::value == 3,"");

//static_assert( IndexOf<typename AppendTL<char, int>::Result, int>::index == 1);
static_assert( IndexOf<Typelist<char, Typelist<int, NullType>>, int>::value == 1,"");

// 1) set T = CharTypes
// 2) get T::Head
// 3) set T = T::Tail
// 4) get T::Head
// 5) set T = T::Tail
//  ...

static_assert( std::is_same<char, CharTypes::Head>::value,"" );



//typedef TYPELIST_4(char, unsigned char, signed char, int) CharTypes;

typedef Erase<CharTypes, signed char>::Result CharMinusSignedChar;

static_assert( IndexOf<CharTypes, signed char>::value != -1,"");
static_assert( IndexOf<CharMinusSignedChar, signed char>::value == -1,"");



typedef TYPELIST_6(char, unsigned char, char, int,int, unsigned char) DuplicateCharTypes;

typedef typename NoDuplicates<DuplicateCharTypes>::Result NoDuplicateCharTypes;


void usePrint() {
    std::cout << "-------------------------------\n";
    PrintList<DuplicateCharTypes>::Print();
    std::cout << "-------------------------------\n";
    PrintList<NoDuplicateCharTypes>::Print();
    std::cout << "-------------------------------\n";
}
 


typedef TYPELIST_7(char, unsigned char, char, int,int, unsigned char,long) IntegralMultipleTypes;

typedef typename ReplaceAll<IntegralMultipleTypes,int,long>::Result IntegralTypesInt_for_long;


void useReplaceAndPrint() {
    std::cout << "-------------------------------\n";
    PrintList<IntegralMultipleTypes>::Print();
    std::cout << "-------------------------------\n";
    PrintList<IntegralTypesInt_for_long>::Print();
    std::cout << "-------------------------------\n";
}



namespace TL {
    
    
    typedef Typelist<char, Typelist<unsigned char, NullType>> CharTypes;




}