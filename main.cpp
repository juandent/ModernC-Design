//
//  main.cpp
//  ModernC++Design
//
//  Created by Juan Dent on 27/April/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <memory>
#include <iterator>



struct P {
    std::string first, last;
};

template<typename T>
struct OpNewCreator {
    static T* Create() {
        return new T;
    }
    
protected:
    ~OpNewCreator() {}
};

class Person : public OpNewCreator<Person>
{
public:

    Person()=default;
    Person( const std::string& first, const std::string& last)
    : firstName{first}, lastName{last} {}
    
    Person( const Person& p) {
        firstName = p.firstName;
        lastName = p.lastName;
    }
    Person& operator=( const Person& rhs) {
        Person tmp{rhs};
        tmp.Swap(*this);
        return *this;
    }
    
    void Swap( Person& rhs )
    {
        std::swap(firstName, rhs.firstName);
        std::swap(lastName, rhs.lastName);
        
    }

    std::string firstName;
    std::string lastName;
};

template <class T> class Widget
{
public:
    typedef typename std::add_const<T>::type const_T;
    
    void fun() { std::cout << "fun\n"; }
    void boring() { std::cout << "boring\n"; }
};

template<> void Widget<char>::fun()
{
    std::cout << "<char>fun\n";
}

class Thing {
public:
    std::string name;
    int value;
};

/// VERSION 1
namespace v_1 {
    template<class CreationPolicy>
    class ThingManager : public CreationPolicy
    {
    public:
        Thing* Create()  { return CreationPolicy::Create(); }
    };
    
    void useThingManager() {
        typedef ThingManager<OpNewCreator<Thing>> MyThingMgr;
        MyThingMgr mgr{};
        std::unique_ptr<Thing> { mgr.Create()};
    }
}

namespace v_2 {
    
    template< template<class Created> class CreationPolicy>
    class ThingManager : public CreationPolicy<Thing>
    {
        using classToManage = Thing;
    public:
        classToManage* Create()  { return CreationPolicy<classToManage>::Create(); }
    };
    
    void useThingManager() {
        
    }
}

inline namespace v_3 {
    
    template< template<class> class CreationPolicy>
    class ThingManager : public CreationPolicy<Thing>
    {
    public:
        Thing* Create()  { return CreationPolicy<Thing>::Create(); }
    };
    
    using ThingMgr = ThingManager<OpNewCreator>;
    void useThingManager() {
        
    }
}


template <class T, class U>
class Gadget
{
public:
    virtual void fun() { std::cout << "fun\n"; }
};

/*
template<class U> void Gadget<char,U>::fun() {
    std::cout << "<char>fun\n";
}
 */

template<> void Gadget<double,char>::fun() {
    std::cout << "<double,char>fun\n";
}



template<class T>
class Gadget<T, char> {
public:
    virtual void fun() { std::cout << "Gadget<T,char>::fun\n"; }
};

template<class T, class U>
class SpecialGadget : public Gadget<T, U>
{
public:
    virtual void fun() override  { std::cout << "SpecialGadget<T,U>::fun()\n"; }
};

template<typename T>
decltype(auto) constexpr abs(const T& x)
{
    return x < T{} ? -x : x;
}


struct Base {    
    virtual int type() const noexcept {
        return 0;
    }
    virtual Base* clone() const noexcept {
        return new Base(*this);
    }
};

struct SimilarBase
{
    SimilarBase( const Base& base ) {}  // can convert Base to SimilarBase
    /*
    operator Base() const noexcept { // can convert SimilarBase to Base 
        return Base{};
    }
*/
};

struct Derived : public Base {
    virtual int type() const noexcept override {
        return 1;
    }
    virtual Derived* clone() const noexcept override {
        return new Derived(*this);
    }
};

void useConvertible() {

    constexpr bool isConvertible1 = std::is_convertible<Base, SimilarBase>::value;
    constexpr bool isConvertible2 = std::is_convertible<SimilarBase, Base>::value;
    constexpr bool isConvertible3 = std::is_convertible<Derived, Base>::value;
    constexpr bool isConvertible4 = std::is_convertible<Base,Derived>::value;
    
}

void useCovariantReturnTypes() {
    Base* pDerived = new Derived{};
    Base* pCloned = pDerived->clone();
    std::cout << pCloned->type() << std::endl;
    Derived* pDerivedClone = dynamic_cast<Derived*>(pCloned);
    std::cout << pDerivedClone ->type() << std::endl;
    delete pDerived;
    delete pCloned;
}

struct W {
    W()=default;
    template<typename T, typename W = std::enable_if_t<std::is_integral<T>::value, void>>
    static T twice(const T& t)
    {
        return t*T{2};
    }
    
    template<typename T>
    T operator()(const T& t, typename std::enable_if<std::is_integral<T>::value, void>::type* = nullptr)
    {
        return t*T{3};
    }
/*    
    template<typename T, typename W = typename std::enable_if<std::is_integral<T>::value, void>::type*>
    static W thrice(const T& t)
    {
        return t*T{2};
    }
*/
    template<typename T>
    static T forth(const T& t, typename std::enable_if<std::is_integral<T>::value, void>::type* = nullptr)
    {
        return t*T{2};
    }

};

struct T {
    enum { int_t,float_t } m_type;
    template <typename Integer,
    typename = std::enable_if_t<std::is_integral<Integer>::value>>
    T(Integer) : m_type(int_t) {}
#ifdef DUPLICATE    
    template <typename Floating,
    typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
    T(Floating) : m_type(float_t) {} // error: cannot overload
#endif
};

template<class T>
struct TypeDeclarer;

void usePrint();
void useReplaceAndPrint();
void useSuperSubClass();
void printHierarchy();
void useLeaf();
void useGenLinearHierarchy();
void useChuncks();
void useGenScatterHierarchy();

namespace JD {
	void useAssign();
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    auto x = abs(-4);
    auto s = -6;
    
	useGenScatterHierarchy();

	JD::useAssign();

	useChuncks();

	useGenLinearHierarchy();

    useLeaf();
    
    printHierarchy();
    
    useSuperSubClass();
    
    useReplaceAndPrint();
    
    usePrint();
    
    useConvertible();
    
    useCovariantReturnTypes();
  
    
    auto was = W{};
    int res = was(4);
    
    //auto ff = W::forth(55.555);
    auto four = W::forth(60);
//    TypeDeclarer<decltype(four)> four_type;
    
//    auto thw = W::thrice(50);
    
    
    auto tw = W::twice(50);
    
    
    //auto ftw = W::twice(50.55);
    
    auto two = std::make_pair(40, 56.78);
    std::pair<int,double> five {two};
    auto three = std::make_pair(55, 90.55);
    two = three;
    
    using is_iter = std::istream_iterator<int>;
    
    is_iter in{ std::cin };
    is_iter end {};
    
    std::unique_ptr<Person> p { Person::Create()};
    p->firstName = "Juan";
    p->lastName = "Dent";
    
    Person o { "leslie", "hulse" };
    o = *p;
    
    OpNewCreator<Person>* pCreator = p.get();
    // NEXT ERROR:
    //delete pCreator;
    
    ThingMgr mgr{};
    std::unique_ptr<Thing> t { mgr.Create() };
    t->value = 500;
    t->name = "junior";
    
    for( in; in != end; ++in) {
        s = *in;
    }
    x = abs(s);
    
    Widget<char> w{};
    w.fun();
    w.boring();
    
    Gadget<int, char> g{};
    g.fun();
    
    SpecialGadget<int,char> sg{};
    sg.fun();
    
    std::cout << "Hello, World!\n";
    return 0;
}
