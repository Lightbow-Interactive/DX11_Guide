#pragma once 

#include <Windows.h>
#include <DirectXMath.h>

#include "Camera.h"
#include "Cube.h"
#include "PixelShader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

/*struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 textureCoord;

	Vertex(const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT2 _texCoord)
		: position(_position), textureCoord(_texCoord)
	{ }
};*/

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 color;

    Vertex(const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3 _color)
        : position(_position), color(_color)
    { }
};

struct SDL_Window;

class Graphics
{
public:
    void Init(HWND hWnd, SDL_Window* sdlWindow, int width, int height);
    void Render();
    void Shutdown();

private:
    int m_width = 0;
    int m_height = 0;
    HWND m_hWnd = NULL;
    SDL_Window* m_sdlWindow = nullptr;

    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

    // Scene
    //Texture m_texture;
    //VertexBuffer m_vertexBuffer;

    Camera m_camera;
    std::vector<Cube*> m_cubes;
    ConstantBuffer<GlobalCBuffer> m_globalCBuffer;

    // Depth
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    // Shaders
    VertexShader m_vertexShader;
    PixelShader m_pixelShader;

    void InitD3D();
    void LoadShaders();
    void SetupScene();
    void RenderGui();

};