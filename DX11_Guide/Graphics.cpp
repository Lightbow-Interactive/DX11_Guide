#include "Graphics.h"

#include <exception>
#include <d3dcompiler.h>

void Graphics::Init(HWND hWnd, int width, int height)
{
    m_hWnd = hWnd;
    m_width = width;
    m_height = height;

    InitD3D();

    LoadShaders();

    SetupScene();
}

void Graphics::Render()
{
    const float clearColor[] = { 0.f, 0.f, 0.f, 0.f };
    m_deviceContext->ClearRenderTargetView(m_renderTarget.Get(), clearColor);

    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->IASetInputLayout(m_inputLayout.Get());

    m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
    m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    m_deviceContext->Draw(3, 0);

    m_swapChain->Present(false, NULL);
}

void Graphics::InitD3D()
{
    DXGI_SWAP_CHAIN_DESC scd{};
    scd.BufferDesc.Width = m_width;                                                           
    scd.BufferDesc.Height = m_height;                                       
    scd.BufferDesc.RefreshRate.Numerator = 60;                             
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                        
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.SampleDesc.Count = 1;                                                                 
    scd.SampleDesc.Quality = 0;                                               
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                    
    scd.BufferCount = 1;                                                   
    scd.OutputWindow = m_hWnd;                                          
    scd.Windowed = true;                                                 
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;                             
    scd.Flags = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        0,                                              
        D3D_DRIVER_TYPE_HARDWARE,               
        0,                                         
        D3D11_CREATE_DEVICE_DEBUG,              
        0,                                       
        0,                                       
        D3D11_SDK_VERSION,                       
        &scd,                                   
        m_swapChain.GetAddressOf(),               
        m_device.GetAddressOf(),
        0,
        m_deviceContext.GetAddressOf());

    if (FAILED(hr))
        throw std::exception();

    ID3D11Texture2D* backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    hr = m_device->CreateRenderTargetView(backBuffer, NULL, m_renderTarget.GetAddressOf());
    if (FAILED(hr)) throw std::exception();
    backBuffer->Release();                                                                  

    m_deviceContext->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), nullptr);

    const auto vp = CD3D11_VIEWPORT(0.f, 0.f, static_cast<float>(m_width), static_cast<float>(m_height));
    m_deviceContext->RSSetViewports(1, &vp);
}

void Graphics::LoadShaders()
{
    // Vertex Shader

    D3D11_INPUT_ELEMENT_DESC vsInputLayoutDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    HRESULT hr = D3DReadFileToBlob(L"../bin/triangleVS.cso", m_vertexShaderBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    hr = m_device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), NULL, m_vertexShader.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    hr = m_device->CreateInputLayout(vsInputLayoutDesc, ARRAYSIZE(vsInputLayoutDesc), m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), m_inputLayout.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    // Pixel Shader

    hr = D3DReadFileToBlob(L"../bin/solidColorPS.cso", m_pixelShaderBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    hr = m_device->CreatePixelShader(m_pixelShaderBuffer->GetBufferPointer(), m_pixelShaderBuffer->GetBufferSize(), NULL, m_pixelShader.GetAddressOf());
    if (FAILED(hr)) throw std::exception();
}

void Graphics::SetupScene()
{
    Vertex vertices[] =
    {
        { { -0.5f, -0.5f, 1.f }, { 1.f, 0.f, 0.f } },
        { { 0.f, 0.5f, 1.f }, { 0.f, 1.f, 0.f } },
        { { 0.5f, -0.5f, 1.f }, { 0.f, 0.f, 1.f } }
    };

    auto vertexBufferDesc = CD3D11_BUFFER_DESC();
    //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; <- no need to set this, as its CD3D11_DEFAULT (Press F12 on CD3D11_BUFFER_DESC to see defaults)
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices); // size of the buffer
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // what's the buffer used for

    D3D11_SUBRESOURCE_DATA vertexBufferData{};
    vertexBufferData.pSysMem = vertices; // Set the buffer data

    HRESULT hr = m_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();
}
