#pragma once
#include "DllExport.h"

#include "string"
#include "vector"
#include "v8.h"

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

namespace pj
{
	namespace utils
	{
		inline std::vector<std::string> splitString(const char* str, const char separator)
		{
			std::vector<std::string> strs;
			int subStrLen = 1;
			const char* subStr = str;
			while (true)
			{
				if (*str == separator)
				{
					strs.emplace_back(subStr, subStrLen);
					subStr = ++str;
					subStrLen = 1;
					continue;
				}

				if (*str == '\0')
				{
					strs.emplace_back(subStr, subStrLen);
					break;
				}

				str++;
				subStrLen++;
			}

			return strs;
		}

		inline std::string toLower(std::string str)
		{
			for (char& c : str)
			{
				// cast to unsigned char to avoid negative value
				c = std::tolower(static_cast<unsigned char>(c));
			}
			return str;
		}

		// Reads a file into a v8 string.
		inline v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name)
		{
#pragma warning(suppress : 4996)
			FILE* file = fopen(name, "rb");
			if (file == nullptr) return {};

			fseek(file, 0, SEEK_END);
			size_t size = ftell(file);
			rewind(file);

			char* chars = new char[size + 1];
			chars[size] = '\0';
			for (size_t i = 0; i < size;) {
				i += fread(&chars[i], 1, size - i, file);
				if (ferror(file)) {
					fclose(file);
					return {};
				}
			}
			fclose(file);
			v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
			delete[] chars;
			return result;
		}

		std::string formatString(const char* pStr);

		template<typename... Args>
		void formatString(const char* pStrFirst, const char* pStrSecond, Args... args)
		{
			std::string str = std::string(pStrFirst) + std::string(pStrSecond);
			return formatString(str.c_str(), args...);
		}

		template<typename T>
		std::string toString(T value)
		{
			return std::to_string<T>(value);
		}

		PLAYABLEJOURNAL_API inline std::string toString(const char* value)
		{
			return std::string(value);
		}

		PLAYABLEJOURNAL_API inline std::string toString(const char value)
		{
			char str[2]{0};
			str[0] = value;
			return std::string(str);
		}

		/// <summary>
		/// E.g. "std::string str" => ""std::string str""
		/// TODO: add this to regression
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		PLAYABLEJOURNAL_API inline std::string toString(const std::string& value)
		{
			return '"' + value + '"';
		}
	}
}