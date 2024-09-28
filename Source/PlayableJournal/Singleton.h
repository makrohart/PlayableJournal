#pragma once
#include "DllExport.h"

#include "mutex"

#define DECLARE_SINGLETON_DEFAULT(Class, ...) \
public: \
	PLAYABLEJOURNAL_API static Class* getInstance(); \
private: \
Class() = default; \
~Class() =default; \
Class(const Class&) = delete; \
Class(Class&&) = delete; \
Class& operator=(const Class&) = delete; \
Class& operator=(Class&&) = delete;

#define DECLARE_SINGLETON(Class, ...) \
public: \
	PLAYABLEJOURNAL_API static Class* getInstance(); \
private: \
Class(); \
~Class(); \
Class(const Class&) = delete; \
Class(Class&&) = delete; \
Class& operator=(const Class&) = delete; \
Class& operator=(Class&&) = delete;

#define DEFINE_SINGLETON(NameSpace, Class) \
namespace \
{ \
	std::mutex s_mutex; \
	NameSpace::Class* s_pInstance; \
} \
NameSpace::Class* NameSpace::Class::getInstance() \
{ \
	if (!s_pInstance) \
	{ \
		s_mutex.lock(); \
		if (!s_pInstance) \
		{ \
			s_pInstance = new NameSpace::Class(); \
		} \
		s_mutex.unlock(); \
	} \
	return s_pInstance; \
}
