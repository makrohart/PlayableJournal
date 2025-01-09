// UnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "PlayableJournal.lib")
#ifdef _DEBUG
#pragma comment(lib, "Debug\\v8.dll.lib")
#pragma comment(lib, "Debug\\v8_libbase.dll.lib")
#pragma comment(lib, "Debug\\v8_libplatform.dll.lib")
#else
#pragma comment(lib, "Release\\v8.dll.lib")
#pragma comment(lib, "Release\\v8_libbase.dll.lib")
#pragma comment(lib, "Release\\v8_libplatform.dll.lib")
#endif

#include <iostream>

#include "functional"
#include "Journalable.h"
#include "Playable.h"
#include "PlayableAspect.h"
#include "Player.h"
#include "JournalAspect.h"
#include "Utils.h"
#include "Journalable.h"
#include "ObjectInfo.h"
#include "StringUtils.h"

namespace test
{
    class TestBaseClass
    {
    public:
        virtual void print(std::string str, int count)
        {
            std::string message;
            for (int ii = count; ii != 0; ii--)
                message += str;

            pj::journal::INFO("test::TestBaseClass::print: ", message.c_str());
        }
    };

    class TestDerivedClass : TestBaseClass
    {
    public:
        virtual void print(std::string str, int count) override
        {
            std::string message;
            for (int ii = count; ii != 0; ii--)
                message += str;

            pj::journal::INFO("test::testDerivedClass::print: ", message.c_str());
        }
    };

    class NativeClass
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
    };
}

/// <summary>
/// Test case 1: std::vector<std::string> base::string::splitString(const char* str, const char* separator)
/// </summary>
JOURNALABLE_METHOD(std::vector<std::string>, base::string, splitString, const char*, const char*)
PLAYABLE_METHOD(std::vector<std::string>, base::string, splitString, const char*, const char*)

/// <summary>
/// Test case 2: Journal playable feature on class object
/// </summary>
JOURNALABLE_CLASS_BEGIN(test, NativeClass)
JOURNALABLE_PROPERTY(Count, int)
JOURNALABLE_MMETHOD(void, voidMethodInt, int)
JOURNALABLE_MMETHOD(std::string, stringMethodStringInt, const std::string&, const int)
JOURNALABLE_CLASS_END(test, NativeClass)

PLAYABLE_CLASS_BEGIN(test, NativeClass)
PLAYABLE_PROPERTIES_BEGIN(1)
PLAYABLE_PROPERTY(Count, int, getCount, setCount)
PLAYABLE_PROPERTIES_END
PLAYABLE_MMETHODS_BEGIN(2)
PLAYABLE_MMETHOD(void, voidMethodInt, int)
PLAYABLE_MMETHOD(std::string, stringMethodStringInt, std::string, int)
PLAYABLE_MMETHODS_END
PLAYABLE_ClASS_END(test, NativeClass)

/// <summary>
/// Test case 3: AOP feature on derived class
/// </summary>
ASPECT_VCLASS_BEGIN(pj::playable::PlayableAspect, test::TestDerivedClass)
ASPECT_VMETHOD(void, print, std::string, int)
ASPECT_VCLASS_END

int main()
{
    test::TestDerivedClass testDerivedClass;
    aop::AspectProxy<test::TestDerivedClass, pj::playable::PlayableAspect> proxy(testDerivedClass);
    proxy.print("xyz_", 3);

    pj::player::Player player;
    const char* unitTest = "D:\\Projects\\PlayableJournal\\Test\\UnitTest\\unitTest.js";
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
