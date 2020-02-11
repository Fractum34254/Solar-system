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
	void SpawnControlWindow();
	DirectX::XMVECTOR GetClickVector(DirectX::XMFLOAT3 pixel) const;
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
	int selected = -1;
	bool clicked = false;
	double time = 0.0;
	//Controls
	bool accurate = true;
	bool real = true;
	bool blah = false;
	//Planets
	std::vector<std::unique_ptr<Planet>> planets;
};