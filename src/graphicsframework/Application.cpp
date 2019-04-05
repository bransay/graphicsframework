#include "graphicsframework/Application.h"

namespace GraphicsFramework
{
    Application* Application::ms_Primary = nullptr;

    Application::Application(ILifeCycle& lifeCycle)
        : m_LifeCycle(&lifeCycle)
    {
    }

    Application::~Application()
    {
    }

    void Application::Start()
    {
        m_LifeCycle->OnStart();
    }

    void Application::End()
    {
        m_LifeCycle->OnEnd();
    }
}
