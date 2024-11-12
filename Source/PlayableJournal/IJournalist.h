#pragma once

#include "exception"

namespace pj
{
	namespace journal
	{
		class IJournalist
		{
		public:
			virtual void write(const char* pData) { throw std::exception("IJournalist::write shouldbe overriden!"); };
		};
	}
}