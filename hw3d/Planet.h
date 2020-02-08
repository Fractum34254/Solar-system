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
private:
	Model sphere;
};
