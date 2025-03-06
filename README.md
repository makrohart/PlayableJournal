# PlayableJournal
A cutting-edge C++ log framework (dynamic link library) designed to revolutionize application debugging and testing. This robust framework not only offers comprehensive logging capabilities but also leverages the powerful JS V8 engine to automatically record and replay application state, providing developers with unparalleled insights into their code.
# Features
- Logging
- Recording key methods into journal
- Replaying key methods from journal
- Testing, from unit, integration, to regression tests
# Prerequisites
MS Visual Studio 2022 with C++ 20 is required
# Installation
1. Fetch the latest code
2. Compile
# Tutorial
#### Create and setup a blank C++ project
1. Setup proper output and intermediate directory.
2. C++ language standard selected to C++ 20 standard.
3. Use standard conforming preprocessor (/Zc:preprocessor) under "Properties -> C/C++ -> Preprocessor" specific for MS Visual Studio.
4. Add headers of PlayableJournal and v8 (under "ThirdParties" folder) to additional include directories.
5. Add lib files of PlayableJournal and v8 to additional library directories under "Properties -> Linker -> General".
6. `#pragma comment` libs of 
```
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
```
7. Resolve `_ITERATOR_DEBUG_LEVEL` error under debug mode if occurs by setting `_ITERATOR_DEBUG_LEVEL=0` in preprocessor definition under "Properties -> C/C++ -> Preprocessor".
8. Set `V8_COMPRESS_POINTERS=1` and `V8_ENABLE_SANDBOX` if necessary for both debug and release mode.
9. Resolve "mismatch detected for 'RuntimeLibrary'" errors after a build under "Properties -> C/C++ -> Code Generation".
10. Add /Zc:__cplusplus to additional options under "Properties -> C/C++ -> Command Line".
#### Use `PlayableJournal` library
- Declare and define your own class

  Given an example here:
  ```
    namespace test
    {
        class NativeClass
        {
        public:
           void voidMethodInt(int count) 
           {
               m_count = count;
               pj::journal::INFO("test::NativeClass::voidMethodInt: ", pj::utils::toString(count).c_str());
           };
        
           std::string stringMethodStringInt(const std::string& str, const int count)
           {
               m_count = count;
               std::string retVal;
               for (int ii = count; ii != 0; ii--)
                   retVal += str;
        
               return retVal;
           }
        
           int getCount() { return m_count; };
           void setCount(const int count) { m_count = count; };
        
        private:
           int m_count = 0;
        };
    }
  ```
- Use `JOURNALABLE_XXX` and `PLAYABLE_XXX` macros for recording and replaying target

  Now, use macros like follow
  ```
    JOURNALABLE_CLASS_BEGIN(test, NativeClass)
        JOURNALABLE_PROPERTY(Count, int)
        JOURNALABLE_MMETHOD(void, voidMethodInt, int)
        JOURNALABLE_MMETHOD(std::string, stringMethodStringInt, const std::string&, const int)
    JOURNALABLE_CLASS_END(test, NativeClass)
    
    PLAYABLE_CLASS_BEGIN(test, NativeClass)
        PLAYABLE_PROPERTIES_BEGIN(1)
            PLAYABLE_PROPERTY(Count, int, getCount, setCount)
        PLAYABLE_PROPERTIES_END
        PLAYABLE_MMETHODS_BEGIN(2)
            PLAYABLE_MMETHOD(void, voidMethodInt, int)
            PLAYABLE_MMETHOD(std::string, stringMethodStringInt, std::string, int)
        PLAYABLE_MMETHODS_END
    PLAYABLE_ClASS_END(test, NativeClass)
  ```
  The indent space is not really necessary but to make a clear structure of used macros.
- Play your project
  
  Now, we can play the js script with one last step
  ```
  pj::player::Player player;
  const char* script = "pathToScript\\script.js";
  player.play(script);
  ```
  In parallel, we have a script file (e.g. named as "script.js") in JavaScript
  ```
  nativeClass = new NativeClass();
  nativeClass.voidMethodInt(12);
  count = nativeClass.Count;
  str = nativeClass.stringMethodStringInt("xyz_", count + 1);
  ```
  If you run the application, the object and its methods are recalled successfully. This will also generate a corrosponding journal file (in format of "journal.js") under the same folder where executatable application locates. The journal file will log the detailed information you want, as well as the method you want to re-play. You can then try replay the journal file!
# Known Issues
1. Static Link Library is not supported as V8 objects generated in heap owned by lib, in which case the caller who use static link liarary will encounter crashes when operate objects.
2. To avoid this, most part of code logic related to bindings from C++ to JS and initilize V8 engine should be moved to header.
# Roadmap
1. Asynchronous logging
2. Support complete feature for recording C++ operation
3. JS code generated based on AST is more ideally.
4. Debugg feature
5. More feature for testing
...


