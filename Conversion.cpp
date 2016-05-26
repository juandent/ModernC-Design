//
//  Conversion.cpp
//  ModernC++Design
//
//  Created by Juan Dent on 16/May/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <string>
#include <cassert>

#include "Typelist.h"
#include "HierarchyGenerators.h"

#include "Typelist.hpp"
#include "Conversion.hpp"

#include <iostream>
#include <string>

using namespace Loki;
using namespace Loki::TL;


namespace JD {

	// TList must have at least 1 type!!

	template< typename TList, const std::type_info** start_address>
	struct AssignInv {
		static void assign() {}
	};

	template<typename Head, const std::type_info** start_address>
	struct AssignInv< Typelist<Head, NullType>, start_address> 
	{
		enum { index = 0 };
		static void assign() {
			start_address[index] = &typeid(Head);
		}
	};

	template<typename Head, typename NextHead, typename Tail, const std::type_info** start_address>
	struct AssignInv< Typelist<Head, Typelist<NextHead, Tail>>, start_address>
	{
		using Base = AssignInv<Typelist<NextHead, Tail>, start_address>;

		enum {
			index = 1 + Base::index
		};

		static void assign() {
			Base::assign();
			start_address[index] = &typeid(Head);
		}
	};

	// TList must have at least 1 type!!

	template< typename TList, const std::type_info** start_address, size_t length>
	struct Assign {
		static void assign() {}
	};

	template<typename Head, const std::type_info** start_address, size_t length>
	struct Assign< Typelist<Head, NullType>, start_address, length>
	{
		enum { index = 1 };
		static void assign() {
			start_address[length - index] = &typeid(Head);
		}
	};

	template<typename Head, typename NextHead, typename Tail, const std::type_info** start_address, size_t length>
	struct Assign< Typelist<Head, Typelist<NextHead, Tail>>, start_address, length>
	{
		using Base = Assign<Typelist<NextHead, Tail>, start_address, length>;

		enum {
			index = 1 + Base::index
		};

		static void assign() {
			Base::assign();
			start_address[length - index] = &typeid(Head);
		}
	};

	typedef ::Loki::Typelist<char, Typelist<short, Typelist<int, Typelist<long, NullType>>>> SignedIntegers;
	//typedef TYPELIST_4(char, short, int, long) SignedIntegers;

	const std::type_info* intsRtti[Length<SignedIntegers>::value];

	// TList must have at least 1 type!!

	template< typename TList, const std::type_info** start_address, size_t index=0>
	struct AssignEff {
		static void assign() {}
	};

	template<typename Head, const std::type_info** start_address, size_t index>
	struct AssignEff< Typelist<Head, NullType>, start_address, index>
	{
		static void assign() {
			start_address[index] = &typeid(Head);
		}
	};

	constexpr const std::type_info** next(const std::type_info** start)
	{
		return start + sizeof(void*);
	}

	constexpr unsigned next(unsigned start)
	{
		return start + sizeof(void*);
	}

	template<typename Head, typename NextHead, typename Tail, const std::type_info** start_address, size_t index>
	struct AssignEff< Typelist<Head, Typelist<NextHead, Tail>>, start_address, index>
	{
		using Base = AssignEff<Typelist<NextHead, Tail>, start_address, index + 1>;

		static void assign() {
			Base::assign();
			start_address[index] = &typeid(Head);
		}
	};

#if FALSE
	namespace UsingClang {

		// TList must have at least 1 type!!

		template< typename TList, const std::type_info** start_address>
		struct AssignEff {
			static void assign() {}
		};

		template<typename Head, const std::type_info** start_address>
		struct AssignEff< Typelist<Head, NullType>, start_address>
		{
			static void assign() {
				*start_address = &typeid(Head);
			}
		};

		constexpr const std::type_info** next(const std::type_info** start)
		{
			return start + sizeof(void*);
		}

		constexpr unsigned next(unsigned start)
		{
			return start + sizeof(void*);
		}

		template<typename Head, typename NextHead, typename Tail, const std::type_info** start_address>
		struct AssignEff< Typelist<Head, Typelist<NextHead, Tail>>, start_address>
		{
			enum { next_address = reinterpret_cast<size_t>( start_address + 1) };

			using Base = AssignEff<Typelist<NextHead, Tail>, next_address>;

			static void assign() {
				Base::assign();
				*start_address = &typeid(Head);
			}
		};
	}
#endif

	void useAssign() {
#if FALSE
		AssignInv<SignedIntegers, intsRtti>::assign();

		for (int i = 0; i < Length<SignedIntegers>::value; ++i) {

			auto x = intsRtti[i];
			std::cout << x->name() << std::endl;
		}

		Assign<SignedIntegers, intsRtti, Length<SignedIntegers>::value>::assign();

		for (int i = 0; i < Length<SignedIntegers>::value; ++i) {

			auto x = intsRtti[i];
			std::cout << x->name() << std::endl;
		}
#endif
#if 1==1
		auto res = next(intsRtti);

		AssignEff<SignedIntegers, intsRtti>::assign();

		for (int i = 0; i < Length<SignedIntegers>::value; ++i) {

			auto x = intsRtti[i];
			std::cout << x->name() << std::endl;
		}
#else
		UsingClang::AssignEff<SignedIntegers, intsRtti>::assign();

		for (int i = 0; i < Length<SignedIntegers>::value; ++i) {

			auto x = intsRtti[i];
			std::cout << x->name() << std::endl;
		}

#endif
	}

#if FALSE
	template<typename Head, typename Tail, const std::type_info* start_address>
	struct AssignValues;


