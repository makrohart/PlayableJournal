#pragma once
#include "DllExport.h"

#include "Journal.h"
#include "JournalEngine.h"
#include "PlayableClass.h"
#include "PlayableManager.h"
#include "PlayableMethod.h"
#include "string"
#include "Utils.h"
#include "v8.h"

// =====================================================================================================
// ---------------------------------------------- SUMMARY ----------------------------------------------
// Make Class object playable in journal.js. The macros PLAYABLE_XXX will implement a binding between js and c++ class.
// 
// ----------------------------------------------- USAGE -----------------------------------------------
// /*Given a C++ class:*/
// namespace test
// {
//    class NativeClass
//    {
//    public:
//       NativeClass() {};
//       void HelloWorld(int count) { /* do things*/ };
//       int getCount() { return m_count; };
//       void setCount(const int count) { m_count = count; };
//    private:
//       int m_count;
//    }
// }
// 
// /*Put the following macros into source file (e.g. NativeClass.cpp)*/
// PLAYABLE_CLASS_BEGIN(test, NativeClass)
//    PLAYABLE_PROPERTIES_BEGIN(1)
//       PLAYABLE_PROPERTY(test, NativeClass, Count, int, getCount, setCount)
//    PLAYABLE_PROPERTIES_END
//    PLAYABLE_METHODS_BEGIN(1)
//       PLAYABLE_METHOD_BEGIN(test, NativeClass, HelloWorld)
//          /*If there is multiple args, all PLAYABLE_METHOD_ARG but the last one need extra comma in the end.*/
//          PLAYABLE_METHOD_ARG(int)
//       PLAYABLE_METHOD_END
//    PLAYABLE_METHODS_END
// PLAYABLE_CLASS_END(test, NativeClass)
// 
// /*Then, native class is binded with js class. In .js file, we can call:*/
// jsClass = new NativeClass();
// jsClass.HelloWorld();
// jsClass.Count;
// =====================================================================================================

