#pragma once

#include <string>
#include "Type.h"

namespace reflectable
{
    struct Reflectable
    {
        static reflectable::Type& type(const std::string& name)
        {
            return *(new reflectable::Type(name));
        }

        //template<typename F>
        //static void method(const std::string& name, F&& method)
        //{
        //    GMethodManager::getInstance()->add(new GMethod(name, method));
        //}
    };
}