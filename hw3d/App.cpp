#include "App.h"
#include <memory>
#include <algorithm>
#include "ChiliMath.h"
#include "Surface.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"
#include "ChiliUtil.h"
#include "Testing.h"
#include "PerfLog.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "DynamicConstant.h"
#include "ModelProbe.h"
#include "Node.h"
#include "ChiliXM.h"
#include "Controller.h"

namespace dx = DirectX;

App::App( const std::string& commandLine )
	:
	commandLine( commandLine ),
	wnd( 1280,720,"The Solar System" ),
	scriptCommander( TokenizeQuoted( commandLine ) ),
	light( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( dx::XMMatrixPerspectiveLH( 1.0f,9.0f / 16.0f,0.5f,400.0f ) );

	planets.emplace_back(std::move(std::make_unique<Planet>("Merkur", wnd.Gfx(), "Models\\merkur\\sphere.obj",	1.0f / 200.0f, 0.20563069, 0.38709888, 0.12225804517, 0.843546774485, 1.3518700794, 0.2408467 )));
	planets.emplace_back(std::move(std::make_unique<Planet>("Venus", wnd.Gfx(), "Models\\venus\\sphere.obj",	1.0f / 200.0f, 0.00677323, 0.72333193, 0.05924886665, 1.3383305132,  2.295683575954 , 0.61519726 )));
	planets.emplace_back(std::move(std::make_unique<Planet>("Erde", wnd.Gfx(), "Models\\erde\\sphere.obj",	1.0f / 200.0f, 0.01671022, 1.0, 0.0, -0.196535243881, 1.796767421172, 1.0 )));
	planets.emplace_back(std::move(std::make_unique<Planet>("Mars", wnd.Gfx(), "Models\\mars\\sphere.obj",	1.0f / 200.0f, 0.09341233, 1.5236621, 0.03229923767, 0.86530876133, 5.865019079153, 1.8808476 )));
	planets.emplace_back(std::move(std::make_unique<Planet>("Jupiter", wnd.Gfx(), "Models\\jupiter\\sphere.obj", 1.0f / 200.0f, 0.04839266,  5.2033623,  0.022781782726,  1.755035900625, 0.257503259845, 11.862615 )));
	planets.emplace_back(std::move(std::make_unique<Planet>("Saturn", wnd.Gfx(), "Models\\saturn\\sphere.obj",	1.0f / 200.0f, 0.05415060, 9.5370690, 0.043362007134, 1.984701857032, 1.613241687002, 29.447498 )));
	planets.emplace_back(std::move(std::make_unique<Planet>("Uranus", wnd.Gfx(), "Models\\uranus\\sphere.obj",	1.0f / 200.0f, 0.04716771, 19.191261, 0.013436591779, 1.29555580936, 2.983888891162, 84.016846 )));
	planets.emplace_back(std::move(std::make_unique<Planet>("Neptun", wnd.Gfx(), "Models\\neptun\\sphere.obj",	1.0f / 200.0f, 0.00858587, 30.068960, 0.030877841527, 2.298977186786, 0.784898126565, 164.79132 )));

}

void App::DoFrame()
{
	//Frame setup
	const auto cam_dt = timer.Mark();
	const auto dt = cam_dt * speed_factor;
	time += (double)dt;
	wnd.Gfx().BeginFrame( 0.07f,0.0f,0.12f );
	wnd.Gfx().SetCamera( cam.GetMatrix() );
	light.Bind( wnd.Gfx(),cam.GetMatrix() );

	//Imgui control window
	if (ImGui::Begin("Steuerung"))
	{
		if (ImGui::TreeNodeEx((void*)(intptr_t)0, 0, "Bahnradien"))
		{
			for (size_t i = 0; i < planets.size(); i++)
			{
				ImGui::SliderFloat(planets.at(i)->GetName().c_str(), planets.at(i)->GetRadiusScale(), 0.1f, 10.0f, "%.1f");
			}
			if (ImGui::Checkbox("Reale Radien?", &real))
			{
				if (real)
				{
					for (size_t i = 0; i < planets.size(); i++)
					{
						*planets.at(i)->GetRadiusScale() = 2.0f;
					}
				}
				else
				{
					for (size_t i = 0; i < planets.size(); i++)
					{
						*planets.at(i)->GetRadiusScale() = (float)(i + 1) / (float)planets.at(i)->GetA();
					}
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx((void*)(intptr_t)1, 0, "Geschwindigkeit"))
		{
			ImGui::Checkbox("Exakt?", &accurate);
			if (accurate)
			{
				ImGui::SliderFloat("Faktor", &speed_factor, speed_factor_point-3.0f, speed_factor_point+3.0f, "%.1f");
			}
			else
			{
				ImGui::SliderFloat("Faktor", &speed_factor, -50.0f, 50.0f, "%1.0f");
				speed_factor_point = speed_factor;
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
	

	//Calculate positions
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets.at(i)->CalculatePosition(time);
	}


	//Submit draws
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets.at(i)->Submit(fc);
	}
	light.Submit( fc );
	fc.Execute( wnd.Gfx() );

	Controller::ResolveKeyboard(wnd, cam, cam_dt / 2.0f);
	Controller::ResolveMouse(wnd, cam);
	

	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();

	// present
	wnd.Gfx().EndFrame();
	fc.Reset();
}

void App::ShowImguiDemoWindow()
{
	if( showDemoWindow )
	{
		ImGui::ShowDemoWindow( &showDemoWindow );
	}
}

App::~App()
{}


int App::Go()
{
	while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}