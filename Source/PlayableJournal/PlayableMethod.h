#pragma once
#include "DllExport.h"

#include "string"
#include "v8.h"
namespace pj
{
	namespace playable
	{
		PLAYABLEJOURNAL_API class PlayableMethod
		{
		public:
			PLAYABLEJOURNAL_API PlayableMethod(const std::string& methodName, v8::FunctionCallback method) : m_methodName(methodName), m_method(method) {}

			const std::string getName() const { return m_methodName; }
			const v8::FunctionCallback& getMethod() const { return m_method; }
		private:
			std::string m_methodName;
			v8::FunctionCallback m_method;
		};
	}
}