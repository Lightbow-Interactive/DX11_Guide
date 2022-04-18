#pragma once

#include <string>
#include <wrl/client.h>

#include "IBindable.h"

class VertexShader : public IBindable
{
public:
	void Init(ID3D11Device* device, const std::wstring& file, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputElementCount);
	void Bind(ID3D11DeviceContext* deviceContext) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_vertexShaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

};
