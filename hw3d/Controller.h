#pragma once
#include "Window.h"
#include "Camera.h"

class Controller
{
public:
	static void ResolveKeyboard(Window& wnd, Camera& cam, float dt)
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
	static void ResolveMouse(Window& wnd, Camera& cam)
	{
		while (const auto delta = wnd.mouse.ReadRawDelta())
		{
			if (!wnd.CursorEnabled())
			{
				cam.Rotate((float)delta->x, (float)delta->y);
			}
		}
	}
};
