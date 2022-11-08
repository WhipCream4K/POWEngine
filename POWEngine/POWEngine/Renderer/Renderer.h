#pragma once
#include <SFML/OpenGL.hpp>

#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

namespace powe
{
	struct Archetype;
	class RendererImpl;
	class Window;
	class RenderSystemBase;
	class RenderAPI;
	class WorldEntity;
	class Renderer
	{
	public:

		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		~Renderer();


	public:

		void Draw(const Window& window) const;

		void RegisterSystem(const SharedPtr<RenderSystemBase>& system);
		void RemoveSystem(const SharedPtr<RenderSystemBase>& system);

		void UpdateSystem(
			const WorldEntity& worldEntity,
			const std::unordered_map<std::string, SharedPtr<Archetype>>& archetypePool) const;

		void UpdateSystem(
			const WorldEntity& worldEntity,
			const Window& renderWindow,
			const std::unordered_map<std::string,SharedPtr<Archetype>>& archetypePool
			) const;

		void RegisterRenderAPI(OwnedPtr<RenderAPI>&& renderInst);


	private:

		OwnedPtr<RenderAPI> m_RenderAPI;
		std::vector<SharedPtr<RenderSystemBase>> m_RenderSystems;
	};
}

