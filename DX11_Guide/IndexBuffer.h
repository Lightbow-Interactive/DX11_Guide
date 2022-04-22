#pragma once

#include <vector>
#include <wrl/client.h>

#include "IBindable.h"

class IndexBuffer : public IBindable
{
public:
	void Init(ID3D11Device* device, const std::vector<UINT>& indices);
	void Bind(ID3D11DeviceContext* deviceContext) override;

	int GetSize() const { return (int)m_indices.size(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	std::vector<UINT> m_indices;

};
