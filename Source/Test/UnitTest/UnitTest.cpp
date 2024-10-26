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
#include "JournalAspect.h"
#include "Utils.h"
#include "Journalable.h"

JOURNALABLE_METHOD(std::vector<std::string>, pj::utils, splitString, const char*, const char)
PLAYABLE_METHOD(std::vector<std::string>, pj::utils, splitString, const char*, const char)

namespace demo
{
    class test
    {
    public:
        virtual void print(std::string str, int count)
        {

        }
    };

    class childTest : test
    {
    public:
        virtual void print(std::string str, int count) override
        {

        }
    };

    
}

void printt(std::string str, int count) 
{

}

ASPECT_METHOD(pj::journal::JournalAspect, void, printt, std::string, int)

ASPECT_VCLASS_BEGIN(pj::journal::JournalAspect, demo::test)
ASPECT_VMETHOD(void, print, std::string, int)
ASPECT_VCLASS_END

int main()
{
    demo::test test;
    aop::AspectProxy<demo::test, pj::journal::JournalAspect> proxy(test);
    proxy.print("xxx", 12);

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
