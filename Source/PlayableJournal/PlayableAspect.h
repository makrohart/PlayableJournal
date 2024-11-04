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

			virtual void preConstructor(const aop::ObjectInfo& objectInfo) override
			{
				const std::string className = objectInfo.getInstanceName();
				const int id = objectInfo.getId();
				const std::string instanceName = pj::utils::toLower(className) + "_" + std::to_string(id);
				pj::journal::PLAYABLE(instanceName.c_str(), " = new ", className.c_str(), "()", ";");
			}

			template<typename R, typename F, typename... Args>
			static R invoke(const aop::ObjectInfo* pObjectInfo, const aop::MethodInfo& methodInfo, const F& func, Args&&... args)
			{
				std::string argsStr = pj::utils::toStringFromArgs(", ", std::forward<Args>(args)...);

				if (pObjectInfo)
				{
					const std::string className = pObjectInfo->getInstanceName();
					const int id = pObjectInfo->getId();
					const std::string instanceName = pj::utils::toLower(className) + "_" + std::to_string(id);
					pj::journal::PLAYABLE(instanceName.c_str(), ".", methodInfo.getMethodName().c_str(), "(", argsStr.c_str(), ");");
				}
				else
				{
					pj::journal::PLAYABLE(methodInfo.getMethodName().c_str(), "(", argsStr.c_str(), ");");
				}

				return func(std::forward<Args>(args)...);
			}
		};
	}
}