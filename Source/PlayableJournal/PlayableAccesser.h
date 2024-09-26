#pragma once
#include "DllExport.h"

#include "PlayableMethod.h"
#include "string"
#include "v8.h"

namespace pj
{
	namespace playable
	{
		class PlayableAccesser final
		{
		public:
			PlayableAccesser(const std::string& name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter) : m_name(name), m_getter(getter), m_setter(setter) {};

			const std::string getName() const { return m_name; }
			const v8::AccessorGetterCallback& getGetter() const { return m_getter; }
			const v8::AccessorSetterCallback& getSetter() const { return m_setter; }
		private:
			std::string m_name;
			v8::AccessorGetterCallback m_getter = nullptr;
			v8::AccessorSetterCallback m_setter = nullptr;
		};
	}
}