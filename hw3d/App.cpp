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

namespace dx = DirectX;

App::App( const std::string& commandLine )
	:
	commandLine( commandLine ),
	wnd( 1440,810,"The Solar System" ),
	scriptCommander( TokenizeQuoted( commandLine ) ),
	light( wnd.Gfx(), "Models\\sonne\\sphere.obj", 1.0f/190.0f )
{
	wnd.Gfx().SetProjection( dx::XMMatrixPerspectiveLH( 1.0f,9.0f / 16.0f,0.5f,400.0f ) );

	selected.resize(8, false);

	planets.emplace_back(std::move(std::make_unique<Planet>("Merkur", wnd.Gfx(), "Models\\merkur\\sphere.obj", 1.0f / 200.0f, 0.20563069, 0.38709888, 0.12225804517, 0.843546774485, 1.3518700794, 0.2408467, 58.646225, 0.0, 0.0 / 365.25 * 2.0 * PI, 20, DirectX::XMFLOAT3( 184.0f/255.0f, 134.0f / 255.0f,11.0f / 255.0f))));
	planets.emplace_back(std::move(std::make_unique<Planet>("Venus", wnd.Gfx(), "Models\\venus\\sphere.obj", 1.0f / 200.0f, 0.00677323, 0.72333193, 0.05924886665, 1.3383305132, 2.295683575954, 0.61519726, -243.0187, 3.09446876, 0.0 / 365.25 * 2.0 * PI, 40, DirectX::XMFLOAT3( 210.0f / 255.0f,105.0f / 255.0f,30.0f / 255.0f))));
	planets.emplace_back(std::move(std::make_unique<Planet>("Erde", wnd.Gfx(), "Models\\erde\\sphere.obj", 1.0f / 200.0f, 0.01671022, 1.0, 0.0, -0.196535243881, 1.796767421172, 1.0, 1.0, 0.40927971, 0.0 / 365.25 * 2.0 * PI, 60, DirectX::XMFLOAT3(0.0f / 255.0f,0.0f / 255.0f,255.0f / 255.0f))));
	planets.emplace_back(std::move(std::make_unique<Planet>("Mars", wnd.Gfx(), "Models\\mars\\sphere.obj", 1.0f / 200.0f, 0.09341233, 1.5236621, 0.03229923767, 0.86530876133, 5.865019079153, 1.8808476, 1.02595675, 0.439648439, 0.0 / 365.25 * 2.0 * PI, 80, DirectX::XMFLOAT3(255.0f / 255.0f, 69.0f / 255.0f, 0.0f / 255.0f))));
	planets.emplace_back(std::move(std::make_unique<Planet>("Jupiter", wnd.Gfx(), "Models\\jupiter\\sphere.obj", 1.0f / 200.0f, 0.04839266, 5.2033623, 0.022781782726, 1.755035900625, 0.257503259845, 11.862615, 0.41354, 0.054454273, 0.0 / 365.25 * 2.0 * PI, 100, DirectX::XMFLOAT3(255.0f / 255.0f,231.0f / 255.0f,186.0f / 255.0f))));
	planets.emplace_back(std::move(std::make_unique<Planet>("Saturn", wnd.Gfx(), "Models\\saturn\\sphere.obj", 1.0f / 200.0f, 0.05415060, 9.5370690, 0.043362007134, 1.984701857032, 1.613241687002, 29.447498, 0.44401, 0.466526509, 0.0 / 365.25 * 2.0 * PI, 120, DirectX::XMFLOAT3(255.0f / 255.0f, 165.0f / 255.0f,79.0f / 255.0f))));
	planets.emplace_back(std::move(std::make_unique<Planet>("Uranus", wnd.Gfx(), "Models\\uranus\\sphere.obj", 1.0f / 200.0f, 0.04716771, 19.191261, 0.013436591779, 1.29555580936, 2.983888891162, 84.016846, -0.71833, 1.70797921, 0.0 / 365.25 * 2.0 * PI, 140, DirectX::XMFLOAT3(224.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f))));
	planets.emplace_back(std::move(std::make_unique<Planet>("Neptun", wnd.Gfx(), "Models\\neptun\\sphere.obj", 1.0f / 200.0f, 0.00858587, 30.068960, 0.030877841527, 2.298977186786, 0.784898126565, 164.79132, 0.67125, 0.516268393, 0.0 / 365.25 * 2.0 * PI, 160, DirectX::XMFLOAT3(72.0f / 255.0f, 118.0f / 255.0f, 255.0f / 255.0f))));

	planets.at(0)->SetInfo("Erst 2 Sonden, Mariner 10 und Messenger, haben Merkur besucht und kartografiert.\nAufgrund 2km hoher Steilhänge wird vermutet, dass der gesamte Planet geschrumpft ist.");
	planets.at(1)->SetInfo("Auf der Venus herrscht 90-facher Erdluftdruck.\nTrotzdem wird spekuliert, ob eventuell Bakterien in den heißen Wolken leben.");
	planets.at(2)->SetInfo("Unser Heimatplanet. Einzigartig im Universum.\nVor 3.5 Milliarden Jahren entstanden hier die ersten Formen allen uns bekannten Lebens.");

}

