# PlayableJournal
C++ based journal framework that supports automatically recording and replaying application with help of JS V8 engine.
# Feature
- Logging into journal
- Recording key methods into journal
- Replaying key methods from journal
- Testing, including unit, integration, and regression testing
# Prerequisites
It supports MS Visual Studio 2022 with C++ 20 using standard conforming preprocessor.
# Example
## Unit Test
#### Create a new unit test console application
#### Setup project properties
1. Setup proper output and intermediate directory
2. C++ language standard to C++ 20 standard
3. Using standard conforming preprocessor (/Zc:preprocessor) under "Properties -> C/C++ -> Preprocessor"
4. Additional include directories of headers of PlayableJournal and v8
5. Additional library directories of lib files of PlayableJournal and v8 under "Properties -> Linker -> General"
6. #pragma comment libs of 
```
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
    // Win OS
    #pragma comment(lib, "DbgHelp.lib")
    #pragma comment(lib, "winmm.lib")
```
7. Resolve `_ITERATOR_DEBUG_LEVEL` error under debug mode if occurs by setting `_ITERATOR_DEBUG_LEVEL=0` in preprocessor definition under "Properties -> C/C++ -> Preprocessor"
8. Set `V8_COMPRESS_POINTERS=1` and `V8_ENABLE_SANDBOX` if necessary for both debug and release mode
9. Resolve "mismatch detected for 'RuntimeLibrary'" errors after a build under "Properties -> C/C++ -> Code Generation"
10. Add /Zc:__cplusplus to additional options under "Properties -> C/C++ -> Command Line"
# Known issues
1. V8 objects like v8::Array generated in callback from other dll from PlayableJournal cause Access violation reading location.
# Roadmap
TBDTBDTBDBTDTBDTBDTBDTBDTBDTBD

