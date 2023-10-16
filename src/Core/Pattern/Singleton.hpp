#pragma once
#include <iostream>
#include <cassert>
/*
 * Ensure a class has only one instance, and provide a global point of access to it.
 * 1. Contructor must be private.
 * 2. Instance is the static member of current class
 * https://github.com/cginternals/globjects/blob/master/source/globjects/source/base/Singleton.h
 * https://github.com/cginternals/globjects/blob/master/source/globjects/source/base/Singleton.inl
 */
template<class T>
class Singleton
{
public:
    static T* instance();

protected:
    explicit Singleton();
    virtual ~Singleton();

protected:
    static T* s_instance;
};

template<class T>
T* Singleton<T>::s_instance(nullptr);

template<class T>
T* Singleton<T>::instance()
{
    if (!s_instance)
    {
        s_instance = new T();
    }

    return s_instance;
}

template<class T>
Singleton<T>::Singleton()
{
    assert(!s_instance);
}

template<class T>
Singleton<T>::~Singleton()
{
    assert(s_instance);

    delete s_instance;
}