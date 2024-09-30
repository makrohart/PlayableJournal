#pragma once
#include "DllExport.h"

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
/// <param name="NameSpace">Namespace of class</param>
/// <param name="Class">Class</param>
#define JOURNALABLE_CLASS_BEGIN(NameSpace, Class)                                     \
template<>                                                                            \
struct Journalable<NameSpace::Class>                                                  \
{                                                                                     \
	Journalable()                                                                     \
	{                                                                                 \
		m_lastId++;                                                                   \
		m_instanceName = pj::utils::toLower(#Class) + "_" + std::to_string(m_lastId); \
		pj::journal::PLAYABLE(m_instanceName.c_str(), " = new ", #Class, "()", ";");  \
	}

/// <param name="Method">Method of class</param>
/// <param name="ArgType">Data type of one argument of method</param>
/// <param name="Arg">argument of method</param>
#define JOURNALABLE_METHOD(Method, ArgType, Arg, ...)                                 \
    void Method(                                                                      \
			ArgType Arg                                                               \
			FOR_EACH_2(JOURNALABLE_ARG, __VA_ARGS__)                                  \
		)                                                                             \
    {                                                                                 \
	    pj::journal::PLAYABLE(m_instanceName.c_str(), ".", #Method, "(",              \
			pj::utils::toString(Arg).c_str()                                          \
			FOR_EACH_2(JOURNALABLE_ARG_TO_STRING, __VA_ARGS__),                       \
		");");                                                                        \
	    m_object.Method(                                                              \
			Arg                                                                       \
			FOR_EACH_2(JOURNALABLE_METHOD_ARG, __VA_ARGS__)                           \
		);                                                                            \
    }

/// <param name="NameSpace">Namespace of class</param>
/// <param name="Class">Class</param>
#define JOURNALABLE_CLASS_END(NameSpace, Class)                                       \
private:                                                                              \
	inline static int m_lastId = 0;                                                   \
	std::string m_instanceName;                                                       \
	NameSpace::Class m_object;                                                        \
};                          

#define JOURNALABLE_ARG(ArgType, Arg) , ArgType Arg

#define JOURNALABLE_ARG_TO_STRING(ArgType, Arg) , ", ", pj::utils::toString(Arg).c_str()

#define JOURNALABLE_METHOD_ARG(ArgType, Arg) , Arg
// =====================================================================================================