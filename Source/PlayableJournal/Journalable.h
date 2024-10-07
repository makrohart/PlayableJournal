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

/// <param name="NameSpace">Namespace of class</param>
/// <param name="Class">Class</param>
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

/// <param name="Property">Property of class</param>
/// <param name="PropertyType">Data type of one property</param>
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

/// <param name="Method">Method of class</param>
/// <param name="ArgType">Data type of one argument of method</param>
/// <param name="Arg">argument of method</param>
#define JOURNALABLE_MMETHOD(Method, ArgType, Arg, ...)                                \
    void Method(                                                                      \
			ArgType Arg                                                               \
			FOR_EACH_2(JOURNALABLE_COMMA_ARGTYPE_ARG, __VA_ARGS__)                    \
		)                                                                             \
    {                                                                                 \
	    pj::journal::PLAYABLE(m_instanceName.c_str(), ".", #Method, "(",              \
			pj::utils::toString(Arg).c_str()                                          \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG_TOSTRING, __VA_ARGS__),                  \
		");");                                                                        \
	    m_object.Method(                                                              \
			Arg                                                                       \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG, __VA_ARGS__)                            \
		);                                                                            \
    }

/// <param name="Method">Method of class</param>
/// <param name="ReturnType">Return type of method return value</param>
/// <param name="ArgType">Data type of one argument of method</param>
/// <param name="Arg">argument of method</param>
#define JOURNALABLE_R_MMETHOD(Method, ReturnType, ArgType, Arg, ...)                  \
    ReturnType Method(                                                                \
			ArgType Arg                                                               \
			FOR_EACH_2(JOURNALABLE_COMMA_ARGTYPE_ARG, __VA_ARGS__)                    \
		)                                                                             \
    {                                                                                 \
	    pj::journal::PLAYABLE("retVal = ", m_instanceName.c_str(), ".", #Method, "(", \
			pj::utils::toString(Arg).c_str()                                          \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG_TOSTRING, __VA_ARGS__),                  \
		");");                                                                        \
	    return m_object.Method(                                                       \
			Arg                                                                       \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG, __VA_ARGS__)                            \
		);                                                                            \
    }

/// <param name="NameSpace">Namespace of class</param>
/// <param name="Class">Class</param>
#define JOURNALABLE_CLASS_END(NameSpace, Class)                                       \
private:                                                                              \
	inline static int m_lastId = 0;                                                   \
	int m_id;                                                                         \
	std::string m_instanceName;                                                       \
	NameSpace::Class m_object;                                                        \
};                          

#define JOURNALABLE_COMMA_ARGTYPE_ARG(ArgType, Arg) , ArgType Arg

#define JOURNALABLE_COMMA_ARG_TOSTRING(ArgType, Arg) , ", ", pj::utils::toString(Arg).c_str()

#define JOURNALABLE_COMMA_ARG(ArgType, Arg) , Arg

// =====================================================================================================

/// <param name="Method">Method of class</param>
/// <param name="ArgType">Data type of one argument of method</param>
/// <param name="Arg">argument of method</param>
#define JOURNALABLE_METHOD(NameSpace, Method, ArgType, Arg, ...)                  \
namespace journalable                                                             \
{                                                                                 \
	void Method(                                                                  \
		ArgType Arg                                                               \
		FOR_EACH_2(JOURNALABLE_COMMA_ARGTYPE_ARG, __VA_ARGS__)                    \
	)                                                                             \
    {                                                                             \
	    pj::journal::PLAYABLE(#Method, "(",                                       \
			pj::utils::toString(Arg).c_str()                                      \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG_TOSTRING, __VA_ARGS__),              \
		");");                                                                    \
	    NameSpace::Method(                                                        \
			Arg                                                                   \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG, __VA_ARGS__)                        \
		);                                                                        \
    };                                                                            \
};

/// <param name="Method">Method of class</param>
/// <param name="ReturnType">Return type of method return value</param>
/// <param name="ArgType">Data type of one argument of method</param>
/// <param name="Arg">argument of method</param>
#define JOURNALABLE_R_METHOD(NameSpace, Method, ReturnType, ArgType, Arg, ...)    \
namespace journalable                                                             \
{                                                                                 \
	ReturnType Method(                                                            \
		ArgType Arg                                                               \
		FOR_EACH_2(JOURNALABLE_COMMA_ARGTYPE_ARG, __VA_ARGS__)                    \
	)                                                                             \
    {                                                                             \
	    pj::journal::PLAYABLE("retVal = ", #Method, "(",                          \
			pj::utils::toString(Arg).c_str()                                      \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG_TOSTRING, __VA_ARGS__),              \
		");");                                                                    \
	    return NameSpace::Method(                                                 \
			Arg                                                                   \
			FOR_EACH_2(JOURNALABLE_COMMA_ARG, __VA_ARGS__)                        \
		);                                                                        \
    };                                                                            \
};