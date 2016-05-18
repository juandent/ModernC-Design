//
//  Conversion.hpp
//  ModernC++Design
//
//  Created by Juan Dent on 16/May/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

#ifndef Conversion_hpp
#define Conversion_hpp

#if FALSE
template<typename T, typename U>
class Conversion {
	typedef char Small;
	class Big { char dummy[2]; };
	static Small Test(const U&);
	static Big Test(...);
	static T MakeT();
public:
	enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
	enum { same_type = false };
};


template<typename T>
class Conversion<T, T>
{
public:
	enum { exists = true };
	enum { same_type = true };
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
struct Select<false, A, B> {
	typedef B Result;
};

////////////////////////////////////////////////////////////////////////////////
// class template IsCustomUnsignedInt
// Offers a means to integrate nonstandard built-in unsigned integral types
// (such as unsigned __int64 or unsigned long long int) with the TypeTraits 
//     class template defined below.
// Invocation: IsCustomUnsignedInt<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in unsigned
//     integral type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IsCustomUnsignedInt
{
	enum { value = 0 };
};

////////////////////////////////////////////////////////////////////////////////
// class template IsCustomSignedInt
// Offers a means to integrate nonstandard built-in unsigned integral types
// (such as unsigned __int64 or unsigned long long int) with the TypeTraits 
//     class template defined below.
// Invocation: IsCustomSignedInt<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in signed
//     integral type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IsCustomSignedInt
{
	enum { value = 0 };
};

////////////////////////////////////////////////////////////////////////////////
// class template IsCustomFloat
// Offers a means to integrate nonstandard floating point types with the
//     TypeTraits class template defined below.
// Invocation: IsCustomFloat<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in
//     floating point type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IsCustomFloat
{
	enum { value = 0 };
};

////////////////////////////////////////////////////////////////////////////////
// Helper types for class template TypeTraits defined below
////////////////////////////////////////////////////////////////////////////////

namespace Private
{
	typedef TYPELIST_4(unsigned char, unsigned short int,
		unsigned int, unsigned long int) StdUnsignedInts;
	typedef TYPELIST_4(signed char, short int,
		int, long int) StdSignedInts;
	typedef TYPELIST_3(bool, char, wchar_t) StdOtherInts;
	typedef TYPELIST_3(float, double, long double) StdFloats;
}

////////////////////////////////////////////////////////////////////////////////
// class template TypeTraits
// Figures out various properties of any given type
// Invocations (T is a type):
// a) TypeTraits<T>::isPointer
// returns (at compile time) true if T is a pointer type
// b) TypeTraits<T>::PointeeType
// returns the type to which T points is T is a pointer type, NullType otherwise
// a) TypeTraits<T>::isReference
// returns (at compile time) true if T is a reference type
// b) TypeTraits<T>::ReferredType
// returns the type to which T refers is T is a reference type, NullType
// otherwise
// c) TypeTraits<T>::isMemberPointer
// returns (at compile time) true if T is a pointer to member type
// d) TypeTraits<T>::isStdUnsignedInt
// returns (at compile time) true if T is a standard unsigned integral type
// e) TypeTraits<T>::isStdSignedInt
// returns (at compile time) true if T is a standard signed integral type
// f) TypeTraits<T>::isStdIntegral
// returns (at compile time) true if T is a standard integral type
// g) TypeTraits<T>::isStdFloat
// returns (at compile time) true if T is a standard floating-point type
// h) TypeTraits<T>::isStdArith
// returns (at compile time) true if T is a standard arithmetic type
// i) TypeTraits<T>::isStdFundamental
// returns (at compile time) true if T is a standard fundamental type
// j) TypeTraits<T>::isUnsignedInt
// returns (at compile time) true if T is a unsigned integral type
// k) TypeTraits<T>::isSignedInt
// returns (at compile time) true if T is a signed integral type
// l) TypeTraits<T>::isIntegral
// returns (at compile time) true if T is a integral type
// m) TypeTraits<T>::isFloat
// returns (at compile time) true if T is a floating-point type
// n) TypeTraits<T>::isArith
// returns (at compile time) true if T is a arithmetic type
// o) TypeTraits<T>::isFundamental
// returns (at compile time) true if T is a fundamental type
// p) TypeTraits<T>::ParameterType
// returns the optimal type to be used as a parameter for functions that take Ts
// q) TypeTraits<T>::isConst
// returns (at compile time) true if T is a const-qualified type
// r) TypeTraits<T>::NonConstType
// removes the 'const' qualifier from T, if any
// s) TypeTraits<T>::isVolatile
// returns (at compile time) true if T is a volatile-qualified type
// t) TypeTraits<T>::NonVolatileType
// removes the 'volatile' qualifier from T, if any
// u) TypeTraits<T>::UnqualifiedType
// removes both the 'const' and 'volatile' qualifiers from T, if any
////////////////////////////////////////////////////////////////////////////////

template <typename T>
class TypeTraits
{
private:
	template <class U> struct PointerTraits
	{
		enum { result = false };
		typedef NullType PointeeType;
	};

	template <class U> struct PointerTraits<U*>
	{
		enum { result = true };
		typedef U PointeeType;
	};

	template <class U> struct ReferenceTraits
	{
		enum { result = false };
		typedef U ReferredType;
	};

	template <class U> struct ReferenceTraits<U&>
	{
		enum { result = true };
		typedef U ReferredType;
	};

	template <class U> struct PToMTraits
	{
		enum { result = false };
	};

	template <class U, class V>
	struct PToMTraits<U V::*>
	{
		enum { result = true };
	};

