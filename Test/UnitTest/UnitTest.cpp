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

JOURNALABLE_METHOD(std::vector<std::string>, pj::utils, splitString, const char*, const char)

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

JOURNALABLE_METHOD(void, test, print, int, std::string)
PLAYABLE_METHOD(void, test, print, int, std::string)

JOURNALABLE_CLASS_BEGIN(test, testClass)
JOURNALABLE_PROPERTY(Count, int)
JOURNALABLE_MMETHOD(void, testMethod, int, std::string)
JOURNALABLE_CLASS_END(test, testClass)

PLAYABLE_CLASS_BEGIN(test, testClass)
    PLAYABLE_PROPERTIES_BEGIN(1)
        PLAYABLE_PROPERTY(Count, int, getCount, setCount)
    PLAYABLE_PROPERTIES_END
    PLAYABLE_MMETHODS_BEGIN(1)
        PLAYABLE_MMETHOD(void, testMethod, int, std::string)
    PLAYABLE_MMETHODS_END
PLAYABLE_ClASS_END(test, testClass)

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
