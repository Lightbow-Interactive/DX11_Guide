#include "Stencil.h"

#include <exception>

void Stencil::Init(ID3D11Device* device, EStencilMode mode)
{
	auto stencilDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT{});

	if (mode == MODE_WRITE)
	{
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if (mode == MODE_MASK)
	{
		stencilDesc.DepthEnable = false;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}
	else
	{
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	}

	HRESULT hr = device->CreateDepthStencilState(&stencilDesc, m_stencil.GetAddressOf());
	if (FAILED(hr)) throw std::exception();
}

void Stencil::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetDepthStencilState(m_stencil.Get(), 0xFF);
}