void App::DoFrame()
{
	//Frame setup
	const auto cam_dt = timer.Mark();
	const auto dt = cam_dt * speed_factor * constant_speed_factor;
	time += (double)dt;
	wnd.Gfx().BeginFrame( 0.07f,0.0f,0.12f );
	wnd.Gfx().SetCamera( cam.GetMatrix() );
	light.Bind( wnd.Gfx(),cam.GetMatrix() );
	ResolveKeyboard(cam_dt);
	ResolveMouse();

	//Calculate positions & rotations
	for (size_t i = 0; i < planets.size(); i++)
	{
		//1s * constant_speed_factor = 1a
		planets.at(i)->CalculatePosition(time * 2.0 * PI_D);
		if (rotation)
		{
			//input in siderischen Tagen
			planets.at(i)->CalculateRotation(time * 366.25);
		}
	}

	//Test for clicking
	if (left_down && wnd.CursorEnabled())
	{
		DirectX::XMFLOAT3 dir;
		DirectX::XMStoreFloat3(&dir, GetClickVector({ (float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY(), 0.0f }));
		const DirectX::XMFLOAT3 camPos = cam.GetPos();
		const float adbecf = camPos.x * dir.x + camPos.y * dir.y + camPos.z * dir.z;
		const float def = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;

		std::vector<std::pair<float, float>> diff;
		std::vector<bool> focused;

		for (size_t i = 0; i < planets.size(); i++)
		{
			const DirectX::XMFLOAT3 pos = planets.at(i)->GetPosition();
			const float r = (pos.x * dir.x + pos.y * dir.y + pos.z * dir.z - adbecf ) / (def);
			const DirectX::XMVECTOR point = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&camPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&dir), r));
			DirectX::XMFLOAT3 length;
			DirectX::XMStoreFloat3(&length, DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pos), point)));
			diff.emplace_back( length.x, r );
		}

		std::for_each(diff.begin(), diff.end(), [&focused](std::pair<float, float> p) {focused.emplace_back((p.first < 0.25f) && (p.second >= 0.0f)); });

		if (std::any_of(focused.begin(), focused.end(), [](const bool& b) {return b;}))
		{
			for (size_t i = 0; i < selected.size(); i++)
			{
				selected.at(i) = (selected.at(i) != focused.at(i));
			}
		}
		else
		{
			for (size_t i = 0; i < selected.size(); i++)
			{
				selected.at(i) = false;
			}
		}
		clicked = true;
	}

	//Highlight the clicked planet
	if (clicked)
	{
		for (size_t i = 0; i < planets.size(); i++)
		{
			if (selected.at(i))
			{
				planets.at(i)->Highlight();
			}
			else
			{
				planets.at(i)->DeHighlight();
			}
		}
		clicked = false;
	}

	//Show Info Window of Highlighted planet
	for (size_t i = 0; i < planets.size(); i++)
	{
		if (selected.at(i))
		{
			planets.at(i)->SpawnInfoWindow();
		}
	}

	//Apply transforms
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets.at(i)->ApplyTransforms();
	}

	//Submit draws
	light.Submit(fc);
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets.at(i)->Submit(fc);
	}
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets.at(i)->SubmitHighlighted(fc);
	}
	fc.Execute( wnd.Gfx() );

	if (speed_factor != 0.0f && former_speed_factor != 0.0f)
	{
		former_speed_factor = 0.0f;
	}
	

	// imgui windows
	SpawnControlWindow();

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

