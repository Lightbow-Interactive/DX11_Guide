#include "Cube.h"

#include "Graphics.h"

void Cube::Init(ID3D11Device* device)
{
	std::vector<Vertex> vertices =
	{
		//*0*/ Vertex({-1.f, -1.f, -1.f}, {1.f, 0.f, 0.f}),
		//*1*/ Vertex({1.f, -1.f, -1.f}, {0.f, 1.f, 0.f}),
		//*2*/ Vertex({1.f, 1.f, -1.f}, {0.f, 0.f, 1.f}),
		//*3*/ Vertex({-1.f, 1.f, -1.f}, {1.f, 0.f, 0.f}),

		//*4*/ Vertex({-1.f, -1.f, 1.f}, {0.f, 1.f, 0.f}),
		//*5*/ Vertex({1.f, -1.f, 1.f}, {0.f, 0.f, 1.f}),
		//*6*/ Vertex({1.f, 1.f, 1.f}, {1.f, 0.f, 0.f}),
		//*7*/ Vertex({-1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}),

		// Front
		Vertex({-1.f, -1.f, -1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}),
		Vertex({-1.f, 1.f, -1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}),
		Vertex({1.f, -1.f, -1.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f}),
		Vertex({1.f, -1.f, -1.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f}),
		Vertex({-1.f, 1.f, -1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}),
		Vertex({1.f, 1.f, -1.f}, {0.f, 0.f, -1.f}, {0.f, 0.f, 1.f}),

		// Right
		Vertex({1.f, -1.f, -1.f}, {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}),
		Vertex({1.f, 1.f, -1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({1.f, -1.f, 1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({1.f, -1.f, 1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({1.f, 1.f, -1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({1.f, 1.f, 1.f}, {1.f, 0.f, 0.f}, {1.f, 0.f, 0.f}),

		// Back
		Vertex({1.f, -1.f, 1.f}, {0.f, 0.f, 1.f}, {0.f, 0.f, 1.f}),
		Vertex({1.f, 1.f, 1.f}, {0.f, 0.f, 1.f}, {1.f, 0.f, 0.f}),
		Vertex({-1.f, -1.f, 1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f}),
		Vertex({-1.f, -1.f, 1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f}),
		Vertex({1.f, 1.f, 1.f}, {0.f, 0.f, 1.f}, {1.f, 0.f, 0.f}),
		Vertex({-1.f, 1.f, 1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f}),

		// Left
		Vertex({-1.f, -1.f, 1.f}, {-1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}),
		Vertex({-1.f, 1.f, 1.f}, {-1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}),
		Vertex({-1.f, -1.f, -1.f}, {-1.f, 0.f, 0.f}, {1.f, 0.f, 0.f}),
		Vertex({-1.f, -1.f, -1.f}, {-1.f, 0.f, 0.f}, {1.f, 0.f, 0.f}),
		Vertex({-1.f, 1.f, 1.f}, {-1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}),
		Vertex({-1.f, 1.f, -1.f}, {-1.f, 0.f, 0.f}, {1.f, 0.f, 0.f}),

		// Top
		Vertex({-1.f, 1.f, -1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}),
		Vertex({-1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 1.f, 0.f}),
		Vertex({1.f, 1.f, -1.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({1.f, 1.f, -1.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({-1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 1.f, 0.f}),
		Vertex({1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}),

		// Bottom
		Vertex({-1.f, -1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 1.f, 0.f}),
		Vertex({-1.f, -1.f, -1.f}, {0.f, -1.f, 0.f}, {1.f, 0.f, 0.f}),
		Vertex({1.f, -1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({1.f, -1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 0.f, 1.f}),
		Vertex({-1.f, -1.f, -1.f}, {0.f, -1.f, 0.f}, {1.f, 0.f, 0.f}),
		Vertex({1.f, -1.f, -1.f}, {0.f, -1.f, 0.f}, {0.f, 1.f, 0.f})
	};

	std::vector<UINT> indices =
	{
		0, 3, 1, 1, 3, 2,
		1, 2, 5, 5, 2, 6,
		5, 6, 4, 4, 6, 7,
		4, 7, 0, 0, 7, 3,
		3, 7, 2, 2, 7, 6,
		4, 0, 5, 5, 0, 1
	};

	/*std::vector<UINT> indices =
	{
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};*/

	m_cubeVertices.Init(device, vertices);
	m_cubeIndices.Init(device, indices);
	m_cubeConstantBuffer.Init(BUFFER_VS, 1, device);
	m_whitePS.Init(device, L"../bin/solidColorPS.cso");
	m_normalPS.Init(device, L"../bin/solidColorPhong.cso");
	m_stencilMask.Init(device, MODE_MASK);
	m_stencilWrite.Init(device, MODE_WRITE);
	m_stencilNone.Init(device, MODE_NONE);
}

void Cube::Render(ID3D11DeviceContext* deviceContext, int passIndex)
{
	if (passIndex == 0)
	{
		// Draw the cube normal
		m_normalPS.Bind(deviceContext);
		m_cubeVertices.Bind(deviceContext);
		m_stencilWrite.Bind(deviceContext);
		DirectX::XMStoreFloat4x4(&m_cubeConstantBuffer.Data.objectWorldMatrix, m_objectWorldMatrix);
		m_cubeConstantBuffer.Bind(deviceContext);
		deviceContext->Draw(m_cubeVertices.GetSize(), 0);
	}
	
	if (passIndex == 1)
	{
		// Draw a bigger version ob the cube with solid color and without depth and with stencil
		DirectX::XMFLOAT3 scale = m_scale;
		SetScale(m_scale.x + 0.02f, m_scale.y + 0.02f, m_scale.z + 0.02f);
		DirectX::XMStoreFloat4x4(&m_cubeConstantBuffer.Data.objectWorldMatrix, m_objectWorldMatrix);
		m_cubeConstantBuffer.Bind(deviceContext);
		m_stencilMask.Bind(deviceContext);
		m_whitePS.Bind(deviceContext);
		deviceContext->Draw(m_cubeVertices.GetSize(), 0);
		SetScale(scale);
	}
}
