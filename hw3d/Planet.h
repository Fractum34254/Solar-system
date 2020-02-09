#pragma once
#include "Model.h"
#include "FrameCommander.h"

class Planet
{
public:
	Planet(Graphics& gfx, const std::string& pathString, float scale = 1.0f)
		:
		sphere(gfx, pathString, scale)
	{}
	void Submit(FrameCommander& fc) const
	{
		sphere.Submit(fc);
	}
	void SetPos(const DirectX::XMFLOAT3& pos)
	{
		sphere.SetRootTransform(DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos)));
	}
private:
	Model sphere;
};
