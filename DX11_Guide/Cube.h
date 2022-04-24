#pragma once

#include "WorldObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Stencil.h"

class Cube : public WorldObject
{
public:
	void Init(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext, int passIndex);
		
private:
	VertexBuffer m_cubeVertices;
	IndexBuffer m_cubeIndices;
	ConstantBuffer<PerObjectCBuffer> m_cubeConstantBuffer;

	PixelShader m_whitePS;
	PixelShader m_normalPS;
	Stencil m_stencilNone;
	Stencil m_stencilWrite;
	Stencil m_stencilMask;

	std::vector<IBindable*> m_pass1Bindables;
	std::vector<IBindable*> m_pass2Bindables;

};
