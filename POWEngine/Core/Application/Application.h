#pragma once

#include "AppDesc.h"
#include "Core/Memory/Allocator.h"
#include "Utils/Utils.h"
#include "Core/Logger/Logger.h"
#include "Utils/ServiceLocator.h"
#include "Core/Thread/SimpleThreadPool.h"
#include "Core/Layer.h"
#include "Core/Clock/Clock.h"


namespace powe
{
	class Core;
	class Layer;
	class WindowManager;

	using ServiceLocatorT = ServiceLocator<Logger,SimpleThreadPool>;

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
		T* GetAppService() { return m_ServiceLocator->GetService<T>(); }

	private:

		// Allocator need to initialize first

		Vector<UniquePtr<Layer>> m_LayerStack;
		Clock m_Clock;
		UniquePtr<WindowManager> m_WindowManager;
		UniquePtr<ServiceLocatorT> m_ServiceLocator;
		AppDesc m_AppDesc;

		static Application* m_Instance;
	};
	template<LayerConcept T>
	inline Layer* Application::PushLayer(T&& layer)
	{
		auto layerPtr = AllocateUnique<T>(DefaultAllocator::Application, std::forward<T>(layer));
		m_LayerStack.emplace_back(std::move(layerPtr));
		m_LayerStack.back()->OnAttach();
		return m_LayerStack.back().get();
	}

}
