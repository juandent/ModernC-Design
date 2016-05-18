//
//  Typelist.hpp
//  ModernC++Design
//
//  Created by Juan Dent on 11/May/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

#ifndef Typelist_hpp
#define Typelist_hpp

#include "NullType.h"


#if FALSE
#include <type_traits>
#include <typeinfo>
#include <iostream>


template<class T, class U>
struct TypeList {
    typedef T Head;
    typedef U Tail;
};

struct NullType {};

#define TYPELIST_1(T1)              TypeList<T1, NullType>
#define TYPELIST_2(T1,T2)           TypeList<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1,T2,T3)        TypeList<T1, TYPELIST_2(T2,T3)>
#define TYPELIST_4(T1,T2,T3,T4)        TypeList<T1, TYPELIST_3(T2,T3,T4)>
#define TYPELIST_5(T1,T2,T3,T4,T5)        TypeList<T1, TYPELIST_4(T2,T3,T4,T5)>
#define TYPELIST_6(T1,T2,T3,T4,T5,T6)        TypeList<T1, TYPELIST_5(T2,T3,T4,T5,T6)>
#define TYPELIST_7(T1,T2,T3,T4,T5,T6,T7)        TypeList<T1, TYPELIST_6(T2,T3,T4,T5,T6,T7)>

template<class TList>
struct Length;

template<> struct Length<NullType> {
    enum { value = 0};
};

template<class T1, class T2>
struct Length< TypeList<T1,T2>> {
    enum { value = 1 + Length<T2>::value };
};

#if FALSE
template<class T>
struct Extract;

template<class T, class U, class V>
struct Extract< TypeList<T, TypeList<U, V>>> {
    typedef T Head;
    typedef U NextHead;
};

template<class T, class U>
struct Extract< TypeList<T, TypeList<U, NullType>>> {
    typedef T Head;
    typedef U NextHead;
};
#endif

template<class TList, unsigned int index>
struct TypeAt;


template<class Head, class Tail>
struct TypeAt< TypeList<Head, Tail>, 0> {
    typedef Head Result;
};

template<class Head, class Tail, unsigned int index>
struct TypeAt< TypeList<Head, Tail>, index> {
    typedef typename TypeAt<Tail, index-1>::Result Result;
};


template<class TList, class TypeSearched>
struct IndexOf;

template<class TypeSearched>
struct IndexOf< NullType, TypeSearched>
{
    enum { index = -1 };
};


template<class T, class TypeSearched>
struct IndexOf< TypeList<TypeSearched, T>, TypeSearched>
{
    enum { index = 0 };
};


template<class Head, class Tail, class TypeSearched>
struct IndexOf< TypeList<Head, Tail>, TypeSearched>
{
    enum { temp = IndexOf< Tail, TypeSearched>::index};
    enum { index = temp == -1 ? -1 : 1 + temp };
};


template<class TList1, class T>
struct Append;

template<> 
struct Append<NullType,NullType>
{
    typedef NullType Result;
};

template<class T> 
struct Append<NullType,T>
{
    typedef TypeList<T, NullType>   Result;
};

template<class Head, class Tail> 
struct Append<NullType, TypeList<Head, Tail>>
{
    typedef TypeList<Head, Tail>    Result;
};

template<class Head, class Tail, class T> 
struct Append< TypeList<Head, Tail>, T>
{
    typedef TypeList<Head, typename Append<Tail, T>::Result> Result;
};

template<class TList, class T>
struct Erase;

template<class T>
struct Erase<NullType,T>
{
    typedef NullType Result;
};

template<class T,class Tail>
struct Erase<TypeList<T, Tail>, T>
{
    typedef Tail Result;
};

template<class Head, class Tail,class T>
struct Erase<TypeList<Head, Tail>, T>
{
    typedef TypeList<Head, typename Erase<Tail,T>::Result> Result;
};

template<class TList, class T>
struct EraseAll;

template<class T>
struct EraseAll<NullType,T>
{
    typedef NullType Result;
};

template<class T,class Tail>
struct EraseAll<TypeList<T, Tail>, T>
{
    typedef typename EraseAll<Tail,T>::Result Result; 
};


template<class Head, class Tail,class T>
struct EraseAll<TypeList<Head, Tail>, T>
{
    typedef TypeList<Head, typename EraseAll<Tail,T>::Result> Result;
};

template<class TList>
struct NoDuplicates;

template<>
struct NoDuplicates<NullType>
{
    typedef NullType Result;
};

#define JD_DEFINITION

template<class Head, class Tail>
struct NoDuplicates< TypeList<Head, Tail>>
{
#ifdef JD_DEFINITION
    typedef TypeList<Head, typename NoDuplicates<typename EraseAll<Tail, Head>::Result>::Result> Result;
#else
private:
    typedef typename NoDuplicates<Tail>::Result L1;
    typedef typename Erase<L1, Head>::Result L2;
public:
    typedef TypeList<Head, L2> Result;
#endif
};
#endif


template<class T>
void DefaultPrint()
{
    std::cout << typeid(T).name() << ", " << typeid(T).hash_code() << std::endl;
}

template<class TList>
struct PrintList;


template<>
struct PrintList<Loki::NullType>
{
    static void Print() {
    }
};

template<class Head, class Tail>
struct PrintList<Loki::Typelist<Head,Tail>>
{
    static void Print() {
        DefaultPrint<Head>();
        PrintList<Tail>::Print();
    }
};


#if FALSE
template<class TList, class T, class U>
struct Replace;


template<class T, class U>
struct Replace<NullType,T,U> {
    typedef NullType Result;
};

template<class Tail, class T, class U>
struct Replace<TypeList<T,Tail>, T, U>
{
    typedef TypeList<U, Tail> Result;
};

template<class Head, class Tail, class T, class U>
struct Replace<TypeList<Head,Tail>, T, U>
{
    typedef TypeList<Head, typename Replace<Tail,T,U>::Result> Result;  
};


template<class TList, class T, class U>
struct ReplaceAll;

template<class T, class U>
struct ReplaceAll<NullType, T, U> {
    typedef NullType Result;
};

template<class Tail, class T, class U>
struct ReplaceAll<TypeList<T,Tail>, T, U>
{
    typedef TypeList<U, typename ReplaceAll<Tail,T,U>::Result> Result;
};

template<class Head, class Tail, class T, class U>
struct ReplaceAll<TypeList<Head,Tail>, T, U>
{
    typedef TypeList<Head, typename ReplaceAll<Tail,T,U>::Result> Result;
};
#endif

#endif /* Typelist_hpp */
