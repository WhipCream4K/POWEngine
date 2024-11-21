#pragma once

#include "AppDesc.h"
#include "Utils/Utils.h"
#include "Logger/Logger.h"
#include "Utils/ServiceLocator.h"
#include "Core/Thread/SimpleThreadPool.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Layer.h"
#include "Core/Clock/Clock.h"


namespace powe
{
	class Core;
	class Layer;
	class WindowManager;

	using ServiceLocatorT = ServiceLocator<Logger,SimpleThreadPool,MemoryManager>;

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

		template<LayerConcept T>
		Layer* PushLayer(T&& layer);
		void PopLayer();

		void Run();
		
		WindowManager& GetWindowManager() const { return *m_WindowManager; }
		
		template<typename T>
		static T* GetAppService() 
		{ 
			return m_Instance->GetAppService<T>();
		}

	private:

		// Allocator need to initialize first

		Vector<UniquePtr<Layer>> m_LayerStack;
		Clock m_Clock;
		UniquePtr<WindowManager> m_WindowManager;
		UniquePtr<ServiceLocatorT> m_ServiceLocator;
		AppDesc m_AppDesc;
		PMRResource* m_AppAllocator;

		static Application* m_Instance;
	};
	template<LayerConcept T>
	Layer* Application::PushLayer(T&& layer)
	{
		auto layerPtr = AllocateUnique<T>(m_AppAllocator, std::forward<T>(layer));
		m_LayerStack.emplace_back(std::move(layerPtr));
		m_LayerStack.back()->OnAttach();
		return m_LayerStack.back().get();
	}

}
