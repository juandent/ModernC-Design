//
//  Conversion.cpp
//  ModernC++Design
//
//  Created by Juan Dent on 16/May/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

#include "Typelist.hpp"
#include "Conversion.hpp"
#include <iostream>
#include <string>


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

struct Widget {
    int _number;
};

struct Scrollbar : Widget {};

struct Button : Widget {};

struct GraphicButton : Button {};

typedef TYPELIST_4(Widget, Button, Scrollbar, GraphicButton) FourClasses;

typedef typename DerivedToFront<FourClasses>::Result OrderedClasses;

void printHierarchy() {
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
 //     T is a type in the typelist used to generate H):
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

typedef GenScatterHierarchy<FourClasses, Holder> FourHierarchy;


void useGenScatterHierarchy() {
 
    FourHierarchy obj;
    
    // shows implementation:
    static_cast<Holder<Widget>&>(obj)._value._number = 89;
    
    // hides implementation:
    Field<Widget>(obj)._value._number = 89;
    
}



