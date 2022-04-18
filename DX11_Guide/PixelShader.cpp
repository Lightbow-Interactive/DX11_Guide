#include "PixelShader.h"

#include <d3dcompiler.h>

void PixelShader::Init(ID3D11Device* device, const std::wstring& file)
{
    HRESULT hr = D3DReadFileToBlob(file.c_str(), m_pixelShaderBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    hr = device->CreatePixelShader(m_pixelShaderBuffer->GetBufferPointer(), m_pixelShaderBuffer->GetBufferSize(), NULL, m_pixelShader.GetAddressOf());
    if (FAILED(hr)) throw std::exception();
}

void PixelShader::Bind(ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
}
