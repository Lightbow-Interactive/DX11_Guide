#pragma once

#include <DirectXMath.h>

class WorldObject
{
public:
    const DirectX::XMFLOAT3& GetPosition() const;
    void SetPosition(const DirectX::XMFLOAT3& position);
    void SetPosition(float x, float y, float z);
    void AddLocalOffset(float x, float y, float z);
    void AddLocalOffset(const DirectX::XMFLOAT3& oF3);
    void AddLocalOffset(const DirectX::XMVECTOR& oVector);

    const DirectX::XMFLOAT3& GetRotation() const;
    void SetRotation(const DirectX::XMFLOAT3& rotation);
    void SetRotation(float roll, float pitch, float yaw);
    void AddRotationOffset(float roll, float pitch, float yaw);

    const DirectX::XMFLOAT3& GetScale() const;
    void SetScale(const DirectX::XMFLOAT3& scale);
    void SetScale(float x, float y, float z);

    const DirectX::XMMATRIX& GetObjectWorldMatrix() const;

    const DirectX::XMVECTOR& GetForwardVector();
    const DirectX::XMVECTOR& GetUpVector();
    const DirectX::XMVECTOR& GetDownVector();
    const DirectX::XMVECTOR& GetBackVector();
    const DirectX::XMVECTOR& GetLeftVector();
    const DirectX::XMVECTOR& GetRightVector();

    const DirectX::XMFLOAT3& GetForwardF3();
    const DirectX::XMFLOAT3& GetUpF3();
    const DirectX::XMFLOAT3& GetDownF3();
    const DirectX::XMFLOAT3& GetBackF3();
    const DirectX::XMFLOAT3& GetLeftF3();
    const DirectX::XMFLOAT3& GetRightF3();

    virtual void UpdateMatrix();

protected:
    DirectX::XMMATRIX m_objectWorldMatrix = DirectX::XMMatrixIdentity();

    const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
    const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
    const DirectX::XMVECTOR DEFAULT_DOWN = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);
    const DirectX::XMVECTOR DEFAULT_BACK = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
    const DirectX::XMVECTOR DEFAULT_LEFT = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
    const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);

    DirectX::XMVECTOR m_currentForward = DEFAULT_FORWARD;
    DirectX::XMVECTOR m_currentUp = DEFAULT_UP;
    DirectX::XMVECTOR m_currentDown = DEFAULT_DOWN;
    DirectX::XMVECTOR m_currentBack = DEFAULT_BACK;
    DirectX::XMVECTOR m_currentLeft = DEFAULT_LEFT;
    DirectX::XMVECTOR m_currentRight = DEFAULT_RIGHT;

    DirectX::XMVECTOR m_position = DirectX::XMVectorZero();
    DirectX::XMVECTOR m_rotation = DirectX::XMVectorZero();
    DirectX::XMFLOAT3 m_scale = { 1.f, 1.f, 1.f };

};