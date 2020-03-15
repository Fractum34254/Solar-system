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
#include "LightMesh.h"
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
	void ResolveKeyboard(float dt);
	void ResolveMouse();
private:
	std::string commandLine;
	bool showDemoWindow = false;
	ImguiManager imgui;
	Window wnd;
	ScriptCommander scriptCommander;
	ChiliTimer timer;
	float speed_factor = 1.0f;
	float constant_speed_factor = 0.001f;
	float speed_factor_point = speed_factor;
	float former_speed_factor = 0.0f;
	Camera cam;
	FrameCommander fc;
	LightMesh light;
	bool clicked = false;
	bool left_down = false;
	double time = 0.0;
	//Controls
	bool accurate = true;
	bool real = true;
	bool rotation = true;
	int monthsCount[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	//Planets
	std::vector<std::unique_ptr<Planet>> planets;
	std::vector<bool> selected;
};