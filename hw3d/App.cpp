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
	for (int i = 0; i < 8; i++)
	{
		radia.push_back(2.0f);
	}
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
			ImGui::SliderFloat("Merkur", &(radia.at(0)), 0.1f, 10.0f, "%.1f");
			ImGui::SliderFloat("Venus", &(radia.at(1)), 0.1f, 10.0f, "%.1f");
			ImGui::SliderFloat("Erde", &(radia.at(2)), 0.1f, 10.0f, "%.1f");
			ImGui::SliderFloat("Mars", &(radia.at(3)), 0.1f, 10.0f, "%.1f");
			ImGui::SliderFloat("Jupiter", &(radia.at(4)), 0.1f, 10.0f, "%.1f");
			ImGui::SliderFloat("Saturn", &(radia.at(5)), 0.1f, 10.0f, "%.1f");
			ImGui::SliderFloat("Uranus", &(radia.at(6)), 0.1f, 10.0f, "%.1f");
			ImGui::SliderFloat("Neptun", &(radia.at(7)), 0.1f, 10.0f, "%.1f");
			if (ImGui::Checkbox("Reale Radien?", &real))
			{
				if (real)
				{
					for (int i = 0; i < 8; i++)
					{
						radia.at(i) = 2.0f;
					}
				}
				else
				{
					radia.at(0) = 1.0f / ((float)mercury.GetA());
					radia.at(1) = 2.0f / ((float)venus.GetA());
					radia.at(2) = 3.0f / ((float)earth.GetA());
					radia.at(3) = 4.0f / ((float)mars.GetA());
					radia.at(4) = 5.0f / ((float)jupiter.GetA());
					radia.at(5) = 6.0f / ((float)saturn.GetA());
					radia.at(6) = 7.0f / ((float)uranus.GetA());
					radia.at(7) = 8.0f / ((float)neptune.GetA());
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
		if (ImGui::TreeNodeEx((void*)(intptr_t)2, 0, "Informationen"))
		{
			ImGui::Checkbox("Erde - ja", &blah);
			if (blah)
			{
				earth.SpawnInfoWindow();
				earth.Highlight();
			}
			else
			{
				earth.DeHighlight();
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();

	//Radia update
	mercury.SetRadiusScale(radia.at(0));
	venus.SetRadiusScale(radia.at(1));
	earth.SetRadiusScale(radia.at(2));
	mars.SetRadiusScale(radia.at(3));
	jupiter.SetRadiusScale(radia.at(4));
	saturn.SetRadiusScale(radia.at(5));
	uranus.SetRadiusScale(radia.at(6));
	neptune.SetRadiusScale(radia.at(7));


	//Calculate positions
	mercury.CalculatePosition(time);
	venus.CalculatePosition(time);
	earth.CalculatePosition(time);
	mars.CalculatePosition(time);
	jupiter.CalculatePosition(time);
	saturn.CalculatePosition(time);
	uranus.CalculatePosition(time);
	neptune.CalculatePosition(time);


	//Submit draws
	mercury.Submit(fc);
	venus.Submit(fc);
	earth.Submit(fc);
	mars.Submit(fc);
	jupiter.Submit(fc);
	saturn.Submit(fc);
	uranus.Submit(fc);
	neptune.Submit(fc);
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