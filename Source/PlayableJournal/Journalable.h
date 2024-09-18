#pragma once
#include "DllExport.h"

#include "Journal.h"
#include "string"
#include "Utils.h"
template<typename T>
struct Journalable
{
};
// =====================================================================================================
// Macro: JOURNALABLE_xxx, that can make class journalable in journal in JS-syntax compliant
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

#define JOURNALABLE_METHOD(Class, Method)                                             \
    void Method()                                                                     \
    {                                                                                 \
	    pj::journal::PLAYABLE(m_instanceName.c_str(), ".", #Method, "();");           \
	    m_##Class.Method();                                                           \
    }

#define JOURNALABLE_CLASS_END(NameSpace, Class)                                       \
private:                                                                              \
	inline static int m_lastId = 0;                                                   \
	std::string m_instanceName;                                                       \
	NameSpace::Class m_##Class;                                                       \
};                           
// =====================================================================================================