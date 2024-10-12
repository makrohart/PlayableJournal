#pragma once
#include "DllExport.h"

#include "Journal.h"
#include "JournalEngine.h"
#include "PlayableClass.h"
#include "PlayableManager.h"
#include "PlayableMethod.h"
#include "string"
#include "Utils.h"
#include "ValueUtils.h"
#include "v8.h"

// =====================================================================================================
// ---------------------------------------------- SUMMARY ----------------------------------------------
// Make Class object playable in journal.js. The following bundle of macros are designed to bind C++ 
// native classes with JS classes.
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
//    };
// };
// 
// /*Put the following macros into source file (e.g. NativeClass.cpp)*/
// PLAYABLE_CLASS_BEGIN(test, NativeClass)
//    PLAYABLE_PROPERTIES_BEGIN(1)
//       PLAYABLE_PROPERTY(Count, int, getCount, setCount)
//    PLAYABLE_PROPERTIES_END
//    PLAYABLE_METHODS_BEGIN(1)
//          PLAYABLE_METHOD(HelloWorld, 0, int)
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
struct Playable_##Class##                                                                                                \
{                                                                                                                        \
    using Type = Journalable<NameSpace::Class>;                                                                          \
    Playable_##Class##()                                                                                                 \
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
        Type* p##Class = new Type();                                                                                     \
        args.This()->SetInternalField(0, v8::External::New(pIsolate, p##Class));                                         \
    }                                                                                                                                                                                                     

/// <param name="PropertyCount">Count of properties in class</param>
#define PLAYABLE_PROPERTIES_BEGIN(PropertyCount)                                                                         \
private:                                                                                                                 \
    int m_propertyCount = PropertyCount;                                                                                 \
    pj::playable::PlayableAccesser m_playableProperties[PropertyCount] = {

/// <param name="Property">One property of class</param>
/// <param name="PropertyType">Data type of property</param>
/// <param name="Getter">Getter for the property</param>
/// <param name="Setter">Setter for the property</param>
#define PLAYABLE_PROPERTY(Property, PropertyType, Getter, Setter)                                                         \
        pj::playable::PlayableAccesser {                                                                                  \
            #Property,                                                                                                    \
            [] (v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {                        \
                v8::Isolate* pIsolate = info.GetIsolate();                                                                \
                v8::Local<v8::Object> self = info.Holder();                                                               \
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));                \
                void* pNative = native->Value();                                                                          \
                auto nativeValue = static_cast<Type*>(pNative)->Getter();                                                 \
                auto value = pj::utils::toJSFromNative(pIsolate, nativeValue);                                            \
                info.GetReturnValue().Set(value);                                                                         \
            },                                                                                                            \
            [] (v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) { \
                v8::Isolate* pIsolate = info.GetIsolate();                                                                \
                v8::Local<v8::Object> self = info.Holder();                                                               \
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));                \
                void* pNative = native->Value();                                                                          \
                auto nativeValue = pj::utils::toNativeFromJS<PropertyType>(pIsolate, value);                              \
                static_cast<Type*>(pNative)->Setter(nativeValue);                                                         \
            },                                                                                                            \
        },

#define PLAYABLE_PROPERTIES_END };

/// <param name="MethodCount">Count of methods in class</param>
#define PLAYABLE_MMETHODS_BEGIN(MethodCount)                                                                  \
private:                                                                                                      \
    int m_methodCount = MethodCount;                                                                          \
    pj::playable::PlayableMethod m_playableMethods[MethodCount] = {                                       

/// <param name="Method">One method of class</param>
/// <param name="ArgIndex">Place of one argument of method</param>
/// <param name="ArgType">One argument of method</param>
#define PLAYABLE_MMETHOD(Method, ArgIndex, ArgType, ...)                                                      \
        pj::playable::PlayableMethod {                                                                        \
            #Method,                                                                                          \
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {                                             \
        	    v8::Isolate* pIsolate = args.GetIsolate();                                                    \
	            v8::HandleScope handleScope(pIsolate);                                                        \
	            v8::Local<v8::Object> self = args.Holder();                                                   \
	            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));    \
	            void* pNative = native->Value();                                                              \
	            static_cast<Type*>(pNative)->Method(                                                          \
                    pj::utils::toNativeFromJS<ArgType>(pIsolate, args[ArgIndex])                              \
                    FOR_EACH_2(PLAYABLE_COMMA_NATIVE_ARG, __VA_ARGS__)                                        \
                );                                                                                            \
            },                                                                                                \
        }, 

