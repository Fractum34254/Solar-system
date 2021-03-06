#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"

class PointLight
{
public:
	PointLight( Graphics& gfx, DirectX::XMFLOAT3 color, float radius = 0.5f );
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Submit( class FrameCommander& frame ) const noxnd;
	void Bind( Graphics& gfx,DirectX::FXMMATRIX view ) const noexcept;
	void SetPos(DirectX::XMFLOAT3 newPos);
	DirectX::XMFLOAT3 GetPos() const noexcept;
	DirectX::XMFLOAT3 GetStartPos() const noexcept;
	void SetStartPos(DirectX::XMFLOAT3 newPos);
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	DirectX::XMFLOAT3 startPos;
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
};