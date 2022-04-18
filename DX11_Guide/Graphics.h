#pragma once 

#include <Windows.h>
#include <DirectXMath.h>

#include "PixelShader.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 color;

	Vertex(const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3 _color)
		: position(_position), color(_color)
	{ }
};

class Graphics
{
public:
    void Init(HWND hWnd, int width, int height);
    void Render();

private:
    int m_width = 0;
    int m_height = 0;
    HWND m_hWnd = NULL;

    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;

    // Scene
    VertexBuffer m_vertexBuffer;

    // Shaders
    VertexShader m_vertexShader;
    PixelShader m_pixelShader;

    void InitD3D();
    void LoadShaders();
    void SetupScene();

};