/// <param name="Method">One method of class</param>
/// <param name="ReturnType">Return type of method return value</param>
/// <param name="ArgIndex">Place of one argument of method</param>
/// <param name="ArgType">One argument of method</param>
#define PLAYABLE_R_MMETHOD(Method, ReturnType, ArgIndex, ArgType, ...)                                        \
        pj::playable::PlayableMethod {                                                                        \
            #Method,                                                                                          \
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {                                             \
        	    v8::Isolate* pIsolate = args.GetIsolate();                                                    \
	            v8::HandleScope handleScope(pIsolate);                                                        \
	            v8::Local<v8::Object> self = args.Holder();                                                   \
	            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));    \
	            void* pNative = native->Value();                                                              \
                auto nativeRetVal = static_cast<Type*>(pNative)->Method(                                      \
                    pj::utils::toNativeFromJS<ArgType>(pIsolate, args[ArgIndex])                              \
                    FOR_EACH_2(PLAYABLE_COMMA_NATIVE_ARG, __VA_ARGS__)                                        \
                );                                                                                            \
                auto retVal = pj::utils::toJSFromNative(pIsolate, nativeRetVal);                              \
                args.GetReturnValue().Set(retVal);                                                            \
            },                                                                                                \
        }, 

#define PLAYABLE_MMETHODS_END };

#define PLAYABLE_ClASS_END(NameSpace, Class)                                                                  \
} s_Playable_##Class##;
// =====================================================================================================

/// <param name="NameSpace">Namespace</param>
/// <param name="Method">One global method</param>
/// <param name="ArgIndex">Place of one argument of method</param>
/// <param name="ArgType">One argument of method</param>
#define PLAYABLE_METHOD(NameSpace, Method, ArgIndex, ArgType, ...)                                            \
struct Playable_##Method##                                                                                    \
{                                                                                                             \
    Playable_##Method##()                                                                                     \
    {                                                                                                         \
        pj::playable::PlayableMethod method {                                                                 \
            #Method,                                                                                          \
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {                                             \
        	    v8::Isolate* pIsolate = args.GetIsolate();                                                    \
	            v8::HandleScope handleScope(pIsolate);                                                        \
                journalable::Method(                                                                          \
                    pj::utils::toNativeFromJS<ArgType>(pIsolate, args[ArgIndex])                              \
                    FOR_EACH_2(PLAYABLE_COMMA_NATIVE_ARG, __VA_ARGS__)                                        \
                );                                                                                            \
            }                                                                                                 \
        };                                                                                                    \
        pj::playable::PlayableManager::getInstance()->add(method);                                            \
    }                                                                                                         \
} s_Playable_##Method##;

/// <param name="NameSpace">Namespace</param>
/// <param name="Method">One global method</param>
/// <param name="ReturnType">Return type of method return value</param>
/// <param name="ArgIndex">Place of one argument of method</param>
/// <param name="ArgType">One argument of method</param>
#define PLAYABLE_R_METHOD(NameSpace, Method, ReturnType, ArgIndex, ArgType, ...)                              \
struct Playable_##Method##                                                                                    \
{                                                                                                             \
    Playable_##Method##()                                                                                     \
    {                                                                                                         \
        pj::playable::PlayableMethod method {                                                                 \
            #Method,                                                                                          \
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {                                             \
        	    v8::Isolate* pIsolate = args.GetIsolate();                                                    \
	            v8::HandleScope handleScope(pIsolate);                                                        \
                auto nativeRetVal = journalable::Method(                                                      \
                    pj::utils::toNativeFromJS<ArgType>(pIsolate, args[ArgIndex])                              \
                    FOR_EACH_2(PLAYABLE_COMMA_NATIVE_ARG, __VA_ARGS__)                                        \
                );                                                                                            \
                auto retVal = pj::utils::toJSFromNative(pIsolate, nativeRetVal);                              \
                args.GetReturnValue().Set(retVal);                                                            \
            }                                                                                                 \
        };                                                                                                    \
        pj::playable::PlayableManager::getInstance()->add(method);                                            \
    }                                                                                                         \
} s_Playable_##Method##;
// =====================================================================================================

