// UnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "PlayableJournal.lib")
#ifdef _DEBUG
#pragma comment(lib, "debug\\v8_monolith.lib")
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

#include "Journalable.h"
#include "Playable.h"
#include "Player.h"

//JOURNALABLE_R_METHOD(pj::utils, splitString, std::vector<std::string>, const char*, str, const char, separator)
//PLAYABLE_R_METHOD(pj::utils, splitString, std::vector<std::string>, 0, const char*, 1, const char)

namespace journalable 
{
    std::vector<std::string> splitString(const char* str, const char separator) 
    {
        pj::journal::PLAYABLE("retVal = ", "splitString", "(", pj::utils::toString(str).c_str(), ", ", pj::utils::toString(separator).c_str(), ");"); 
        return pj::utils::splitString(str, separator);
    };
};
struct Playable_splitString {
    Playable_splitString() {
        pj::playable::PlayableMethod method{ 
            "splitString", 
            [](const v8::FunctionCallbackInfo<v8::Value>& args) { 
                v8::Isolate* pIsolate = args.GetIsolate(); 
                v8::HandleScope handleScope(pIsolate); 
                auto context = pIsolate->GetCurrentContext();
                v8::Local<v8::Context> context1(context);
                // Create a new empty array.
                v8::Local<v8::Array> array = v8::Array::New(pIsolate, 3);

                // Fill out the values
                array->Set(context, 0, v8::Integer::New(pIsolate, 1));
                array->Set(context, 1, v8::Integer::New(pIsolate, 2));
                array->Set(context, 2, v8::Integer::New(pIsolate, 3));

                auto nativeRetVal = journalable::splitString(pj::utils::toNativeFromJS<const char*>(pIsolate, args[0]) , pj::utils::toNativeFromJS<const char>(pIsolate, args[1])); 
                auto retVal = pj::utils::toJSFromNative(pIsolate, nativeRetVal); 
                args.GetReturnValue().Set(retVal); 
            } 
        }; 
        pj::playable::PlayableManager::getInstance()->add(method);
    }
} s_Playable_splitString;

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
