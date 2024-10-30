#pragma once

#include "Aspect.h"
#include "Journal.h"

namespace pj
{
	namespace journal
	{
		class JournalAspect : public aop::Aspect<JournalAspect>
		{
		public:
			template<typename R, typename F, typename... Args>
			static R invoke(const aop::MethodInfo& methodInfo, const F& func, Args&&... args) {}
		};
	}
}