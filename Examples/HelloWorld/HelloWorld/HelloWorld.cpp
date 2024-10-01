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
#include "Journalable.h"
#include "Playable.h"
#include "Player.h"
#include "string"
#include "vector"

namespace command
{
    class Command
    {
    public:
        Command() {};

        void execute(std::string message, int repeatCount)
        {
            m_message = message;
            m_repeatCount = repeatCount;
            for (; repeatCount != 0; repeatCount--)
                message += m_message;
            pj::journal::INFO(message.c_str());
        }

        int getRepeatCount()
        {
            return m_repeatCount;   
        }

        void setRepeatCount(const int repeatCount)
        {
            m_repeatCount = repeatCount;
        }

        int m_repeatCount;
        std::string m_message;
    };

    void print(std::string str)
    {
        pj::journal::INFO(str.c_str());
    }
}

JOURNALABLE_METHOD(command, print, std::string, str)

JOURNALABLE_CLASS_BEGIN(command, Command)
JOURNALABLE_PROPERTY(RepeatCount, int)
JOURNALABLE_MMETHOD(execute, std::string, message, int, repeatCount)
JOURNALABLE_CLASS_END(command, Command)


PLAYABLE_METHOD(command, print, 0, std::string)

PLAYABLE_CLASS_BEGIN(command, Command)
PLAYABLE_PROPERTIES_BEGIN(1)
PLAYABLE_PROPERTY(RepeatCount, int, getRepeatCount, setRepeatCount)
PLAYABLE_PROPERTIES_END
PLAYABLE_MMETHODS_BEGIN(1)
PLAYABLE_MMETHOD(execute, 0, std::string, 1, int)
PLAYABLE_MMETHODS_END
PLAYABLE_ClASS_END(command, Command)

int main()
{
    Journalable<command::Command> command;
    command.execute("Manually execute", 4);

    const char file[] = "D:\\Projects\\PlayableJournal\\Examples\\HelloWorld\\x64\\Debug\\JSScript.js";
    pj::player::Player player;
    player.play(file);
    pj::journal::INFO("Hello World!\n");
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
