#pragma once
#include "DllExport.h"

#include "functional"
#include "Journal.h"
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

template<typename T>
struct Journalable;
// =====================================================================================================

/// <param name="NameSpace:">Namespace of class</param>
/// <param name="Class:">Class</param>
#define JOURNALABLE_CLASS_BEGIN(NameSpace, Class)                                     \
template<>                                                                            \
struct Journalable<NameSpace::Class>                                                  \
{                                                                                     \
	Journalable()                                                                     \
	{                                                                                 \
		m_lastId++;                                                                   \
		m_id = m_lastId;                                                              \
		m_instanceName = pj::utils::toLower(#Class) + "_" + std::to_string(m_lastId); \
		pj::journal::PLAYABLE(m_instanceName.c_str(), " = new ", #Class, "()", ";");  \
	}

/// <param name="Property:">Property of class</param>
/// <param name="PropertyType:">Data type of one property</param>
#define JOURNALABLE_PROPERTY(Property, PropertyType)                                                                  \
PropertyType get##Property()                                                                                          \
{                                                                                                                     \
	const std::string propertyName = pj::utils::toLower(#Property) + "_" + std::to_string(m_id);                      \
	pj::journal::PLAYABLE(propertyName.c_str(), " = ", m_instanceName.c_str(), ".", #Property, ";");                  \
	return m_object.get##Property();                                                                                  \
}                                                                                                                     \
void set##Property(const PropertyType& value)                                                                         \
{                                                                                                                     \
	pj::journal::PLAYABLE(m_instanceName.c_str(), ".", #Property, "= ", pj::utils::toString(value).c_str(), ";");     \
	m_object.set##Property(value);                                                                                    \
}

/// <param name="ReturnType:">Return type of method return value</param>
/// <param name="Method:">One global method</param>
/// <param name="ArgType:">One argument of method</param>
/// <param name="...:">Argument types of method</param>
#define JOURNALABLE_MMETHOD(ReturnType, Method, ArgType, ...)                                \
	ReturnType Method(                                                                       \
		ArgType _arg                                                                         \
		FOR_EACH_WITH_STEP(JOURNALABLE_COMMA_ARGTYPE_ARG, STEP_ARG, __arg, __VA_ARGS__)      \
	)                                                                                        \
    {                                                                                        \
	    pj::journal::PLAYABLE(m_instanceName.c_str(), ".", #Method, "(",                     \
			pj::utils::toString(_arg).c_str()                                                \
			FOR_EACH_WITH_STEP(JOURNALABLE_COMMA_ARG_TOSTRING, STEP_ARG, __arg, __VA_ARGS__) \
		, ");");                                                                             \
	    return m_object.Method(                                                              \
			_arg                                                                             \
			FOR_EACH_WITH_STEP(JOURNALABLE_COMMA_ARG, STEP_ARG, __arg, __VA_ARGS__)          \
		);                                                                                   \
    }

/// <param name="NameSpace:">Namespace of class</param>
/// <param name="Class:">Class</param>
#define JOURNALABLE_CLASS_END(NameSpace, Class)                                       \
private:                                                                              \
	inline static int m_lastId = 0;                                                   \
	int m_id;                                                                         \
	std::string m_instanceName;                                                       \
	NameSpace::Class m_object;                                                        \
};                          
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
#define JOURNALABLE_METHOD(ReturnType, NameSpace, Method, ArgType, ...)                              \
namespace journalable                                                                                \
{                                                                                                    \
	ReturnType Method(                                                                               \
		ArgType _arg                                                                                 \
		FOR_EACH_WITH_STEP(JOURNALABLE_COMMA_ARGTYPE_ARG, STEP_ARG, __arg, __VA_ARGS__)              \
	)                                                                                                \
    {                                                                                                \
	    pj::journal::PLAYABLE(#Method, "(",                                                          \
			pj::utils::toString(_arg).c_str()                                                        \
			FOR_EACH_WITH_STEP(JOURNALABLE_COMMA_ARG_TOSTRING, STEP_ARG, __arg, __VA_ARGS__)         \
		, ");");                                                                                     \
	    return NameSpace::Method(                                                                    \
			_arg                                                                                     \
			FOR_EACH_WITH_STEP(JOURNALABLE_COMMA_ARG, STEP_ARG, __arg, __VA_ARGS__)                  \
		);                                                                                           \
    };                                                                                               \
};
// =====================================================================================================

#define JOURNALABLE_COMMA_ARGTYPE_ARG(Arg, ArgType) , ArgType Arg

#define JOURNALABLE_COMMA_ARG_TOSTRING(Arg, ArgType) , ", ", pj::utils::toString(Arg).c_str()

#define JOURNALABLE_COMMA_ARG(Arg, ArgType) , Arg

#define STEP_ARG(Arg) _Arg
// =====================================================================================================