#pragma once

#include <string>
#include "aspectable/Aspectable.h"
#include "reflectable/Reflectable.h"
#include "PlayableAspect.h"

struct A : public aspectable::Aspectable<pj::playable::PlayableAspect>
{
    A() {}
    void voidMethod() {
        double a = 12;
        a;
    }

    int getId()
    {
        return 0;
    }

    int intMethodIntString(const int, const std::string&) { return 1; }

    int m_intField;
    std::string m_stringField;

    inline static uintptr_t s_reflectable = []() {
        return reflectable::Reflectable::type("A")
            .inherit(nullptr)
            .constructor<>()
            .method("voidMethod", &A::voidMethod)
            .method("intMethodIntString", &A::intMethodIntString)
            .field("m_intField", &A::m_intField)
            .field("m_stringField", &A::m_stringField)
            .end();
        }();
};

//namespace testReflectable
//{
//    std::string stringMethodIntString(const int intValue, const std::string& stringValue)
//    {
//        std::string retStr;
//        for (int ii = intValue; ii >= 0; ii--)
//        {
//            retStr += stringValue;
//        }
//        return retStr;
//    }
//
//    inline static uintptr_t s_reflectable = []() {
//        reflectable::Reflectable::method("stringMethodIntString", &stringMethodIntString);
//        return 0;
//    }();
//}