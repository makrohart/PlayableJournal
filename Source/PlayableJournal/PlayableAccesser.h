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
			PlayableAccesser(const std::string& name, v8::AccessorNameGetterCallback getter, v8::AccessorNameSetterCallback setter) : m_name(name), m_getter(getter), m_setter(setter) {};

			const std::string getName() const { return m_name; }
			const v8::AccessorNameGetterCallback& getGetter() const { return m_getter; }
			const v8::AccessorNameSetterCallback& getSetter() const { return m_setter; }
		private:
			std::string m_name;
			v8::AccessorNameGetterCallback m_getter = nullptr;
			v8::AccessorNameSetterCallback m_setter = nullptr;
		};
	}
}