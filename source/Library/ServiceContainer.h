#pragma once

#include "Common.h"

namespace Library {

	class ServiceContainer {

	public:
		ServiceContainer();

		void add_service(UINT id, void* service);
		void remove_service(UINT id);
		void* get_service(UINT id) const;

	private:
		std::map<UINT, void*> m_services;

	};

}