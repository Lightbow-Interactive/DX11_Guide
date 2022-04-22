#include "IndexBuffer.h"

#include "Graphics.h"

void IndexBuffer::Init(ID3D11Device* device, const std::vector<UINT>& indices)
{
    auto indexBufferDesc = CD3D11_BUFFER_DESC();
    indexBufferDesc.ByteWidth = sizeof(UINT) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexBufferData{};
    indexBufferData.pSysMem = indices.data();

    HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    m_indices = indices;
}

void IndexBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
