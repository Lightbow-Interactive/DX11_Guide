#include "Camera.h"

void Camera::SetProjection(float fovDeg, float aspectRatio, float nearZ, float farZ)
{
    float fovRad = (fovDeg / 360.f) * DirectX::XM_2PI;
    m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);   // Creates a perspective projection matrix with a fov, an aspect ratio and far and near Z.
                                                                                                // fov: field of view
                                                                                                // aspect ratio: aspect ratio of our screen (width/height)
                                                                                                // farZ: most far z value until it's clipped
                                                                                                // nearZ: most near z value until it's clipped
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

const DirectX::XMMATRIX& Camera::GetViewProjectionMatrix() const
{
    return DirectX::XMMatrixMultiply(m_viewMatrix, m_projectionMatrix);
}

void Camera::UpdateMatrix()
{
    WorldObject::UpdateMatrix();

    const DirectX::XMVECTOR camTarget = DirectX::XMVectorAdd(m_currentForward, m_position);
    m_viewMatrix = DirectX::XMMatrixLookAtLH(m_position, camTarget, m_currentUp);
																									     
}