/// <param name="NameSpace">Namespace of class</param>
/// <param name="Class">Class</param>
#define PLAYABLE_CLASS_BEGIN(NameSpace, Class)                                                                           \
struct Playable_##NameSpace##_##Class##                                                                                  \
{                                                                                                                        \
    Playable_##NameSpace##_##Class##()                                                                                   \
    {                                                                                                                    \
        std::vector<pj::playable::PlayableMethod> methods;                                                               \
        for (int ii = 0; ii != m_methodCount; ++ii)                                                                      \
            methods.push_back(m_playableMethods[ii]);                                                                    \
        std::vector<pj::playable::PlayableAccesser> accessers;                                                           \
        for (int ii = 0; ii != m_propertyCount; ++ii)                                                                    \
            accessers.push_back(m_playableProperties[ii]);                                                               \
        pj::playable::PlayableManager::getInstance()->add(pj::playable::PlayableClass(#Class, New, methods, accessers)); \
    }                                                                                                                    \
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)                                                     \
    {                                                                                                                    \
        v8::Isolate* pIsolate = args.GetIsolate();                                                                       \
        NameSpace::Class* p##Class = new NameSpace::Class();                                                             \
        args.This()->SetInternalField(0, v8::External::New(pIsolate, p##Class));                                         \
    }

/// <param name="PropertyCount">Count of properties in class</param>
#define PLAYABLE_PROPERTIES_BEGIN(PropertyCount)                                                                         \
private:                                                                                                                 \
    int m_propertyCount = PropertyCount;                                                                                 \
    pj::playable::PlayableAccesser m_playableProperties[PropertyCount] = {

/// <param name="NameSpace">Namespace of class</param>
/// <param name="Class">Class</param>
/// <param name="Property">One property of class</param>
/// <param name="Type">Data type of property</param>
/// <param name="Getter">Getter for the property</param>
/// <param name="Setter">Setter for the property</param>
#define PLAYABLE_PROPERTY(NameSpace, Class, Property, Type, Getter, Setter)                                               \
        pj::playable::PlayableAccesser {                                                                                  \
            #Property,                                                                                                    \
            [] (v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {                        \
                v8::Local<v8::Object> self = info.Holder();                                                               \
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));                \
                void* pNative = native->Value();                                                                          \
                auto nativeValue = static_cast<NameSpace::Class*>(pNative)->Getter();                                     \
                auto value = propertyNative2JS<Type>(nativeValue, info);                                                  \
                info.GetReturnValue().Set(value);                                                                         \
            },                                                                                                            \
            [] (v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) { \
                v8::Local<v8::Object> self = info.Holder();                                                               \
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));                \
                void* pNative = native->Value();                                                                          \
                auto nativeValue = propertyJS2Native<Type>(value, info);                                                  \
                static_cast<command::Command*>(pNative)->setRepeatCount(nativeValue);                                     \
            },                                                                                                            \
        },

#define PLAYABLE_PROPERTIES_END };

/// <param name="MethodCount">Count of methods in class</param>
#define PLAYABLE_METHODS_BEGIN(MethodCount)                                                                   \
private:                                                                                                      \
    int m_methodCount = MethodCount;                                                                          \
    pj::playable::PlayableMethod m_playableMethods[MethodCount] = {                                       

/// <param name="NameSpace">Namespace of class</param>
/// <param name="Class">Class</param>
/// <param name="Method">One method of class</param>
#define PLAYABLE_METHOD_BEGIN(NameSpace, Class, Method)                                                       \
        pj::playable::PlayableMethod {                                                                        \
            #Method,                                                                                          \
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {                                             \
        	    v8::Isolate* pIsolate = args.GetIsolate();                                                    \
	            v8::HandleScope handleScope(pIsolate);                                                        \
	            v8::Local<v8::Object> self = args.Holder();                                                   \
	            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));    \
	            void* pNative = native->Value();                                                              \
	            static_cast<NameSpace::Class*>(pNative)->Method(

#define PLAYABLE_METHOD_ARG(Index, Type) argJS2Native<Type>(args, Index)

#define PLAYABLE_METHOD_END                                                                                   \
                );                                                                                            \
            },                                                                                                \
        },

#define PLAYABLE_METHODS_END                                                                                  \
    };  

#define PLAYABLE_ClASS_END(NameSpace, Class)                                                              \
} s_Playable_##NameSpace##_##Class##;
// =====================================================================================================

template<typename T>
T argJS2Native(const v8::FunctionCallbackInfo<v8::Value>& args, int index);

template<>
int argJS2Native<int>(const v8::FunctionCallbackInfo<v8::Value>& args, int index)
{
    return args[index].As<v8::Int32>()->Value();
}

template<>
std::string argJS2Native<std::string>(const v8::FunctionCallbackInfo<v8::Value>& args, int index)
{
    v8::Local<v8::String> v8Str = args[index].As<v8::String>();
    v8::Isolate* pIsolate = args.GetIsolate();
    const int size = v8Str->Utf8Length(pIsolate);
    char* pStr = new char[size + 1] {0};
    v8Str->WriteUtf8(pIsolate, pStr, size);
    std::string str = std::string(pStr);
    delete pStr;
    return str;
}

template<typename T>
T propertyJS2Native(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

template<>
int propertyJS2Native<int>(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    return value.As<v8::Int32>()->Value();
}

template<>
std::string propertyJS2Native<std::string>(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Local<v8::String> v8Str = value.As<v8::String>();
    v8::Isolate* pIsolate = info.GetIsolate();
    const int size = v8Str->Utf8Length(pIsolate);
    char* pStr = new char[size + 1] {0};
    v8Str->WriteUtf8(pIsolate, pStr, size);
    std::string str = std::string(pStr);
    delete pStr;
    return str;
}

template<typename T>
v8::Local<v8::Value> propertyNative2JS(const T& value, const v8::PropertyCallbackInfo<v8::Value>& info);

template<>
v8::Local<v8::Value> propertyNative2JS<int>(const int& value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Isolate* pIsolate = info.GetIsolate();
    v8::Local<v8::Value> v8Value = v8::Number::New(pIsolate, value);
    return v8Value;
}