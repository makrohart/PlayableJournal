#pragma once

#include "Aspectable.h"

namespace pj
{
	namespace journal
	{
		class JournalableAspect : public aspectable::Aspect<JournalableAspect>
		{
		public:
			template<typename R, typename F, typename... Args>
			R invoke(const F&& func, Args&&... args)
			{
			}
		};
	}
}