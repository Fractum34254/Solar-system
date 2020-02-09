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

	mercury.SetPos({ 10.0f, 0.0f, 0.0f });
	venus.SetPos({ 10.0f, 0.0f, 10.0f });
	earth.SetPos({ 0.0f, 0.0f, 10.0f });
	mars.SetPos({ -10.0f, 0.0f, 10.0f });
	jupiter.SetPos({ -10.0f, 0.0f, 0.0f });
	saturn.SetPos({ -10.0f, 0.0f, -10.0f });
	uranus.SetPos({ 0.0f, 0.0f, -10.0f });
	neptune.SetPos({ 10.0f, 0.0f, -10.0f });
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame( 0.07f,0.0f,0.12f );
	wnd.Gfx().SetCamera( cam.GetMatrix() );
	light.Bind( wnd.Gfx(),cam.GetMatrix() );

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

	Controller::ResolveKeyboard(wnd, cam, dt);
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