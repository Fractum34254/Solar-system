#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "TestPlane.h"
#include "TestCube.h"
#include "Model.h"
#include <set>
#include <vector>
#include "ScriptCommander.h"
#include "Stencil.h"
#include "FrameCommander.h"
#include "Material.h"
#include "Planet.h"

class App
{
public:
	App( const std::string& commandLine = "" );
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
	void ShowImguiDemoWindow();
private:
	std::string commandLine;
	bool showDemoWindow = false;
	ImguiManager imgui;
	Window wnd;
	ScriptCommander scriptCommander;
	ChiliTimer timer;
	float speed_factor = 1.0f;
	float speed_factor_point = speed_factor;
	Camera cam;
	FrameCommander fc;
	PointLight light;
	double time = 0.0;
	//Radia
	std::vector<float> radia = {};
	//Controls
	bool accurate = true;
	bool real = true;
	bool blah = false;
	//Planets
	Planet mercury	{"Merkur", wnd.Gfx(), "Models\\merkur\\sphere.obj",	1.0f / 200.0f, 0.20563069, 0.38709888, 0.12225804517, 0.843546774485, 1.3518700794, 0.2408467};
	Planet venus	{"Venus", wnd.Gfx(), "Models\\venus\\sphere.obj",	1.0f / 200.0f, 0.00677323, 0.72333193, 0.05924886665, 1.3383305132,  2.295683575954 , 0.61519726 };
	Planet earth	{"Erde", wnd.Gfx(), "Models\\erde\\sphere.obj",	1.0f / 200.0f, 0.01671022, 1.0, 0.0, -0.196535243881, 1.796767421172, 1.0 };
	Planet mars		{"Mars", wnd.Gfx(), "Models\\mars\\sphere.obj",	1.0f / 200.0f, 0.09341233, 1.5236621, 0.03229923767, 0.86530876133, 5.865019079153, 1.8808476 };
	Planet jupiter	{"Jupiter", wnd.Gfx(), "Models\\jupiter\\sphere.obj", 1.0f / 200.0f, 0.04839266,  5.2033623,  0.022781782726,  1.755035900625, 0.257503259845, 11.862615 };
	Planet saturn	{"Saturn", wnd.Gfx(), "Models\\saturn\\sphere.obj",	1.0f / 200.0f, 0.05415060, 9.5370690, 0.043362007134, 1.984701857032, 1.613241687002, 29.447498 };
	Planet uranus	{"Uranus", wnd.Gfx(), "Models\\uranus\\sphere.obj",	1.0f / 200.0f, 0.04716771, 19.191261, 0.013436591779, 1.29555580936, 2.983888891162, 84.016846 };
	Planet neptune	{"Neptun", wnd.Gfx(), "Models\\neptun\\sphere.obj",	1.0f / 200.0f, 0.00858587, 30.068960, 0.030877841527, 2.298977186786, 0.784898126565, 164.79132 };
};