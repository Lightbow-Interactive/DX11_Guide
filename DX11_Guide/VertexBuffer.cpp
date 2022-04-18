#include "VertexBuffer.h"

#include "Graphics.h"

void VertexBuffer::Init(ID3D11Device* device, const std::vector<Vertex>& vertices)
{
    auto vertexBufferDesc = CD3D11_BUFFER_DESC();
    //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; <- no need to set this, as its CD3D11_DEFAULT (Press F12 on CD3D11_BUFFER_DESC to see defaults)
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size(); // size of the buffer
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // what's the buffer used for

    D3D11_SUBRESOURCE_DATA vertexBufferData{};
    vertexBufferData.pSysMem = vertices.data(); // Set the buffer data

    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    m_vertices = vertices; // Save the vertices, maybe we need them later
}

void VertexBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
}
