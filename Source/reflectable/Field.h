#pragma once

#include <string>
namespace reflectable
{
	struct Field
	{
		Field(const std::string& name, const int offset) : m_name(name), m_offset(offset) {}

	private:
		const std::string m_name;
		const int m_offset;
	};
}