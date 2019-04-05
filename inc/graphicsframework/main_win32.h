#ifndef GF_MAIN_WIN32_H_
#define GF_MAIN_WIN32_H_

#include "graphicsframework/Config.h"
#include "graphicsframework/Application.h"

#ifdef WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    {
        using namespace GraphicsFramework;
        if (auto application = Application::GetPrimary())
        {
            application->Start();
            application->End();
        }
        else
        {
            // TODO: error handling?
        }
    }

	return 0;
}
#endif

#endif
