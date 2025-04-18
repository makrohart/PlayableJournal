#pragma once

#include "aspectable/Aspectable.h"
#include "Journal.h"
#include "Journalable.h"
//#include "Playable.h"
#include "reflectable/Reflectable.h"
#include <string>
#include <Utils.h>


namespace testPlayable
{
    class NativeClass : public aspectable::Aspectable<pj::playable::PlayableAspect>
    {
    public:
       void voidMethodInt(int count) 
       {
           m_count = count;
           pj::journal::INFO("test::NativeClass::voidMethodInt: ", pj::utils::toString(count).c_str());
       };

       std::string stringMethodStringInt(const std::string& str, const int count)
       {
           m_count = count;
           std::string retVal;
           for (int ii = count; ii != 0; ii--)
               retVal += str;

           return retVal;
       }

       int getCount() { return m_count; };
       void setCount(const int count) { m_count = count; };

    private:
       int m_count = 0;

       inline static uintptr_t s_reflectable = []() {
           return reflectable::Reflectable::type("NativeClass")
               .inherit(nullptr)
               .constructor<>()
               .method("voidMethod", &NativeClass::voidMethodInt)
               .method("intMethodIntString", &NativeClass::stringMethodStringInt)
               .field("m_count", &NativeClass::m_count)
               .end();
           }();
    };
}

//PLAYABLE_CLASS_BEGIN(testPlayable, NativeClass)
//PLAYABLE_PROPERTIES_BEGIN(1)
//PLAYABLE_PROPERTY(Count, int, getCount, setCount)
//PLAYABLE_PROPERTIES_END
//PLAYABLE_MMETHODS_BEGIN(2)
//pj::playable::PlayableMethod{ 
//    "voidMethodInt", 
//    [](const v8::FunctionCallbackInfo<v8::Value>& args) { 
//        v8::Isolate* pIsolate = args.GetIsolate(); 
//        v8::HandleScope handleScope(pIsolate); 
//        v8::Local<v8::Object> self = args.Holder(); 
//        v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); 
//        void* pNative = native->Value(); 
//        auto function = std::bind(
//            &Type::invoke<Type, decltype(&Type::voidMethodInt), int>,
//            static_cast<Type*>(pNative), 
//            &Type::voidMethodInt , 
//            std::_Ph<1>{}
//        ); 
//        executePlayableMethod<void>(function, pIsolate, args , pj::utils::toNativeFromJS<int>(pIsolate, args[0])); 
//    }, 
//},
//PLAYABLE_MMETHOD(std::string, stringMethodStringInt, std::string, int)
//PLAYABLE_MMETHODS_END
//PLAYABLE_ClASS_END(testPlayable, NativeClass)

//PLAYABLE_CLASS_BEGIN(testPlayable, NativeClass)
//PLAYABLE_PROPERTIES_BEGIN(1)
//PLAYABLE_PROPERTY(Count, int, getCount, setCount)
//PLAYABLE_PROPERTIES_END
//PLAYABLE_MMETHODS_BEGIN(2)
//PLAYABLE_MMETHOD(void, voidMethodInt, int)
//PLAYABLE_MMETHOD(std::string, stringMethodStringInt, std::string, int)
//PLAYABLE_MMETHODS_END
//PLAYABLE_ClASS_END(testPlayable, NativeClass)