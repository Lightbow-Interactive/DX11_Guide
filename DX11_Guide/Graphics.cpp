#include "Graphics.h"

#include <exception>

void Graphics::Init(HWND hWnd, int width, int height)
{
    m_hWnd = hWnd;
    m_width = width;
    m_height = height;

    InitD3D();
}

void Graphics::Render()
{
    const float clearColor[] = { 0.f, 0.f, 0.f, 0.f };
    m_deviceContext->ClearRenderTargetView(m_renderTarget.Get(), clearColor);

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