	template<typename Head, typename NextHead, typename Tail, const std::type_info* start_address>
	struct AssignValues<Head, Typelist<NextHead, Tail>>
	{
		using Base = AssignValues<NextHead, Tail>;

		static enum {
			index = 1 + Base::index
		};

		static void assign() {
			Base::assign();
			start_address[index] = &typeid(Head);
		}

	};


	template<typename Head, const std::type_info* start_address>
	struct AssignValues<Head, NullType, start_address> {
		static enum { index = 0};
		static void assign() {
			start_address[index] = &typeid(Head);
		}
	};
#endif
}
/*
template

template<typename Head, NullType>
*/

//template<typename T, typename U>
//using Super_Subclass = Conversion<const U*, const void*>::same_type;


void useSuperSubClass() {
    
    auto other = Other{};
    Derived derived{other};
    
    if (SUPERSUBCLASS(Derived, Other)) {
        std::cout << "yes" << std::endl;
    }
    if ( SUPERSUBCLASS(Base, Derived)) {
        std::cout << "yes: Derived --> Base" << std::endl;
    }
}

namespace JD {
	struct Widget {
		int _number;
	};

	struct Scrollbar : Widget {};

	struct Button : Widget {};

	struct GraphicButton : Button {};

	typedef TYPELIST_4(Widget, Button, Scrollbar, GraphicButton) FourClasses;

	typedef typename DerivedToFront<FourClasses>::Result OrderedClasses;

}

void printHierarchy() {
	using namespace JD;

    std::cout << "-------------------------------\n";
    PrintList<FourClasses>::Print();
    std::cout << "-------------------------------\n";
    PrintList<OrderedClasses>::Print();
    std::cout << "-------------------------------\n";
}


/////// SEVERAL SUPERCLASSES WITH SAME NAME DATA MEMBER
struct A {
    int _value;
};

struct B {
    std::string _value;
};

struct C {
    long _value;
};

struct Leaf : A, B, C {
    
};

///// Accesing subobjects of Leaf

void useLeaf() {
    
    Leaf obj;
    //obj._value = 4;
    static_cast<A&>(obj)._value = 4;
    
    static_cast<B&>(obj)._value = "juan";
    
    static_cast<C&>(obj)._value =56L;
}


/*
 ////////////////////////////////////////////////////////////////////////////////
 // function template Field
 // Accesses a field in an object of a type generated with GenScatterHierarchy
 // Invocation (obj is an object of a type H generated with GenScatterHierarchy,
 //     T is a type in the Typelist used to generate H):
 // Field<T>(obj)
 // returns a reference to Unit<T>, where Unit is the template used to generate H 
 ////////////////////////////////////////////////////////////////////////////////
 
template <class T, class H>
typename H::template Rebind<T>::Result& Field(H& obj)
{
    return obj;
}
 */

template<typename T>
struct Holder {
    T _value;
};

typedef GenScatterHierarchy<JD::FourClasses, Holder> FourHierarchy;

typedef GenScatterHierarchy< TYPELIST_4(int, int, std::string, ::JD::Widget), Holder> RepeatedInt;

void useGenScatterHierarchy() {
 
    FourHierarchy obj;
    
    // shows implementation:
    static_cast<Holder<JD::Widget>&>(obj)._value._number = 89;
    
    // hides implementation:
    Field<JD::Widget>(obj)._value._number = 89;

	RepeatedInt rep_int;

	auto isConst = TypeTraits<decltype(rep_int)>::isConst;

	//Field<0>(rep_int)._value = 45;
#if FALSE
	Field<1>(rep_int)._value = 67;
	Field<2>(rep_int)._value = "juan";
#endif
    
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"

template<class T, class Base>
struct EventHandler : public Base
{
public:
	virtual void OnEvent(T& obj, int eventId)
	{
		obj.Do();
		std::cout << eventId << std::endl;
	}
};

#pragma clang diagnostic pop

struct Window {
	virtual void Do()
	{
		std::cout << "Window\n";
	}
};

struct Button : Window {
	virtual void Do() override
	{
		std::cout << "Button\n";
	}
};

struct Scrollbar : Button {
	virtual void Do()
	{
		std::cout << "Scrollbar\n";
	}
};


typedef GenLinearHierarchy< TYPELIST_3(Window, Button, Scrollbar), EventHandler> WidgetEventHandler;



void useGenLinearHierarchy() {
	WidgetEventHandler handler;

	Scrollbar obj{};

	static_cast<EventHandler<Scrollbar, EmptyType>&>(handler).OnEvent(obj, 10);

	//Field<EventHandler<Scrollbar, EmptyType>>(handler).OnEvent(obj, 12);
	


}

struct Chunck {

	unsigned char* pData_;
	unsigned short firstAvailableBlock, blocksAvailable;
	void Init(std::size_t blockSize, unsigned short blocks) {
		pData_ = new unsigned char[blockSize*blocks];
		memset(pData_, 0, blockSize*blocks);
		firstAvailableBlock = 0;
		blocksAvailable = blocks;
		unsigned short i = 256;
		unsigned char* p = pData_;
		for (; i != blocks; p += blockSize) {
			auto address = reinterpret_cast<unsigned short*>(p);
			assert((void*)address == (void*)p);
			*address = ++i;
			std::cout << static_cast<void*>(p) << ", " << address << ": " << *address << std::endl;
		}
	}
};

void useChuncks() {

	Chunck chunck;
	chunck.Init(5, 10);


}
