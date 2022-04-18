#include "VertexShader.h"

#include <d3dcompiler.h>

void VertexShader::Init(ID3D11Device* device, const std::wstring& file, D3D11_INPUT_ELEMENT_DESC* inputLayout,
                        UINT inputElementCount)
{
    HRESULT hr = D3DReadFileToBlob(file.c_str(), m_vertexShaderBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    hr = device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), NULL, m_vertexShader.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    hr = device->CreateInputLayout(inputLayout, inputElementCount, m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), m_inputLayout.GetAddressOf());
    if (FAILED(hr)) throw std::exception();
}

void VertexShader::Bind(ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetInputLayout(m_inputLayout.Get());
    deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
}