void App::SpawnControlWindow()
{
	//Imgui control window
	if (ImGui::Begin("Steuerung"))
	{
		int days = (int)(time * 365.25);
		int years = 2000;
		while (days <= 0)
		{
			years--;
			if (years % 4 == 0)
			{
				days += 366;
			}
			else
			{
				days += 365;
			}
		}
		while (days > 366)
		{
			if (years % 4 == 0)
			{
				days -= 366;
			}
			else
			{
				days -= 365;
			}
			years++;
		}
		if (days == 366)
		{
			if (years % 4 != 0)
			{
				days -= 365;
				years++;
			}
		}
		int months = 1;
		int monthDays = 31; ///January
		for (; days > monthDays; months++)
		{
			days -= monthDays;
			monthDays = (monthsCount[months] + ((((years % 4) == 0) && (months == 1)) ? 1 : 0));
		}

		ImGui::PushStyleColor(NULL, { 1.0f, 1.0f, 1.0f, 1.0f });
		std::string t = "Datum:";
		t += std::to_string(days) + "." + std::to_string(months) + "." + std::to_string(years);
		ImGui::Text(t.c_str());
		if (ImGui::TreeNodeEx((void*)(intptr_t)0, 0, "Bahnradien"))
		{
			for (size_t i = 0; i < planets.size(); i++)
			{
				if (ImGui::SliderFloat(planets.at(i)->GetName().c_str(), planets.at(i)->GetRadiusScale(), 0.1f, 10.0f, "%.1f"))
				{
					planets.at(i)->RescaleEllipse();
				}
			}
			if (ImGui::Checkbox("Reale Radien?", &real))
			{
				if (real)
				{
					for (size_t i = 0; i < planets.size(); i++)
					{
						*planets.at(i)->GetRadiusScale() = 2.0f;
						planets.at(i)->RescaleEllipse();
					}
				}
				else
				{
					for (size_t i = 0; i < planets.size(); i++)
					{
						*planets.at(i)->GetRadiusScale() = (float)(i + 1) / (float)planets.at(i)->GetA();
						planets.at(i)->RescaleEllipse();
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
				ImGui::SliderFloat("Faktor", &speed_factor, speed_factor_point - 10.0f, speed_factor_point + 10.0f, "%.1f");
			}
			else
			{
				ImGui::SliderFloat("Faktor", &speed_factor, -5.0f / constant_speed_factor, 5.0f / constant_speed_factor, "%1.0f");
				speed_factor_point = speed_factor;
			}
			ImGui::TreePop();
		}
		ImGui::Checkbox("Rotation?", &rotation);
	}
	ImGui::PopStyleColor();
	ImGui::End();
}

DirectX::XMVECTOR App::GetClickVector(DirectX::XMFLOAT3 pixel) const
{
	const DirectX::XMVECTOR worldSpace = DirectX::XMVector3Unproject(DirectX::XMLoadFloat3(&pixel), 0.0f, 0.0f, (float)wnd.GetWidth(), (float)wnd.GetHeight(), 0.0f, 1.0f, wnd.Gfx().GetProjection(), wnd.Gfx().GetCamera(), DirectX::XMMatrixIdentity());
	return DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(worldSpace, DirectX::XMLoadFloat3(&cam.GetPos())));
}

void App::ResolveKeyboard(float dt)
{
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_SPACE:
			if (wnd.CursorEnabled())
			{
				std::swap(speed_factor, former_speed_factor);
				speed_factor_point = speed_factor;
			}
			break;
		case VK_LEFT:
			time -= 1.0f / 365.25f;
			break;
		case VK_RIGHT:
			time += 1.0f / 365.25f;
			break;
		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cam.TranslateWithoutPitch({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.TranslateWithoutPitch({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			cam.TranslateWithoutPitch({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		{
			cam.TranslateWithoutPitch({ 0.0f,-dt,0.0f });
		}
	}
}

void App::ResolveMouse()
{
	left_down = false;
	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}
	while (const auto& e = wnd.mouse.Read())
	{
		if (e)
		{
			if (e->GetType() == Mouse::Event::Type::LPress)
			{
				left_down = true;
			}
		}
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