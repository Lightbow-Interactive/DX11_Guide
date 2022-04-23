#include "Graphics.h"

#include <ctime>
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
    const float clearColor[] = { 0.f, 0.f, 0.f, 1.f };
    m_deviceContext->ClearRenderTargetView(m_renderTarget.Get(), clearColor);
    m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_deviceContext->RSSetState(m_rasterizerState.Get());

    m_vertexShader.Bind(m_deviceContext.Get());
    m_pixelShader.Bind(m_deviceContext.Get());

    //m_texture.Bind(m_deviceContext.Get());

    //m_vertexBuffer.Bind(m_deviceContext.Get());
    //m_deviceContext->Draw(m_vertexBuffer.GetSize(), 0);

    DirectX::XMMATRIX viewProjection = m_camera.GetViewMatrix() * m_camera.GetProjectionMatrix();
    DirectX::XMStoreFloat4x4(&m_globalCBuffer.Data.viewProjectionMatrix, viewProjection);
    m_globalCBuffer.Data.eyePos = m_camera.GetPosition();
    m_globalCBuffer.Bind(m_deviceContext.Get());

    m_lightsCBuffer.Bind(m_deviceContext.Get());

    for (int i = 0; i < m_cubes.size(); ++i)
    {
        m_cubes[i]->Render(m_deviceContext.Get());
    }

    RenderGui();

    m_swapChain->Present(false, NULL);
}

void Graphics::Shutdown()
{
	for (int i = 0; i < m_cubes.size(); ++i)
	{
        delete m_cubes[i];
	}

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

    // Create the texture for the depth buffer
    auto depthBufferTextureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, m_width, m_height, 1, 1);
    depthBufferTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferTextureDesc.SampleDesc.Count = 1; // Same as in swapchain desc
    hr = m_device->CreateTexture2D(&depthBufferTextureDesc, 0, m_depthStencilBuffer.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    // Create the depth stencil view
    hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, m_depthStencilView.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    // Create the stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = true; // Enable depth
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK; // Cull the back face
    hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());
    if (FAILED(hr)) throw std::exception();

    m_deviceContext->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), m_depthStencilView.Get());

    const auto vp = CD3D11_VIEWPORT(0.f, 0.f, static_cast<float>(m_width), static_cast<float>(m_height));
    m_deviceContext->RSSetViewports(1, &vp);
}

void Graphics::LoadShaders()
{
    // Vertex Shader

    D3D11_INPUT_ELEMENT_DESC vsInputLayoutDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    m_vertexShader.Init(m_device.Get(), L"../bin/3dVS.cso", vsInputLayoutDesc, ARRAYSIZE(vsInputLayoutDesc));

    // Pixel Shader

    m_pixelShader.Init(m_device.Get(), L"../bin/solidColorPhong.cso");
}

void Graphics::SetupScene()
{
    /*std::vector<Vertex> vertices; // vector instead of array because we need it later, when loading models
    vertices.emplace_back(Vertex({ -0.5f, -0.5f, 1.f }, {0.f, 0.56f}));
    vertices.emplace_back(Vertex({ 0.f, 0.5f, 1.f }, { 0.5f, 0.f}));
    vertices.emplace_back(Vertex({ 0.5f, -0.5f, 1.f }, { 1.f, 0.56f}));

    m_vertexBuffer.Init(m_device.Get(), vertices);

    m_texture.Init(m_device.Get(), L"../Content/grass_texture.jpg");*/

    m_camera.SetProjection(90.f, static_cast<float>(m_width) / static_cast<float>(m_height), 1.f, 10000.f);
    m_camera.UpdateMatrix();

    m_globalCBuffer.Init(BUFFER_VS, 0, m_device.Get());
    m_lightsCBuffer.Init(BUFFER_PS, 0, m_device.Get());

    srand(time(0));

    for (int i = 0; i < 10; ++i)
    {
        Cube* cube = new Cube();
        cube->Init(m_device.Get());
        cube->SetPosition(rand() % 10 + 1, rand() % 10 + 1, rand() % 10 + 1);
        cube->SetRotation(rand() % 360 + 1, rand() % 360 + 1, rand() % 360 + 1);
        m_cubes.push_back(cube);
    }
}

void Graphics::RenderGui()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Menu Bar

    /*if (ImGui::BeginMainMenuBar())
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
    ImGui::End();*/

    ImGui::Begin("Camera Controls");
    {
        DirectX::XMFLOAT3 camPos;
        DirectX::XMStoreFloat3(&camPos, m_camera.m_position);
        ImGui::DragFloat3("Position", &camPos.x, 0.01f);
        m_camera.SetPosition(camPos);

        DirectX::XMFLOAT3 camRotation;
        DirectX::XMStoreFloat3(&camRotation, m_camera.m_rotation);
        ImGui::DragFloat3("Rotation", &camRotation.x, 0.01f);
        m_camera.SetRotation(camRotation);
    }
    ImGui::End();

    ImGui::Begin("Ambient Light");
    {
        ImGui::DragFloat("Ambient Light Factor", &m_lightsCBuffer.Data.ambientLightFactor, 0.01f, 0.f, 1.f);
    }
    ImGui::End();

    ImGui::Begin("Directional Light");
    {
        ImGui::DragFloat3("Directional Light Color", &m_lightsCBuffer.Data.directionalLightColor.x, 0.01f, 0.f, 1.f);
        ImGui::DragFloat3("Directional Light Direction", &m_lightsCBuffer.Data.directionalLightDirection.x, 0.01f);
        ImGui::DragFloat("Directional Light Strength", &m_lightsCBuffer.Data.directionalLightStrength, 0.01f);
    }
    ImGui::End();

    ImGui::Begin("Point Light");
    {
        ImGui::DragFloat3("Point Light Color", &m_lightsCBuffer.Data.pointLightColor.x, 0.01f, 0.f, 1.f);
        ImGui::DragFloat3("Point Light Position", &m_lightsCBuffer.Data.pointLightPosition.x, 0.01f);
        ImGui::DragFloat("Point Light Strength", &m_lightsCBuffer.Data.pointLightStrength, 0.01f);
    }
    ImGui::End();

    ImGui::Begin("Spot Light");
    {
        ImGui::DragFloat3("Spot Light Color", &m_lightsCBuffer.Data.spotLightColor.x, 0.01f, 0.f, 1.f);
        ImGui::DragFloat3("Spot Light Position", &m_lightsCBuffer.Data.spotLightPosition.x, 0.01f);
        ImGui::DragFloat("Spot Light Strength", &m_lightsCBuffer.Data.spotLightStrength, 0.01f);
        ImGui::DragFloat3("Spot Light Direction", &m_lightsCBuffer.Data.spotLightDirection.x, 0.01f);
        ImGui::DragFloat("Spot Light Cutoff", &m_lightsCBuffer.Data.spotLightCutoff, 0.01f);
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