/// <param name="ArgIndex">Place of one argument of method</param>
/// <param name="ArgType">One argument of method</param>
#define PLAYABLE_COMMA_NATIVE_ARG(ArgIndex, ArgType) , pj::utils::toNativeFromJS<ArgType>(pIsolate, args[ArgIndex])
// =====================================================================================================


template<typename R, typename F, typename... Args>
void executePlayableMethod(F func, v8::Isolate* pIsolate, const v8::FunctionCallbackInfo<v8::Value>& info, Args... args)
{
    v8::HandleScope handleScope(pIsolate);
    auto nativeRetVal = func(args);
    auto retVal = pj::utils::toJSFromNative(pIsolate, nativeRetVal);
    info.GetReturnValue().Set(retVal);
}

template<typename R, typename F, typename... Args>
    requires std::is_void_v<R>
void executePlayableMethod(F func, v8::Isolate* /*pIsolate*/, const v8::FunctionCallbackInfo<v8::Value>& /*info*/, Args... args)
{
    func(args);
}

//template<typename R, typename F>
//void executeMethod(F func)
//{
//    R a = func();
//}
//
//template<typename R, typename F>
//    requires std::is_void_v<R>
//void executeMethod(F func)
//{
//    func();
//}
//
//namespace
//{
//    void print()
//    {
//
//    }
//
//    void test()
//    {
//        executeMethod<void>(print);
//    }
//}
// =====================================================================================================

/// <param name="ReturnType">Return type of method return value</param>
/// <param name="NameSpace">Namespace</param>
/// <param name="Method">One global method</param>
/// <param name="ArgType">One argument of method</param>
#define PLAYABLE_METHOD(ReturnType, NameSpace, Method, ArgType, ...)                                          \
struct Playable_##Method##                                                                                    \
{                                                                                                             \
    Playable_##Method##()                                                                                     \
    {                                                                                                         \
        pj::playable::PlayableMethod method {                                                                 \
            #Method,                                                                                          \
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {                                             \
        	    v8::Isolate* pIsolate = args.GetIsolate();                                                    \
	            v8::HandleScope handleScope(pIsolate);                                                        \
                if (IS_VOID(ReturnType))                                                                      \
                {                                                                                             \
                    journalable::Method(                                                                      \
                        pj::utils::toNativeFromJS<ArgType>(pIsolate, args[0])                                 \
                        FOR_EACH_WITH_STEP(PLAYABLE_COMMA_NATIVE_ARG, STEP_1, 1, __VA_ARGS__)                 \
                    );                                                                                        \
                }                                                                                             \
                else                                                                                          \
                {                                                                                             \
                    auto nativeRetVal = journalable::Method(                                                  \
                        pj::utils::toNativeFromJS<ArgType>(pIsolate, args[0])                                 \
                        FOR_EACH_WITH_STEP(PLAYABLE_COMMA_NATIVE_ARG, STEP_1, 1, __VA_ARGS__)                 \
                    );                                                                                        \
                    auto retVal = pj::utils::toJSFromNative(pIsolate, nativeRetVal);                          \
                    args.GetReturnValue().Set(retVal);                                                        \
                }                                                                                             \
            }                                                                                                 \
        };                                                                                                    \
        pj::playable::PlayableManager::getInstance()->add(method);                                            \
    }                                                                                                         \
} s_Playable_##Method##;