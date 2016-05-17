//
//  Conversion.hpp
//  ModernC++Design
//
//  Created by Juan Dent on 16/May/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

#ifndef Conversion_hpp
#define Conversion_hpp


template<typename T, typename U>
class Conversion {
    typedef char Small;
    class Big { char dummy[2]; };
    static Small Test( const U&);
    static Big Test(...);
    static T MakeT();
public:
    static constexpr int exists = sizeof(Test(MakeT())) == sizeof(Small);  
    //enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
    enum { same_type = false };
};


template<typename T>
class Conversion<T,T>
{
public:
    static constexpr int exists = 1;
    //enum { exists = true };
    enum { same_type = true };
};



struct Base
{
    int _x;
    Base(int x) : _x{x} {}
    virtual void Do() {}
};

struct Derived : Base {
    using Base::Base;
    virtual void Do() override {}
};

struct Other {
    operator Derived() const {
        return Derived(4);
    }
};

#define SUPERSUBCLASS(T,U) \
(Conversion<const U*, const T*>::exists && \
 (Conversion<const U*, const void*>::same_type == false))



////// SELECT
template<bool b, typename A, typename B>
struct Select {
    typedef A Result;
};

template<typename A, typename B>
struct Select<false,A,B> {
    typedef B Result;
};



////// MOST DERIVED

template<typename TList, typename T>
struct MostDerived;


template<typename T>
struct MostDerived<NullType,T>
{
    typedef T Result;
};

template<typename Head, typename Tail, typename T>
struct MostDerived< TypeList<Head, Tail>, T>
{
private:
    typedef typename MostDerived<Tail,T>::Result Candidate;
public:
    typedef typename Select<SUPERSUBCLASS(Candidate, Head),Head,Candidate>::Result Result;
};

////// DerivedToFront

template<typename TList>
struct DerivedToFront;

template<>
struct DerivedToFront<NullType>
{
    typedef NullType Result;
};


template<typename Head, typename Tail>
struct DerivedToFront< TypeList<Head, Tail>>
{
private:
    typedef typename MostDerived<Tail, Head>::Result TheMostDerived;        // TheMostDerived could be Head (in which case Replace should not be called)
    typedef typename Replace<Tail, TheMostDerived, Head>::Result Temp;
    typedef typename DerivedToFront<Temp>::Result L;
public:
    typedef TypeList<TheMostDerived, L> Result;
    
};


template <class TList, template <class> class Unit>
class GenScatterHierarchy;

template <class T1, class T2, template <class> class Unit>
class GenScatterHierarchy<TypeList<T1, T2>, Unit>
: public GenScatterHierarchy<T1, Unit>
, public GenScatterHierarchy<T2, Unit>
{
public:
    typedef TypeList<T1, T2> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase;
    typedef GenScatterHierarchy<T2, Unit> RightBase;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

template <class AtomicType, template <class> class Unit>
class GenScatterHierarchy : public Unit<AtomicType>
{
    typedef Unit<AtomicType> LeftBase;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

template <template <class> class Unit>
class GenScatterHierarchy<NullType, Unit>
{
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

////////////////////////////////////////////////////////////////////////////////
// function template Field
// Accesses a field in an object of a type generated with GenScatterHierarchy
// Invocation (obj is an object of a type H generated with GenScatterHierarchy,
//     T is a type in the typelist used to generate H):
// Field<T>(obj)
// returns a reference to Unit<T>, where Unit is the template used to generate H 
////////////////////////////////////////////////////////////////////////////////

template <class T, class H>
typename H::template Rebind<T>::Result& Field(H& obj)
{
    return obj;
}

template <class T, class H>
const typename H::template Rebind<T>::Result& Field(const H& obj)
{
    return obj;
}

#endif /* Conversion_hpp */
