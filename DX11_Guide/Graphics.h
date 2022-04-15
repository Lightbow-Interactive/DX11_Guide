#pragma once 

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

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

    void InitD3D();

};