	template <class U> struct UnConst
	{
		typedef U Result;
		enum { isConst = 0 };
	};

	template <class U> struct UnConst<const U>
	{
		typedef U Result;
		enum { isConst = 1 };
	};

	template <class U> struct UnVolatile
	{
		typedef U Result;
		enum { isVolatile = 0 };
	};

	template <class U> struct UnVolatile<volatile U>
	{
		typedef U Result;
		enum { isVolatile = 1 };
	};

public:
	enum { isPointer = PointerTraits<T>::result };
	typedef typename PointerTraits<T>::PointeeType PointeeType;

	enum { isReference = ReferenceTraits<T>::result };
	typedef typename ReferenceTraits<T>::ReferredType ReferredType;

	enum { isMemberPointer = PToMTraits<T>::result };

	enum {
		isStdUnsignedInt =
		TL::IndexOf<Private::StdUnsignedInts, T>::value >= 0
	};
	enum {
		isStdSignedInt =
		TL::IndexOf<Private::StdSignedInts, T>::value >= 0
	};
	enum {
		isStdIntegral = isStdUnsignedInt || isStdSignedInt ||
		TL::IndexOf<Private::StdOtherInts, T>::value >= 0
	};
	enum { isStdFloat = TL::IndexOf<Private::StdFloats, T>::value >= 0 };
	enum { isStdArith = isStdIntegral || isStdFloat };
	enum {
		isStdFundamental = isStdArith || isStdFloat ||
		Conversion<T, void>::sameType
	};

	enum { isUnsignedInt = isStdUnsignedInt || IsCustomUnsignedInt<T>::value };
	enum { isSignedInt = isStdSignedInt || IsCustomSignedInt<T>::value };
	enum { isIntegral = isStdIntegral || isUnsignedInt || isSignedInt };
	enum { isFloat = isStdFloat || IsCustomFloat<T>::value };
	enum { isArith = isIntegral || isFloat };
	enum { isFundamental = isStdFundamental || isArith || isFloat };

	typedef typename Select<isStdArith || isPointer || isMemberPointer,
		T, ReferredType&>::Result
		ParameterType;

	enum { isConst = UnConst<T>::isConst };
	typedef typename UnConst<T>::Result NonConstType;
	enum { isVolatile = UnVolatile<T>::isVolatile };
	typedef typename UnVolatile<T>::Result NonVolatileType;
	typedef typename UnVolatile<typename UnConst<T>::Result>::Result
		UnqualifiedType;
};


class EmptyType {};

#endif


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


#if FALSE

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


////////////////////////////////////////////////////////////////////////////////
// function template TupleUnit
// The building block of tuples 
////////////////////////////////////////////////////////////////////////////////

template <class T>
struct TupleUnit
{
	T value_;
	operator T&() { return value_; }
	operator const T&() const { return value_; }
};

////////////////////////////////////////////////////////////////////////////////
// class template Tuple
// Implements a tuple class that holds a number of values and provides field 
//     access to them via the Field function (below) 
////////////////////////////////////////////////////////////////////////////////

template <class TList>
struct Tuple : public GenScatterHierarchy<TList, TupleUnit>
{
};

////////////////////////////////////////////////////////////////////////////////
// helper class template FieldHelper
// See Field below
////////////////////////////////////////////////////////////////////////////////

template <class H, unsigned int i> struct FieldHelper;

template <class H>
struct FieldHelper<H, 0>
{
	typedef typename H::TList::Head ElementType;
	typedef typename H::template Rebind<ElementType>::Result UnitType;

	enum
	{
		isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
		isConst = TypeTraits<H>::isConst
	};

	typedef const typename H::LeftBase ConstLeftBase;

	typedef typename Select<isConst, ConstLeftBase,
		typename H::LeftBase>::Result LeftBase;

	typedef typename Select<isTuple, ElementType,
		UnitType>::Result UnqualifiedResultType;

	typedef typename Select<isConst, const UnqualifiedResultType,
		UnqualifiedResultType>::Result ResultType;

	static ResultType& Do(H& obj)
	{
		LeftBase& leftBase = obj;
		return leftBase;
	}
};

template <class H, unsigned int i>
struct FieldHelper
{
	typedef typename TypeAt<typename H::TList, i>::Result ElementType;
	typedef typename H::Rebind<ElementType>::Result UnitType;

	enum
	{
		isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
		isConst = TypeTraits<H>::isConst
	};

	typedef const typename H::RightBase ConstRightBase;

	typedef typename Select<isConst, ConstRightBase,
		typename H::RightBase>::Result RightBase;

	typedef typename Select<isTuple, ElementType,
		UnitType>::Result UnqualifiedResultType;

	typedef typename Select<isConst, const UnqualifiedResultType,
		UnqualifiedResultType>::Result ResultType;

	static ResultType& Do(H& obj)
	{
		RightBase& rightBase = obj;
		return FieldHelper<RightBase, i - 1>::Do(rightBase);
	}
};

////////////////////////////////////////////////////////////////////////////////
// function template Field
// Accesses a field in an object of a type generated with GenScatterHierarchy
// Invocation (obj is an object of a type H generated with GenScatterHierarchy,
//     i is the index of a type in the typelist used to generate H):
// Field<i>(obj)
// returns a reference to Unit<T>, where Unit is the template used to generate H
//     and T is the i-th type in the typelist 
////////////////////////////////////////////////////////////////////////////////

template <int i, class H>
typename FieldHelper<H, i>::ResultType&
Field(H& obj)
{
	return FieldHelper<H, i>::Do(obj);
}
#endif

#endif /* Conversion_hpp */
