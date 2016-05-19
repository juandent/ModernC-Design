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

#include "Typelist.h"
#include "HierarchyGenerators.h"

#include "Typelist.hpp"
#include "Conversion.hpp"

#include <iostream>
#include <string>

using namespace Loki;
using namespace Loki::TL;



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

	Field<0>(rep_int)._value = 45;
	Field<1>(rep_int)._value = 67;
	Field<2>(rep_int)._value = "juan";

    
}

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
			
			*address = ++i;
			std::cout << static_cast<void*>(p) << ", " << address << ": " << *address << std::endl;
		}
	}
};

void useChuncks() {

	Chunck chunck;
	chunck.Init(5, 10);


}
