#pragma once
#include "DllExport.h"

#include "memory"
#include "mutex"
#include "type_traits"

namespace pj
{
	namespace base
	{
		template<typename T>
		class Singleton
		{
		public:
			static T& getInstance()
			{
				if (!s_upInstance)
				{
					s_mutex.lock();
					if (!s_upInstance)
					{
						s_upInstance = std::make_unique<T>();
					}
					s_mutex.unlock();
				}
				return *s_upInstance;
			}

		private:
			Singleton(){}

			// Disable copy constructor and copy assignment
			Singleton(const Singleton&) = delete;
			Singleton& operator=(const Singleton&) = delete;

		private:
			static std::unique_ptr<T> s_upInstance;
			static std::mutex s_mutex;
		};

		template<typename T>
		std::unique_ptr<T> Singleton<T>::s_upInstance = nullptr;

		template<typename T>
		std::mutex Singleton<T>::s_mutex;
	}
}