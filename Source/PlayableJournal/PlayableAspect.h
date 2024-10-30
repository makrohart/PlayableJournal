#pragma once

#include "Aspect.h"
#include "format"
#include "Journal.h"
#include "Utils.h"

namespace pj
{
	namespace playable
	{
		class PlayableAspect : public aop::Aspect<PlayableAspect>
		{
		public:
			template<typename R, typename F, typename... Args>
			static R invoke(const aop::MethodInfo& methodInfo, const F& func, Args&&... args)
			{
				std::string argsStr = pj::utils::toStringFromArgs(", ", std::forward<Args>(args)...);
				pj::journal::PLAYABLE(methodInfo.getMethodName().c_str(), "(", argsStr.c_str(), ");");
				return func(std::forward<Args>(args)...);
			}
		};
	}
}