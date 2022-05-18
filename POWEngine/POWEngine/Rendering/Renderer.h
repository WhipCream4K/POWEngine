#pragma once
#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
	struct Archetype;
	class RendererImpl;
	class Window;
	class RenderSystem;
	class Renderer
	{
	public:

		Renderer(uint32_t width, uint32_t height,const OtherWindowParams& other = {});
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		virtual ~Renderer() = default;
		virtual void DrawBufferOnWindow(const Window& window) const = 0;

		void RegisterSystem(const SharedPtr<RenderSystem>& system);
		void RemoveSystem(const SharedPtr<RenderSystem>& system);
		void UpdateSystem(const SharedPtr<Archetype>& archetype);


	private:

		std::vector<SharedPtr<RenderSystem>> m_RenderSystems;
	};
}

