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

namespace test
{
    class testBaseClass
    {
    public:
        virtual void print(std::string str, int count)
        {
            std::string message;
            for (int ii = count; ii != 0; ii--)
                message += str;

            pj::journal::INFO("test::testBaseClass::print: ", message.c_str());
        }
    };

    class testDerivedClass : testBaseClass
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
}

JOURNALABLE_METHOD(std::vector<std::string>, pj::utils, splitString, const char*, const char)
PLAYABLE_METHOD(std::vector<std::string>, pj::utils, splitString, const char*, const char)

ASPECT_VCLASS_BEGIN(pj::playable::PlayableAspect, test::testDerivedClass)
ASPECT_VMETHOD(void, print, std::string, int)
ASPECT_VCLASS_END

int main()
{
    test::testDerivedClass testDerivedClass;
    aop::AspectProxy<test::testDerivedClass, pj::playable::PlayableAspect> proxy(testDerivedClass);
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
