#pragma once
#include "DllExport.h"

#include "Singleton.h"
#include "PlayableType.h"
#include <vector>

namespace playable
{

	class PlayableManager final
	{
		DECLARE_SINGLETON_DEFAULT(PlayableManager)

	public:
		void add(PlayableType* pType)
		{
			m_types.emplace_back(pType);
		}

		const std::vector<std::unique_ptr<playable::PlayableType>>& getTypes()
		{
			return m_types;
		}

	private:
		std::vector<std::unique_ptr<playable::PlayableType>> m_types;
	};
}