#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"
#include "Model.h"

class LightMesh
{
public:
	LightMesh(Graphics& gfx, const std::string& pathString, float scale = 1.0f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Submit(class FrameCommander& frame) const noxnd;
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
	void SetPos(DirectX::XMFLOAT3 newPos);
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
	PointLightCBuf cbData;
	mutable Model model;
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
};
