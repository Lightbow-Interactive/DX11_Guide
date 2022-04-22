#pragma once

#include "WorldObject.h"

class Camera : public WorldObject
{
public:
    friend class Graphics;

    void SetProjection(float fovDeg, float aspectRatio, float nearZ, float farZ);

    const DirectX::XMMATRIX& GetViewMatrix() const;
    const DirectX::XMMATRIX& GetProjectionMatrix() const;
    const DirectX::XMMATRIX& GetViewProjectionMatrix() const;

    void UpdateMatrix() override;

private:
    DirectX::XMMATRIX m_viewMatrix = DirectX::XMMatrixIdentity();       // The camera's view matrix
    DirectX::XMMATRIX m_projectionMatrix = DirectX::XMMatrixIdentity(); // The camera's projection matrix

};