#include "Material.h"
#include "GameException.h"
#include "Model.h"

namespace Library {

	RTTI_DEFINITIONS(Material);

	Material::~Material() {
		// note material does not own Effect (for now)
	}

	const Effect* Material::effect() const {
		return m_effect;
	}

	const Technique* Material::curr_technique() const {
		return m_curr_technique;
	}

	void Material::set_curr_technique(int index) {
		m_curr_technique = m_effect->technique(index);
	}

	void Material::set_curr_technique(const std::string& name) {
		m_curr_technique = m_effect->technique(name);
	}

	void Material::init(Effect* effect) {
		m_effect = effect;
		set_curr_technique(0);
	}

	void Material::create_vertex_buffer(ID3D11Device* device, const Model& model, std::vector<ID3D11Buffer*>& buffers) const {
		buffers.reserve(model.meshes().size());
		for (auto* mesh : model.meshes()) {
			ID3D11Buffer* buffer;
			create_vertex_buffer(device, *mesh, &buffer);
			buffers.push_back(buffer);
		}
	}

	void Material::create_buffer(ID3D11Device* device, void* data, UINT count, UINT size,
								 D3D11_USAGE usage, ID3D11Buffer** buffer) const {
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = size * count;
		desc.Usage = usage;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA sub;
		ZeroMemory(&sub, sizeof(sub));
		sub.pSysMem = data;
		if (FAILED(device->CreateBuffer(&desc, &sub, buffer))) {
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

}