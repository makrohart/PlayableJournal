// UnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "PlayableJournal.lib")
#ifdef _DEBUG
#pragma comment(lib, "debug\\v8_monolith.lib")
#else
#pragma comment(lib, "release\\v8_monolith.lib")
#endif

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "winmm.lib")

#include <iostream>

#include "Journalable.h"
#include "Playable.h"
#include "Player.h"

JOURNALABLE_R_METHOD(pj::utils, splitString, std::vector<std::string>, const char*, str, const char, separator)
//PLAYABLE_R_METHOD(pj::utils, splitString, std::vector<std::string>, 0, const char*, 1, const char)

namespace test
{
    void print(int count, std::string message)
    {
        pj::journal::INFO("print: ", message.c_str());
    }

    class testClass
    {
    public:
        void testMethod(int count, std::string message)
        {
            pj::journal::INFO("print: ", message.c_str());
        }

        void setCount(const int count)
        {
            m_count = count;
        }

        int getCount() const
        {
            return m_count;
        }

    private:
        int m_count;
    };
}

JOURNALABLE_METHOD(test, print, int, count, std::string, message)
PLAYABLE_METHOD(void, test, print, int, std::string)

JOURNALABLE_CLASS_BEGIN(test, testClass)
JOURNALABLE_PROPERTY(Count, int)
JOURNALABLE_MMETHOD(testMethod, int, count, std::string, message)
JOURNALABLE_CLASS_END(test, testClass)

//PLAYABLE_CLASS_BEGIN(test, testClass)
//PLAYABLE_PROPERTIES_BEGIN(1)
//PLAYABLE_PROPERTY(Count, int, getCount, setCount)
//PLAYABLE_PROPERTIES_END
//PLAYABLE_MMETHODS_BEGIN(1)
//PLAYABLE_MMETHOD(void, testMethod, int, std::string)
//PLAYABLE_MMETHODS_END
//PLAYABLE_CLASS_END(test, testClass)

struct Playable_testClass {
    using Type = Journalable<test::testClass>; Playable_testClass() {
        std::vector<pj::playable::PlayableMethod> methods; for (int ii = 0; ii != m_methodCount; ++ii) methods.push_back(m_playableMethods[ii]); std::vector<pj::playable::PlayableAccesser> accessers; for (int ii = 0; ii != m_propertyCount; ++ii) accessers.push_back(m_playableProperties[ii]); pj::playable::PlayableManager::getInstance()->add(pj::playable::PlayableClass("testClass", New, methods, accessers));
    } static void New(const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* pIsolate = args.GetIsolate(); Type* ptestClass = new Type(); args.This()->SetInternalField(0, v8::External::New(pIsolate, ptestClass));
    }
private: int m_propertyCount = 1; pj::playable::PlayableAccesser m_playableProperties[1] = {
    pj::playable::PlayableAccesser{ "Count", [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) { v8::Isolate* pIsolate = info.GetIsolate(); v8::Local<v8::Object> self = info.Holder(); v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); void* pNative = native->Value(); auto nativeValue = static_cast<Type*>(pNative)->getCount(); auto value = pj::utils::toJSFromNative(pIsolate, nativeValue); info.GetReturnValue().Set(value); }, [](v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) { v8::Isolate* pIsolate = info.GetIsolate(); v8::Local<v8::Object> self = info.Holder(); v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); void* pNative = native->Value(); auto nativeValue = pj::utils::toNativeFromJS<int>(pIsolate, value); static_cast<Type*>(pNative)->setCount(nativeValue); }, },
};
PLAYABLE_MMETHODS_BEGIN(1)
pj::playable::PlayableMethod{ "testMethod", [](const v8::FunctionCallbackInfo<v8::Value>& args) { v8::Isolate* pIsolate = args.GetIsolate(); v8::HandleScope handleScope(pIsolate); v8::Local<v8::Object> self = args.Holder(); v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); void* pNative = native->Value(); auto function = std::bind(&Type::testMethod, static_cast<Type*>(pNative) , std::_Ph<1>{} , std::_Ph<1 + 1>{}); executePlayableMethod<void>(function, pIsolate, args , pj::utils::toNativeFromJS<int>(pIsolate, args[0]) , pj::utils::toNativeFromJS<std::string>(pIsolate, args[0 + 1])); }, },
PLAYABLE_MMETHODS_END
PLAYABLE_ClASS_END(test, testClass)


//struct Playable_testClass
//{
//    using Type = Journalable<test::testClass>;
//    Playable_testClass()
//    {
//        std::vector<pj::playable::PlayableMethod> methods;
//        for (int ii = 0; ii != m_methodCount; ++ii)
//            methods.push_back(m_playableMethods[ii]);
//        std::vector<pj::playable::PlayableAccesser> accessers;
//        for (int ii = 0; ii != m_propertyCount; ++ii)
//            accessers.push_back(m_playableProperties[ii]);
//        pj::playable::PlayableManager::getInstance()->add(pj::playable::PlayableClass("testClass", New, methods, accessers));
//    }
//    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)
//    {
//        v8::Isolate* pIsolate = args.GetIsolate();
//        Type* ptestClass = new Type();
//        args.This()->SetInternalField(0, v8::External::New(pIsolate, ptestClass));
//    }
//private:
//    int m_propertyCount = 1;
//    pj::playable::PlayableAccesser m_playableProperties[1] = {
//        pj::playable::PlayableAccesser{
//            "Count",
//            [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
//                v8::Isolate* pIsolate = info.GetIsolate();
//                v8::Local<v8::Object> self = info.Holder();
//                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
//                void* pNative = native->Value();
//                auto nativeValue = static_cast<Type*>(pNative)->getCount();
//                auto value = pj::utils::toJSFromNative(pIsolate, nativeValue); info.GetReturnValue().Set(value);
//            },
//            [](v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
//                v8::Isolate* pIsolate = info.GetIsolate();
//                v8::Local<v8::Object> self = info.Holder();
//                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
//                void* pNative = native->Value();
//                auto nativeValue = pj::utils::toNativeFromJS<int>(pIsolate, value);
//                static_cast<Type*>(pNative)->setCount(nativeValue);
//            },
//        },
//    };
//private:
//    int m_methodCount = 1;
//    pj::playable::PlayableMethod m_playableMethods[1] = {
//        pj::playable::PlayableMethod{
//            "testMethod",
//            [](const v8::FunctionCallbackInfo<v8::Value>& args) {
//                v8::Isolate* pIsolate = args.GetIsolate();
//                v8::HandleScope handleScope(pIsolate);
//                v8::Local<v8::Object> self = args.Holder();
//                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
//                void* pNative = native->Value();
//                auto function = std::bind(&Type::testMethod, static_cast<Type*>(pNative), std::placeholders::_1, std::placeholders::_2);
//                executePlayableMethod<void>(function, pIsolate, args, pj::utils::toNativeFromJS<int>(pIsolate, args[0]), pj::utils::toNativeFromJS<std::string>(pIsolate, args[0 + 1]));
//            }, 
//        },
//    };
//} s_Playable_testClass;


PLAYABLE_METHOD(std::vector<std::string>, pj::utils, splitString, const char*, const char)

int main()
{
    pj::player::Player player;
    const char* unitTest = "D:\\Projects\\PlayableJournal\\Debugx64\\Test\\UnitTest\\unitTest.js";
    player.play(unitTest);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
