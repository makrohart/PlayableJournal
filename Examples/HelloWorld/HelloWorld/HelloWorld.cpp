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
#include "PlayableManager.h"

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

//JOURNALABLE_CLASS_BEGIN(command, Command)
//JOURNALABLE_METHOD(Command, execute)
//JOURNALABLE_CLASS_END(command, Command)

struct Playable_command_Command 
{
    Playable_command_Command() 
    {
        std::vector<pj::playable::PlayableMethod> methods; 
        for (int ii = 0; ii != m_methodCount; ++ii) 
            methods.push_back(m_playableMethods[ii]); 
        std::vector<pj::playable::PlayableAccesser> accessers; 
        for (int ii = 0; ii != m_propertyCount; ++ii) 
            accessers.push_back(m_playableProperties[ii]); 
        pj::playable::PlayableManager::getInstance()->add(pj::playable::PlayableClass("Command", New, methods, accessers));
    } 
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args) 
    {
        v8::Isolate* pIsolate = args.GetIsolate(); 
        command::Command* pCommand = new command::Command();
        args.This()->SetInternalField(0, v8::External::New(pIsolate, pCommand));
    }
private: 
    int m_propertyCount = 1; 
    pj::playable::PlayableAccesser m_playableProperties[1] = {
        pj::playable::PlayableAccesser{ 
            "RepeatCount", 
            [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) { 
                v8::Local<v8::Object> self = info.Holder(); 
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); 
                void* pNative = native->Value(); auto nativeValue = static_cast<command::Command*>(pNative)->getRepeatCount(); 
                auto value = propertyNative2JS<int>(nativeValue, info); 
                info.GetReturnValue().Set(value); 
            }, 
            [](v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) { 
                v8::Local<v8::Object> self = info.Holder(); 
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); 
                void* pNative = native->Value(); auto nativeValue = propertyJS2Native<int>(value, info); 
                static_cast<command::Command*>(pNative)->setRepeatCount(nativeValue); 
            }, 
        },
    };

private: 
    int m_methodCount = 1; 
    pj::playable::PlayableMethod m_playableMethods[1] = {

        pj::playable::PlayableMethod{ 
            "execute", 
            [](const v8::FunctionCallbackInfo<v8::Value>& args) { 
                v8::Isolate* pIsolate = args.GetIsolate(); 
                v8::HandleScope handleScope(pIsolate); 
                v8::Local<v8::Object> self = args.Holder(); 
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); 
                void* pNative = native->Value(); 
                static_cast<command::Command*>(pNative)->execute(

                    argJS2Native<std::string>(args, 0),
                    argJS2Native<int>(args, 1)
                ); 
            } 
        },
    };

} s_Playable_command_Command;

int main()
{
    Journalable<command::Directive> directive;
    directive.request();

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
