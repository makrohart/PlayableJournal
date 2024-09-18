#pragma once
#include "DllExport.h"

#include "functional"
#include "PlayableMethod.h"
#include "string"
#include "vector"
#include "v8.h"

namespace pj
{
	namespace playable
	{
		PLAYABLEJOURNAL_API class PlayableClass final
		{
		public:
			PLAYABLEJOURNAL_API PlayableClass(const std::string& className, v8::FunctionCallback constructor, const std::vector<PlayableMethod>& methods)
			: m_className(className), m_constructor(constructor), m_methods(methods) {};

			const std::string& getClassName() const { return m_className; }
			const v8::FunctionCallback& getConstructor() const { return m_constructor; }
			const std::vector<PlayableMethod>& getMethods() const { return m_methods; }
		private:
			std::string m_className;
			v8::FunctionCallback m_constructor;
			std::vector<PlayableMethod> m_methods;
		};
	}
}