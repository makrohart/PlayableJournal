// HelloWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "PlayableJournal.lib")
#ifdef _DEBUG
#pragma comment(lib, "debug\\compression_utils_portable.lib")
#pragma comment(lib, "debug\\icui18n.lib")
#pragma comment(lib, "debug\\icuuc.lib")
#pragma comment(lib, "debug\\mksnapshot.lib")
#pragma comment(lib, "debug\\v8_libbase.lib")
#pragma comment(lib, "debug\\v8_libplatform.lib")
#pragma comment(lib, "debug\\v8_monolith.lib")
#pragma comment(lib, "debug\\zlib.lib")
#else
#pragma comment(lib, "release\\compression_utils_portable.lib")
#pragma comment(lib, "release\\icui18n.lib")
#pragma comment(lib, "release\\icuuc.lib")
#pragma comment(lib, "release\\mksnapshot.lib")
#pragma comment(lib, "release\\v8_libbase.lib")
#pragma comment(lib, "release\\v8_libplatform.lib")
#pragma comment(lib, "release\\v8_monolith.lib")
#pragma comment(lib, "release\\zlib.lib")
#endif

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "winmm.lib")

#include <iostream>
#include "Journal.h"
#include "JournalManager.h"
#include "Journalable.h"
#include "Playable.h"
#include "vector"

namespace command
{
    class Command
    {
    public:
        Command() {};

        void execute()
        {
            pj::journal::INFO("Execute Command::excute()!");
        }
    };

    class Directive
    {
    public:
        Directive() {}

        void request()
        {
            pj::journal::INFO("Execute Directive::request()!");
        }
    };
}

JOURNALABLE_CLASS_BEGIN(command, Directive)
JOURNALABLE_METHOD(Directive, request)
JOURNALABLE_CLASS_END(command, Directive)

JOURNALABLE_CLASS_BEGIN(command, Command)
JOURNALABLE_METHOD(Command, execute)
JOURNALABLE_CLASS_END(command, Command)

PLAYABLE_CLASS_BEGIN(command, Command, 1)
PLAYABLE_METHOD(command, Command, execute)
PLAYABLE_ClASS_END(command, Command)

int main()
{
    Journalable<command::Directive> directive;
    directive.request();

    Journalable<command::Command> command;
    command.execute();

    const char file[] = "D:\\Projects\\IIAS\\Debugx64\\JSScript.js";
    pj::journal::JournalManager::getInstance()->executeJournal(file);
    std::cout << "Hello World!\n";
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
