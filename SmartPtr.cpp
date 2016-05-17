//
//  SmartPtr.cpp
//  ModernC++Design
//
//  Created by Juan Dent on 28/April/16.
//  Copyright © 2016 Juan Dent. All rights reserved.
//

#include "SmartPtr.hpp"
#include <exception>

template<class T> struct NoChecking
{
    static void Check(T*) {}
};

template<class T> struct EnforceNotNull
{
    class NullPointerException : public std::exception 
    {
        
    };
    static void Check( T* ptr)
    {
        if (! ptr) throw NullPointerException{};
    }
};

template<class T> struct EnforceNotNullWithDefault
{
    static T* GetDefaultValue() {
        return new T{};
    }
    static void Check( T*& ptr)
    {
        if (! ptr) ptr = GetDefaultValue();
    }
};


template <
    class T,
    template<class> class CheckingPolicy,
    template<class> class ThreadingModel
>
class SmartPtr : public CheckingPolicy<T>, ThreadingModel<T> {
    
    T* pointee_;    
public:
    T* operator->() {
        typename ThreadingModel<SmartPtr>::Lock guard(*this);
        CheckingPolicy<T>::Check(pointee_);
        return pointee_;
    }
};