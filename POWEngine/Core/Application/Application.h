#pragma once

#include "AppDesc.h"
#include "Core/CustomTypes.h"
#include "Core/ModulesManager.h"
#include "Core/Clock/Clock.h"
#include "AppEventSetup.h"

namespace powe
{
	class Core;
	class WindowManager;
	class AppEvent;
	class Window;
	class InputManager;
	class Application final
	{
	public:

		Application(const AppDesc& desc);
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
		~Application();
		
		static Application& Get() { return *m_Instance; }

		template<ModuleConcept T>
		static SharedPtr<T> GetModule() 
		{ 
			return m_Instance->m_AppModules->GetModule<T>();
		}

		void Run();
		
		ModulesManager& GetModulesManager() const noexcept { return *m_AppModules; }

		Window* GetAppWindow() const noexcept { return m_AppWindow; }

		void SetAppEventSetup(const std::function<void(AppEventSetup&)>& setup) noexcept { m_AppEventSetupLogic = setup; }

		void RegisterAppEvent(const SharedPtr<AppEvent>& appEvent) noexcept;

		bool IsInMainThread() const noexcept { return std::this_thread::get_id() == m_AppThreadID; }

	private:

		// App Events
		Vector<SharedPtr<AppEvent>> m_AppEvents;
		std::function<void(AppEventSetup&)> m_AppEventSetupLogic;

		// Core components
		Clock m_Clock;
		UniquePtr<WindowManager> m_WindowManager;
		UniquePtr<InputManager> m_InputManager;
		UniquePtr<ModulesManager> m_AppModules;

		Window* m_AppWindow;
		AppDesc m_AppDesc;
		PMRResource* m_AppAllocator;
		std::thread::id m_AppThreadID;

		static Application* m_Instance;
	};

}
