#pragma once

#include "WorldObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Cube : public WorldObject
{
public:
	void Init(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext);
		
private:
	VertexBuffer m_cubeVertices;
	IndexBuffer m_cubeIndices;
	ConstantBuffer<PerObjectCBuffer> m_cubeConstantBuffer;

};
