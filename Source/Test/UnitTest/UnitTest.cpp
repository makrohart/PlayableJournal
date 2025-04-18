// UnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "PlayableJournal.lib")
#ifdef _DEBUG
#pragma comment(lib, "Debug\\v8.dll.lib")
#pragma comment(lib, "Debug\\v8_libbase.dll.lib")
#pragma comment(lib, "Debug\\v8_libplatform.dll.lib")
#else
#pragma comment(lib, "Release\\v8.dll.lib")
#pragma comment(lib, "Release\\v8_libbase.dll.lib")
#pragma comment(lib, "Release\\v8_libplatform.dll.lib")
#endif

#include <iostream>

#include "functional"
#include "Player.h"
#include "Utils.h"
#include "StringUtils.h"
#include "TestAspectable.h"
#include "TestReflectable.h"
#include "TestPlayable.h"
#include "Playable.h"

int main()
{
    // Aspectable
    TestAspectableClass testClass;
    auto strResult = testClass.invoke(&TestAspectableClass::stringMethodIntString, 3, "xyz_");
    auto doubleResult = testClass.invoke(&TestAspectableClass::doubleMethodInt, 10);
    testClass.invoke(&TestAspectableClass::voidMethodInt, 1);

    playable::Playable::type("A")
        .constructor<A>()
        .method("voidMethod", &A::voidMethod).playable<&A::voidMethod>()
        .end();
        //.method("intMethodIntString", &A::intMethodIntString).makePlayable<&A::intMethodIntString>();


    //Playable playable;
    //playable.method("voidMethod", &A::voidMethod);
    //auto function = std::bind(
    //    &TestAspectableClass::invoke<decltype(&TestAspectableClass::stringMethodIntString), int, std::string>,
    //    testClass,
    //    &TestAspectableClass::stringMethodIntString,
    //    3, "xyz_");

    aspectable::Aspectable<TestAspect_1, TestAspect_2>::invoke(&testAspectableMethods::stringMethodIntString, 2, "xyz_");
    
    // Playable
    playable::Player player;
    const char* unitTest = "D:\\Projects\\PlayableJournal\\Test\\UnitTest\\testPlayable.js";
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
