#ifndef __SMART_POINTER_SMART_PTR_HPP
#define __SMART_POINTER_SMART_PTR_HPP




// *******************************************************************************
// Release algorithm for plain pointer.
// Using this release algorithm CSmartHandle works
// like simple smart pointer.

template<typename T, typename PointedBy>
struct CClosePointer
{
    void Close(T handle)
    {
        delete (PointedBy*)handle;
    }

protected:
    ~CClosePointer()
    {
    }
};

// I want to make typedef for using CSmartHandle with CClosePointer policy.
// However, I cannot do this because user needs to provide class name parameter.
// I can use define:
// #define CAUTOPOINTER(ClassName) CSmartHandle<ClassName*, CClosePointer, ClassName>
// Client code is:
// CAUTOPOINTER(CTestClass) pTestClass = new CTestClass();
//
// but more elegant solution is constructing new type:

template<typename T>
struct CAutoPointer
{
    typedef CSmartHandle<T*, CClosePointer, T> AutoPtr;
};

// Client code is:
// CAutoPointer<CTestClass>::AutoPtr pTestClass = new CTestClass();
//
// the struct CAutoPointer itself has no members apart from the type definition 
// and isn't instantiated, it's only used to provide the type member 
// which returns the type I need.


// *******************************************************************************
// Release algorithm for plain array.

template<typename T, typename PointedBy>
struct CCloseArrayPointer
{
    void Close(T handle)
    {
        delete[] (PointedBy*)handle;
    }

protected:
    ~CCloseArrayPointer()
    {
    }
};


//#define CAUTOARRAYPOINTER(ClassName) CSmartHandle<ClassName*, CCloseArrayPointer, ClassName>

template<typename T>
struct CAutoArrayPointer
{
    typedef CSmartHandle<T*, CCloseArrayPointer, T> AutoPtr;
};
