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
	Camera cam;
	FrameCommander fc;
	PointLight light;
	//Planets
	Planet mercury{ wnd.Gfx(), "Models\\merkur\\sphere.obj", 1.0f / 20.0f };
	Planet venus{ wnd.Gfx(), "Models\\venus\\sphere.obj", 1.0f / 20.0f };
	Planet earth{ wnd.Gfx(), "Models\\erde\\sphere.obj", 1.0f/20.0f};
	Planet mars{ wnd.Gfx(), "Models\\mars\\sphere.obj", 1.0f / 20.0f };
	Planet jupiter{ wnd.Gfx(), "Models\\jupiter\\sphere.obj", 1.0f / 20.0f };
	Planet saturn{ wnd.Gfx(), "Models\\saturn\\sphere.obj", 1.0f / 20.0f };
	Planet uranus{ wnd.Gfx(), "Models\\uranus\\sphere.obj", 1.0f / 20.0f };
	Planet neptune{ wnd.Gfx(), "Models\\neptun\\sphere.obj", 1.0f / 20.0f };
};