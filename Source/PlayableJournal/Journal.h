#pragma once
#include "DllExport.h"

#include "string"

namespace pj
{
	namespace journal
	{
		/// <summary>
		/// Write to journal file as comment in a line starting with "// ".
		/// </summary>
		/// <param name="pStr">Comment in journal file (.js)</param>
		/// <returns>void</returns>
		PLAYABLEJOURNAL_API void INFO(const char* pStr);

		/// <summary>
		/// Write to journal file as comments in a line starting with "// ".
		/// </summary>
		/// <typeparam name="...Args">const char*</typeparam>
		/// <param name="pStrFirst">First part of comment</param>
		/// <param name="pStrSecond">Second part of comment</param>
		/// <param name="...args">Rest part of comment</param>
		/// <returns>void</returns>
		template<typename... Args>
		void INFO(const char* pStrFirst, const char* pStrSecond, Args... args)
		{
			std::string str = std::string(pStrFirst) + std::string(pStrSecond);
			INFO(str.c_str(), args...);
		}

		/// <summary>
		/// Write to journal file as comment in a line starting with "// ".
		/// TODO: comments should have fatal level info
		/// </summary>
		/// <param name="pStr">Comment in journal file (.js)</param>
		/// <returns>void</returns>
		PLAYABLEJOURNAL_API void FATAL(const char* pStr);

		/// <summary>
		/// Write to journal file as comments in a line starting with "// ".
		/// </summary>
		/// <typeparam name="...Args">const char*</typeparam>
		/// <param name="pStrFirst">First part of comment</param>
		/// <param name="pStrSecond">Second part of comment</param>
		/// <param name="...args">Rest part of comment</param>
		/// <returns>void</returns>
		template<typename... Args>
		void FATAL(const char* pStrFirst, const char* pStrSecond, Args... args)
		{
			std::string str = std::string(pStrFirst) + std::string(pStrSecond);
			FATAL(str.c_str(), args...);
		}
	}
}