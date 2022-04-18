#pragma once

#include <string>
#include <wrl/client.h>

#include "IBindable.h"

class Texture : public IBindable
{
public:
	void Init(ID3D11Device* device, const std::wstring& file);
	void Bind(ID3D11DeviceContext* deviceContext) override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;

};
