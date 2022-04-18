#pragma once

#include <string>
#include <wrl/client.h>

#include "IBindable.h"

class PixelShader : public IBindable
{
public:
	void Init(ID3D11Device* device, const std::wstring& file);
	void Bind(ID3D11DeviceContext* deviceContext) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_pixelShaderBuffer;


};
