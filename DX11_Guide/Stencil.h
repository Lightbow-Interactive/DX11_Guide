#pragma once

#include <wrl/client.h>

#include "IBindable.h"

enum EStencilMode
{
	MODE_WRITE,
	MODE_MASK,
	MODE_NONE
};

class Stencil : public IBindable
{
public:
	void Init(ID3D11Device* device, EStencilMode mode);
	void Bind(ID3D11DeviceContext* deviceContext) override;

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_stencil;

};
