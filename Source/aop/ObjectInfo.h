#pragma once

#include "string"

namespace aop
{
	class ObjectInfo
	{
	public:
		ObjectInfo(const std::string& instanceName, const int id) : m_id(id), m_instanceName(instanceName) {}

		int getId() const { return m_id; }
		std::string getInstanceName() const { return m_instanceName; }
	private:
		int m_id;
		std::string m_instanceName;
	};
}