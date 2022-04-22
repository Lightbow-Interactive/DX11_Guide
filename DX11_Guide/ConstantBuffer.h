#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <exception>

#include "IBindable.h"

enum EBufferType { BUFFER_VS, BUFFER_PS, BUFFER_NULL };

struct GlobalCBuffer
{
    DirectX::XMFLOAT4X4 viewProjectionMatrix;
};

struct PerObjectCBuffer
{
    DirectX::XMFLOAT4X4 objectWorldMatrix;
};

template<typename T>
class ConstantBuffer : public IBindable
{
public:
    void Init(EBufferType type, UINT slot, ID3D11Device* device)
    {
        m_device = device;
        m_slot = slot;

        CD3D11_BUFFER_DESC bufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16))),
            D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        HRESULT hr = m_device->CreateBuffer(&bufferDesc, 0, m_buffer.GetAddressOf());
        if (FAILED(hr)) throw std::exception();

        m_type = type;
    }

    void Bind(ID3D11DeviceContext* deviceContext) override
    {
        // Map the buffer in write mode
        D3D11_MAPPED_SUBRESOURCE resource;
        HRESULT hr = deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
        if (FAILED(hr)) throw std::exception();

        // Copy the data into the buffer and unmap it
        CopyMemory(resource.pData, &Data, sizeof(T));
        deviceContext->Unmap(m_buffer.Get(), 0);

        // Set the buffer
        if (m_type == BUFFER_VS)
            deviceContext->VSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
        else if (m_type == BUFFER_PS)
            deviceContext->PSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
    }

    T Data;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

    ID3D11Device* m_device = nullptr;

    EBufferType m_type = BUFFER_NULL;
    UINT m_slot = 0;

};