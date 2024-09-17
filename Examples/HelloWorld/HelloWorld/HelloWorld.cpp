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
#include "Playable.h"
#include "Utils.h"

namespace command
{
    class ICommand
    {
    public:
        virtual void execute() = 0;
    };

    class Command : public ICommand
    {
    public:
        Command() {};

        virtual void execute() override
        {
            pj::journal::INFO("Execute Command::excute()!");
        }
    };
}

template<typename T> 
    requires std::derived_from<T, command::ICommand> 
class Playable : public command::ICommand {
public: 
    Playable() 
    {
    m_lastId++; m_instanceName = pj::utils::toLower("Command") + "_" + std::to_string(m_lastId); 
    pj::journal::PLAYABLE(m_instanceName.c_str(), " = new ", "Command", "()", ";");
}
      virtual void execute() override 
      {
          pj::journal::PLAYABLE(m_instanceName.c_str(), ".", "execute", "();"); m_t.execute();
      }
JOURNALABLE_END()

struct Playable_command_Command {
    Playable_command_Command() {
        pj::journal::JS2Native::add(Playable_command_Command::Init);
    } static void New(const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* pIsolate = args.GetIsolate(); Playable<command::Command>* pCommand = new Playable<command::Command>(); args.This()->SetInternalField(0, v8::External::New(pIsolate, pCommand));
    } static void Init(v8::Local<v8::ObjectTemplate> global) {
        v8::Isolate* pIsolate = v8::Isolate::GetCurrent(); v8::Local<v8::FunctionTemplate> _CommandTemplate = v8::FunctionTemplate::New(pIsolate, Playable_command_Command::New); _CommandTemplate->SetClassName(v8::String::NewFromUtf8(pIsolate, "Command").ToLocalChecked()); global->Set(pIsolate, "Command", _CommandTemplate); v8::Local<v8::ObjectTemplate> _Command_Prototype = _CommandTemplate->PrototypeTemplate();
        _Command_Prototype->Set(pIsolate, "execute", v8::FunctionTemplate::New(pIsolate, Playable_command_Command::execute));
        v8::Local<v8::ObjectTemplate> _Command_Instance = _CommandTemplate->InstanceTemplate(); _Command_Instance->SetInternalFieldCount(1);
    }
    static void execute(const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* pIsolate = args.GetIsolate(); v8::HandleScope handleScope(pIsolate); v8::Local<v8::Object> self = args.Holder(); v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0)); void* pNative = native->Value(); static_cast<Playable<command::Command>*>(pNative)->execute();
    }
} s_Playable_command_Command;

int main()
{
    Playable<command::Command> command;
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
