#pragma once 

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

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
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

    // Vertex Shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D10Blob> m_vertexShaderBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    // Pixel Shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D10Blob> m_pixelShaderBuffer;

    void InitD3D();
    void LoadShaders();
    void SetupScene();

};