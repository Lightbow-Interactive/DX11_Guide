#include "Texture.h"

#include <WICTextureLoader.h>

void Texture::Init(ID3D11Device* device, const std::wstring& file)
{
	auto samplerDesc = CD3D11_SAMPLER_DESC();
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HRESULT hr = device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
	if (FAILED(hr)) throw std::exception();

	hr = DirectX::CreateWICTextureFromFile(device, file.c_str(), nullptr, m_textureView.GetAddressOf());
	if (FAILED(hr)) throw std::exception();
}

void Texture::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, m_textureView.GetAddressOf());
}
