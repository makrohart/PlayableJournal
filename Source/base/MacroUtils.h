#pragma once

// =====================================================================================================
// Experimental: This will disable Intellisense and make macro hard to debug...
// https://www.scs.stanford.edu/~dm/blog/va-opt.html#recursive-macros
// This will re-scan macros 342 times, EXPAND4 gets called 256 times, 
// but the intermediary macros cause rescan as well.
#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(Macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(Macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(Macro, Arg, ...)                        \
  Macro(Arg)                                                    \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (Macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define FOR_EACH_2(Macro, ...)                                  \
  __VA_OPT__(EXPAND(FOR_EACH_2_HELPER(Macro, __VA_ARGS__)))
#define FOR_EACH_2_HELPER(Macro, Arg1, Arg2, ...)               \
  Macro(Arg1, Arg2)                                             \
  __VA_OPT__(FOR_EACH_2_AGAIN PARENS (Macro, __VA_ARGS__))
#define FOR_EACH_2_AGAIN() FOR_EACH_2_HELPER

#define FOR_EACH_WITH_STEP(Macro, Step, Begin, ...)                                  \
  __VA_OPT__(EXPAND(FOR_EACH_WITH_STEP_HELPER(Macro, Step, Begin, __VA_ARGS__)))
#define FOR_EACH_WITH_STEP_HELPER(Macro, Step, Begin, Arg, ...)               \
Macro(Begin, Arg)                                                             \
  __VA_OPT__(FOR_EACH_WITH_STEP_AGAIN PARENS (Macro, Step, Step(Begin) , __VA_ARGS__))
#define FOR_EACH_WITH_STEP_AGAIN() FOR_EACH_WITH_STEP_HELPER

#define STEP_1(ii) ii+1
#define STEP_ARG(Arg) _Arg

#define COMMA_ARGTYPE_ARG(Arg, ArgType) , ArgType Arg
#define COMMA_ARG(Arg, ArgType) , Arg
#define COMMA_STD_PLACEHOLDER(ArgIndex, ArgType) , std::_Ph<ArgIndex>{}
