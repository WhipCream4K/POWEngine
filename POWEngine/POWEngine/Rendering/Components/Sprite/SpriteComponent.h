#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Math/Math.h"

namespace powe
{
	class Texture;
	class SpriteImpl;
	class SpriteComponent : public Component<SpriteComponent>
	{
	public:

		SpriteComponent();
		SpriteComponent(WorldEntity* worldEntity,GameObjectID id);
		SpriteComponent(const SpriteComponent&) = delete;
		SpriteComponent& operator=(const SpriteComponent&) = delete;
		SpriteComponent(SpriteComponent&&) noexcept;
		SpriteComponent& operator=(SpriteComponent&&) noexcept;
		~SpriteComponent() override;

	public:


		void SetTexture(const Texture& texture) const;
		void SetRect(const glm::fvec4& rect) const;
		void SetOrigin(float x, float y) const;
		void SetOrigin(const glm::fvec2& pos) const;
		float GetZDepth() const;

		template<typename T>
		T* GetTypeInstance() const;

	private:

		//OwnedPtr<SpriteImpl> m_SpriteImpl;
		SharedPtr<SpriteImpl> m_SpriteImpl;
		float m_ZDepth{};
	};

	template <typename T>
	T* SpriteComponent::GetTypeInstance() const
	{
		return static_cast<T*>(m_SpriteImpl.get());
	}
}


