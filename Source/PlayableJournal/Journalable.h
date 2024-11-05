#pragma once
#include "DllExport.h"

#include "functional"
#include "Journal.h"
#include "PlayableAspect.h"
#include "string"
#include "Utils.h"

// =====================================================================================================
// ---------------------------------------------- SUMMARY ----------------------------------------------
// Make Class object journable in journal.js. The macros JOURNALANLE_XXX will implement a specialied Class
// Journalable<T>
// 
// ----------------------------------------------- USAGE -----------------------------------------------
// Use macro to  implement journalable class and its member methods. This methods invocation will be 
// recorded as js-syntax compliant code.
// =====================================================================================================

/// <param name="NameSpace:">Namespace of class</param>
/// <param name="Class:">Class</param>
#define JOURNALABLE_CLASS_BEGIN(NameSpace, Class) \
ASPECT_CLASS_BEGIN(pj::playable::PlayableAspect, NameSpace::Class)

/// <param name="Property:">Property of class</param>
/// <param name="PropertyType:">Data type of one property</param>
#define JOURNALABLE_PROPERTY(Property, PropertyType) \
ASPECT_MMETHOD(PropertyType, get##Property)               \
ASPECT_MMETHOD(void, set##Property, PropertyType)

/// <param name="ReturnType:">Return type of method return value</param>
/// <param name="Method:">One global method</param>
/// <param name="ArgType:">One argument of method</param>
/// <param name="...:">Argument types of method</param>
#define JOURNALABLE_MMETHOD(ReturnType, Method, ...)                                \
ASPECT_MMETHOD(ReturnType, Method, __VA_ARGS__)

/// <param name="NameSpace:">Namespace of class</param>
/// <param name="Class:">Class</param>
#define JOURNALABLE_CLASS_END(NameSpace, Class) ASPECT_CLASS_END           
// =====================================================================================================

// ---------------------------------------------- SUMMARY ----------------------------------------------
// Make global function journable in journal.js. The macros JOURNALANLE_XXX will implement a specialied 
// function under namespace of journalable
// 
// ----------------------------------------------- USAGE -----------------------------------------------
// Use macro to  implement journalable class and its member methods. This methods invocation will be 
// recorded as js-syntax compliant code.
// =====================================================================================================

/// <param name="ReturnType:">Return type of method return value</param>
/// <param name="NameSpace:">Namespace</param>
/// <param name="Method:">One global method</param>
/// <param name="ArgType:">One argument of method</param>
/// <param name="...:">Argument types of method</param>
#define JOURNALABLE_METHOD(ReturnType, NameSpace, Method, ...)                              \
ASPECT_METHOD(pj::playable::PlayableAspect, ReturnType, NameSpace, Method, __VA_ARGS__)