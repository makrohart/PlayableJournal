#pragma once

namespace reflectable
{
    struct Constructor
    {
        virtual ~Constructor() {}
    };

    template<typename... Args>
    struct CConstructor : Constructor
    {
    };
}