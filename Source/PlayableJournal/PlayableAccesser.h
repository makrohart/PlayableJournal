#pragma once
#include "DllExport.h"

#include "PlayableMethod.h"
namespace pj
{
	namespace playable
	{
		class PlayableAccesser final
		{
		public:
			PlayableAccesser(PlayableMethod getter, PlayableMethod setter) : m_getter(getter), m_setter(setter) {};

		private:
			PlayableMethod m_getter;
			PlayableMethod m_setter;
		};
	}
}