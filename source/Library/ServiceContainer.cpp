#include "ServiceContainer.h"

namespace Library {

	ServiceContainer::ServiceContainer() : m_services {} {}

	void ServiceContainer::add_service(UINT id, void* service) {
		m_services.insert({id, service});
	}

	void ServiceContainer::remove_service(UINT id) {
		m_services.erase(id);
	}

	void* ServiceContainer::get_service(UINT id) const {
		auto it = m_services.find(id);
		return (it != m_services.end() ? it->second : nullptr);
	}

}