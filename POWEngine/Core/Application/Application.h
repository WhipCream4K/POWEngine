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
		virtual ~Application() = default;

		template<LayerConcept T>
		Layer* PushLayer(T&& layer);
		void PopLayer();

		void Run();

		static Application& Get() { return *m_Instance; }

		ServiceLocatorT& GetServiceLocator() { return *m_ServiceLocator; }
		WindowManager& GetWindowManager() { return *m_WindowManager; }


	private:

		// Allocator need to initilize first
		TrackableAllocator m_TrackAllocator;

		Vector<UniquePtr<Layer>> m_LayerStack;
		Clock m_Clock;
		UniquePtr<WindowManager> m_WindowManager;
		UniquePtr<ServiceLocatorT> m_ServiceLocator;

		static Application* m_Instance;
	};
	template<LayerConcept T>
	inline Layer* Application::PushLayer(T&& layer)
	{
		auto layerPtr = AllocateUnique<T>(std::move(layer),&m_TrackAllocator);
		m_LayerStack.emplace_back(std::move(layerPtr));
		m_LayerStack.back()->OnAttach();
		return m_LayerStack.back();
	}

}
