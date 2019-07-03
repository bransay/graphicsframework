#ifndef GF_APPLICATION_H_
#define GF_APPLICATION_H_

#include "graphicsframework/Config.h"

#include <string>

namespace GraphicsFramework
{
    class GF_DLL Application
    {
    public:
        // abstract interface to hook into the life cycle of an application
        class ILifeCycle
        {
        public:
            virtual void OnStart() = 0;
            virtual void OnEnd() = 0;
        };

        template <typename LifeCycle>
        class RegisterPrimary;

    public:
        Application(ILifeCycle& lifeCycle);
        virtual ~Application();

        void Start();
        void End();

        static Application* GetPrimary()
        {
            // get the primary application
            return ms_Primary;
        }
        
    private:
        ILifeCycle* m_LifeCycle;
        static Application* ms_Primary;
    };

    template <typename LifeCycle>
	class ApplicationImpl : public Application
	{
	public:
        ApplicationImpl()
            : Application(m_LifeCycleImpl)
        {
        }

    private:
        LifeCycle m_LifeCycleImpl;
	};

    template <typename LifeCycle>
    class Application::RegisterPrimary
    {
    public:
        RegisterPrimary()
        {
            if (!Application::ms_Primary)
                Application::ms_Primary = new ApplicationImpl<LifeCycle>();
        }
    };
}

#endif
