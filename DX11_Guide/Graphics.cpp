#include "Graphics.h"

#include <exception>
#include <d3dcompiler.h>
#include <vector>
#include <SDL.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_dx11.h"

void Graphics::Init(HWND hWnd, SDL_Window* sdlWindow, int width, int height)
{
    m_hWnd = hWnd;
    m_width = width;
    m_height = height;
    m_sdlWindow = sdlWindow;

    InitD3D();

    LoadShaders();

    SetupScene();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForD3D(sdlWindow);
    ImGui_ImplDX11_Init(m_device.Get(), m_deviceContext.Get());
}

void Graphics::Render()
{
    const float clearColor[] = { 0.f, 0.f, 0.f, 0.f };
    m_deviceContext->ClearRenderTargetView(m_renderTarget.Get(), clearColor);

    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_vertexShader.Bind(m_deviceContext.Get());
    m_pixelShader.Bind(m_deviceContext.Get());

    m_texture.Bind(m_deviceContext.Get());

    m_vertexBuffer.Bind(m_deviceContext.Get());
    m_deviceContext->Draw(m_vertexBuffer.GetSize(), 0);

    RenderGui();

    m_swapChain->Present(false, NULL);
}

void Graphics::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
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
        {"TEXTURE_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    m_vertexShader.Init(m_device.Get(), L"../bin/triangleVS.cso", vsInputLayoutDesc, ARRAYSIZE(vsInputLayoutDesc));

    // Pixel Shader

    m_pixelShader.Init(m_device.Get(), L"../bin/texturedTrianglePS.cso");
}

void Graphics::SetupScene()
{
    std::vector<Vertex> vertices; // vector instead of array because we need it later, when loading models
    vertices.emplace_back(Vertex({ -0.5f, -0.5f, 1.f }, {0.f, 0.56f}));
    vertices.emplace_back(Vertex({ 0.f, 0.5f, 1.f }, { 0.5f, 0.f}));
    vertices.emplace_back(Vertex({ 0.5f, -0.5f, 1.f }, { 1.f, 0.56f}));

    m_vertexBuffer.Init(m_device.Get(), vertices);

    m_texture.Init(m_device.Get(), L"../Content/grass_texture.jpg");
}

void Graphics::RenderGui()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Menu Bar

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Window"))
        {
            static bool selected = false;
            ImGui::Selectable("selectable", &selected);

            if (ImGui::MenuItem("menu item"))
            {
	            // Menu item clicked
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Window

    ImGui::Begin("A Window");
    {
        ImGui::Text("This is a label");
        ImGui::Button("This is a button");
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
