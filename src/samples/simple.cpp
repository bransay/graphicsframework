#include "graphicsframework/EntryPoint.h"

class SimpleLifeCycle : public GraphicsFramework::Application::ILifeCycle
{
public:
    virtual void OnStart() override
    {
        OutputDebugString(L"START!\n");
    }

    virtual void OnEnd() override
    {
        OutputDebugString(L"END!\n");
    }
};

// Register the lifecycle
GraphicsFramework::Application::RegisterPrimary<SimpleLifeCycle> lifeCycle;
