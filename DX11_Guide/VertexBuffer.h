#pragma once

#include <vector>
#include <wrl/client.h>

#include "IBindable.h"

struct Vertex;

class VertexBuffer : public IBindable
{
public:
	void Init(ID3D11Device* device, const std::vector<Vertex>& vertices);
	void Bind(ID3D11DeviceContext* deviceContext) override;

	int GetSize() const { return (int)m_vertices.size();  }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	std::vector<Vertex> m_vertices;

};
