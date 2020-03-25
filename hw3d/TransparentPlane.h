#pragma once
#include "Drawable.h"

class TransparentPlane : public Drawable
{
public:
	TransparentPlane(Graphics& gfx, std::vector<DirectX::XMFLOAT3> vertices, DirectX::XMFLOAT3 color);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRootTransform(DirectX::XMMATRIX matrix) noexcept;
	void AddRootTransform(DirectX::XMMATRIX matrix) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
};