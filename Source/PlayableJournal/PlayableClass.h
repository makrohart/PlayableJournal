#pragma once
#include "DllExport.h"

#include "functional"
#include "PlayableAccesser.h"
#include "PlayableMethod.h"
#include "string"
#include "vector"
#include "v8.h"

namespace pj
{
	namespace playable
	{
		class PlayableClass final
		{
		public:
			PlayableClass(const std::string& className, 
										      const v8::FunctionCallback& constructor, 
				                              const std::vector<PlayableMethod>& methods, 
				                              const std::vector<PlayableAccesser>& accessers)
			                                : m_name(className), 
											  m_constructor(constructor), 
											  m_methods(methods), 
											  m_accessers(accessers)
			{};

			const std::string& getName() const { return m_name; }
			const v8::FunctionCallback& getConstructor() const { return m_constructor; }
			const std::vector<PlayableMethod>& getMethods() const { return m_methods; }
			const std::vector<PlayableAccesser>& getAccessers() const { return m_accessers; }

		private:
			std::string m_name;
			v8::FunctionCallback m_constructor;
			std::vector<PlayableMethod> m_methods;
			std::vector<PlayableAccesser> m_accessers;
		};
	}
}