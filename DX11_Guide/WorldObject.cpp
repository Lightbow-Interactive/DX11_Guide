#include "WorldObject.h"

const DirectX::XMFLOAT3& WorldObject::GetPosition() const
{
    DirectX::XMFLOAT3 positionF3{};
    DirectX::XMStoreFloat3(&positionF3, m_position);
    return positionF3;
}

void WorldObject::SetPosition(const DirectX::XMFLOAT3& position)
{
    m_position = DirectX::XMLoadFloat3(&position);
    UpdateMatrix();
}

void WorldObject::SetPosition(float x, float y, float z)
{
    SetPosition({ x, y, z });
}

void WorldObject::AddLocalOffset(float x, float y, float z)
{
    const DirectX::XMFLOAT3 currentPos = GetPosition();
    SetPosition(currentPos.x + x, currentPos.y + y, currentPos.z + z);
}

void WorldObject::AddLocalOffset(const DirectX::XMFLOAT3& oF3)
{
    AddLocalOffset(oF3.x, oF3.y, oF3.z);
}

void WorldObject::AddLocalOffset(const DirectX::XMVECTOR& oVector)
{
    DirectX::XMFLOAT3 oF3{};
    DirectX::XMStoreFloat3(&oF3, oVector);
    AddLocalOffset(oF3);
}

const DirectX::XMFLOAT3& WorldObject::GetRotation() const
{
    DirectX::XMFLOAT3 rotationF3{};
    DirectX::XMStoreFloat3(&rotationF3, m_rotation);
    return rotationF3;
}

void WorldObject::SetRotation(const DirectX::XMFLOAT3& rotation)
{
    m_rotation = DirectX::XMLoadFloat3(&rotation);
    UpdateMatrix();
}

void WorldObject::SetRotation(float roll, float pitch, float yaw)
{
    SetRotation({ roll, pitch, yaw });
}

void WorldObject::AddRotationOffset(float roll, float pitch, float yaw)
{
    const DirectX::XMFLOAT3 currentRot = GetRotation();
    SetRotation(currentRot.x + roll, currentRot.y + pitch, currentRot.z + yaw);
}

const DirectX::XMFLOAT3& WorldObject::GetScale() const
{
    return m_scale;
}

void WorldObject::SetScale(const DirectX::XMFLOAT3& scale)
{
    m_scale = scale;
    UpdateMatrix();
}

void WorldObject::SetScale(float x, float y, float z)
{
    m_scale = { x, y, z };
    UpdateMatrix();
}

const DirectX::XMMATRIX& WorldObject::GetObjectWorldMatrix() const
{
    return m_objectWorldMatrix;
}

const DirectX::XMVECTOR& WorldObject::GetForwardVector()
{
    return m_currentForward;
}

const DirectX::XMVECTOR& WorldObject::GetUpVector()
{
    return m_currentUp;
}

const DirectX::XMVECTOR& WorldObject::GetDownVector()
{
    return m_currentDown;
}

const DirectX::XMVECTOR& WorldObject::GetBackVector()
{
    return m_currentBack;
}

const DirectX::XMVECTOR& WorldObject::GetLeftVector()
{
    return m_currentLeft;
}

const DirectX::XMVECTOR& WorldObject::GetRightVector()
{
    return m_currentRight;
}

const DirectX::XMFLOAT3& WorldObject::GetForwardF3()
{
    DirectX::XMFLOAT3 forwardF3{};
    DirectX::XMStoreFloat3(&forwardF3, m_currentForward);
    return forwardF3;
}

const DirectX::XMFLOAT3& WorldObject::GetUpF3()
{
    DirectX::XMFLOAT3 upF3{};
    DirectX::XMStoreFloat3(&upF3, m_currentUp);
    return upF3;
}

const DirectX::XMFLOAT3& WorldObject::GetDownF3()
{
    DirectX::XMFLOAT3 downF3{};
    DirectX::XMStoreFloat3(&downF3, m_currentDown);
    return downF3;
}

const DirectX::XMFLOAT3& WorldObject::GetBackF3()
{
    DirectX::XMFLOAT3 backF3{};
    DirectX::XMStoreFloat3(&backF3, m_currentBack);
    return backF3;
}

const DirectX::XMFLOAT3& WorldObject::GetLeftF3()
{
    DirectX::XMFLOAT3 leftF3{};
    DirectX::XMStoreFloat3(&leftF3, m_currentLeft);
    return leftF3;
}

const DirectX::XMFLOAT3& WorldObject::GetRightF3()
{
    DirectX::XMFLOAT3 rightF3{};
    DirectX::XMStoreFloat3(&rightF3, m_currentRight);
    return rightF3;
}

void WorldObject::UpdateMatrix()
{
    const DirectX::XMFLOAT3 rotation = GetRotation();
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.y, rotation.z, rotation.x);
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(m_position);
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

    m_objectWorldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

    m_currentForward = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotationMatrix);
    m_currentBack = DirectX::XMVector3TransformCoord(DEFAULT_BACK, rotationMatrix);
    m_currentUp = DirectX::XMVector3TransformCoord(DEFAULT_UP, rotationMatrix);
    m_currentDown = DirectX::XMVector3TransformCoord(DEFAULT_DOWN, rotationMatrix);
    m_currentLeft = DirectX::XMVector3TransformCoord(DEFAULT_LEFT, rotationMatrix);
    m_currentRight = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotationMatrix);
}