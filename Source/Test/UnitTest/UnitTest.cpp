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

// TestCase 1: Playable global method
JOURNALABLE_METHOD(std::vector<std::string>, base::string, splitString, const char*, const char*)
PLAYABLE_METHOD(std::vector<std::string>, base::string, splitString, const char*, const char*)

// TestCase 2: Playable class
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

// TestCase 3: AOP derived class
ASPECT_VCLASS_BEGIN(pj::playable::PlayableAspect, test::TestDerivedClass)
ASPECT_VMETHOD(void, print, std::string, int)
ASPECT_VCLASS_END

// TestCase 4: AOP class
ASPECT_CLASS_BEGIN(pj::playable::PlayableAspect, test::NativeClass)
ASPECT_MMETHOD(void, voidMethodInt, int)
ASPECT_MMETHOD(std::string, stringMethodStringInt, const std::string&, const int)
ASPECT_CLASS_END

// TestCase 5: AOP method
ASPECT_METHOD(pj::playable::PlayableAspect, std::vector<std::string>, base::string, splitString, const char*, const char*)

int main()
{
    test::TestDerivedClass testDerivedClass;
    aop::AspectProxy<test::TestDerivedClass, pj::playable::PlayableAspect> proxy(testDerivedClass);
    proxy.print("xyz_", 3);

    test::NativeClass nativeClass;
    aop::AspectProxy<test::NativeClass, pj::playable::PlayableAspect> nativeClassProxy(nativeClass);
    nativeClassProxy.voidMethodInt(12);

    aop::AspectProxy<std::vector<std::string>(const char*, const char*), pj::playable::PlayableAspect> splitStringProxy;
    splitStringProxy.splitString("xyz_", "y");

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
