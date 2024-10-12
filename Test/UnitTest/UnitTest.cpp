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
}


JOURNALABLE_METHOD(test, print, int, count, std::string, message)
PLAYABLE_METHOD(void, test, print, int, std::string)

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
