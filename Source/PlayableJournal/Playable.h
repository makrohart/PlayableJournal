#pragma once
#include "DllExport.h"

#include "Journal.h"
#include "JournalEngine.h"
#include "string"
#include "v8.h"

// =====================================================================================================
// https://www.scs.stanford.edu/~dm/blog/va-opt.html#recursive-macros
// This will re-scan macros 342 times, EXPAND4 gets called 256 times, 
// but the intermediary macros cause rescan as well.
#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(Macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(Macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(Macro, Arg, ...)                         \
  Macro(Arg)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (Macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define FOR_EACH_2_FIXED_ARGS(Macro, FArg1, FArg2, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_2_FIXED_ARGS_HELPER(Macro, FArg1, FArg2, __VA_ARGS__)))
#define FOR_EACH_2_FIXED_ARGS_HELPER(Macro, FArg1, FArg2, Arg, ...)                         \
  Macro(FArg1, FArg2, Arg)                                                     \
  __VA_OPT__(FOR_EACH_2_FIXED_ARGS_AGAIN PARENS (Macro, FArg1, FArg2, __VA_ARGS__))
#define FOR_EACH_2_FIXED_ARGS_AGAIN() FOR_EACH_2_FIXED_ARGS_HELPER
// =====================================================================================================

// =====================================================================================================
// Macro: PLAYABLE_OBJECT, that can be playback in journal
#define PLAYABLE_INITS(NameSpace, Class, ...) \
static void Init(v8::Local<v8::ObjectTemplate> global) \
{ \
	v8::Isolate* pIsolate = v8::Isolate::GetCurrent(); \
	v8::Local<v8::FunctionTemplate> _##Class##Template = v8::FunctionTemplate::New(pIsolate, Playable_##NameSpace##_##Class##::New); \
	_##Class##Template->SetClassName(v8::String::NewFromUtf8(pIsolate, #Class).ToLocalChecked()); \
	global->Set(pIsolate, #Class, _##Class##Template); \
	v8::Local<v8::ObjectTemplate> _##Class##_Prototype = _##Class##Template->PrototypeTemplate(); \
	FOR_EACH_2_FIXED_ARGS(PLAYABLE_INIT, NameSpace, Class, ##__VA_ARGS__) \
	v8::Local<v8::ObjectTemplate> _##Class##_Instance = _##Class##Template->InstanceTemplate(); \
	_##Class##_Instance->SetInternalFieldCount(1); \
}
#define PLAYABLE_INIT(NameSpace, Class, Method, ...) \
_##Class##_Prototype->Set(pIsolate, #Method, v8::FunctionTemplate::New(pIsolate, Playable_##NameSpace##_##Class##::Method));

#define PLAYABLE_METHODS(NameSpace, Class, ...) FOR_EACH_2_FIXED_ARGS(PLAYABLE_METHOD, NameSpace, Class, __VA_ARGS__)
#define PLAYABLE_METHOD(NameSpace, Class, Method, ...) \
static void Method(const v8::FunctionCallbackInfo<v8::Value>& args) \
{ \
	v8::Isolate* pIsolate = args.GetIsolate(); \
	v8::HandleScope handleScope(pIsolate); \
	v8::Local<v8::Object> self = args.Holder(); \
	v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); \
	void* pNative = native->Value(); \
	static_cast<Playable<NameSpace::Class>*>(pNative)->Method(); \
}

#define PLAYABLE_OBJECT(NameSpace, Class, ...) \
struct Playable_##NameSpace##_##Class## \
{ \
	Playable_##NameSpace##_##Class##() { Journal::JS2Native::add(Playable_##NameSpace##_##Class##::Init); } \
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args) \
	{ \
		v8::Isolate* pIsolate = args.GetIsolate(); \
		Playable<NameSpace::Class>* p##Class = new Playable<NameSpace::Class>(); \
		args.This()->SetInternalField(0, v8::External::New(pIsolate, p##Class)); \
	} \
	PLAYABLE_INITS(NameSpace, Class, ##__VA_ARGS__) \
	PLAYABLE_METHODS(NameSpace, Class, ##__VA_ARGS__) \
} s_Playable_##NameSpace##_##Class##;
// =====================================================================================================

// =====================================================================================================
// Macro: JOURNALABLE_xxx, that can make class journalable in journal in JS-syntax compliant
#define JOURNALABLE_BEGIN(NameSpace, Class, Interface) \
template<typename T> \
requires std::derived_from<T, NameSpace::Interface> \
class Playable : public NameSpace::Interface \
{ \
public: \
	Playable() \
	{ \
		m_lastId++; \
		m_instanceName = toLower(#Class) + "_" + std::to_string(m_lastId); \
		pj::journal::PLAYABLE(m_instanceName.c_str(), " = new ", #Class, "()", ";"); \
	}

#define JOURNALABLE_VIRTUAL_METHOD(Method) \
virtual void Method() override \
{ \
	pj::journal::PLAYABLE(m_instanceName.c_str(), ".", #Method, "();"); \
	m_t.Method(); \
}

#define JOURNALABLE_END() \
private: \
	inline static int m_lastId = 0; \
	std::string m_instanceName; \
	T m_t; \
};
// =====================================================================================================