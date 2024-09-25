#pragma once
#include "DllExport.h"

#include "Journal.h"
#include "JournalEngine.h"
#include "PlayableClass.h"
#include "PlayableItems.h"
#include "PlayableMethod.h"
#include "string"
#include "Utils.h"
#include "v8.h"
// =====================================================================================================
// Macro: PLAYABLE_xxx, that can make class playable in journal in JS-syntax compliant
#define PLAYABLE_CLASS_BEGIN(NameSpace, Class, MethodCount)                                               \
struct Playable_##NameSpace##_##Class##                                                                   \
{                                                                                                         \
    Playable_##NameSpace##_##Class##()                                                                    \
    {                                                                                                     \
        std::vector<pj::playable::PlayableMethod> methods;                                                \
        for (int ii = 0; ii != MethodCount; ++ii)                                                         \
            methods.push_back(m_playableMethods[ii]);                                                     \
        pj::playable::PlayableItems::add(pj::playable::PlayableClass(std::string(#Class), New, methods)); \
    }                                                                                                     \
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)                                      \
    {                                                                                                     \
        v8::Isolate* pIsolate = args.GetIsolate();                                                        \
        Journalable<NameSpace::Class>* p##Class = new Journalable<NameSpace::Class>();                    \
        args.This()->SetInternalField(0, v8::External::New(pIsolate, p##Class));                          \
    }                                                                                                     \
private:                                                                                                  \
    pj::playable::PlayableMethod m_playableMethods[MethodCount] = {                                       

#define PLAYABLE_METHOD(NameSpace, Class, Method)                                                         \
        pj::playable::PlayableMethod { #Method, [](const v8::FunctionCallbackInfo<v8::Value>& args) {     \
        	v8::Isolate* pIsolate = args.GetIsolate();                                                    \
	        v8::HandleScope handleScope(pIsolate);                                                        \
	        v8::Local<v8::Object> self = args.Holder();                                                   \
	        v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));    \
	        void* pNative = native->Value();                                                              \
	        static_cast<Journalable<NameSpace::Class>*>(pNative)->Method();                               \
        } },

#define PLAYABLE_ClASS_END(NameSpace, Class)                                                              \
    };                                                                                                    \
} s_Playable_##NameSpace##_##Class##;
// =====================================================================================================

#define PLAYABLE_FIELD(NameSpace, Class, FieldName, FieldType, Getter, Setter)                             \
void Get##FieldName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)       \
{                                                                                                          \
    v8::Local<v8::Object> self = info.Holder();                                                            \
    v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));             \
    void* pNative = native->Value();                                                                       \
    FieldType value = static_cast<Journalable<NameSpace::Class>*>(pNative)->Getter();                      \
    info.GetReturnValue().Set(value);                                                                      \
}