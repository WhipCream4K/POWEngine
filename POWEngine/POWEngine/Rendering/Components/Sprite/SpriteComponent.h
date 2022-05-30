#pragma once

//#include "POWEngine/Rendering/2D/Draw2DObject.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Math/Math.h"

namespace powe
{

	class Texture;
	class SpriteImpl;
	class GameObject;
	class SpriteComponent : public Component<SpriteComponent>
	{
	public:

		SpriteComponent();
		SpriteComponent(const SharedPtr<GameObject>& gameObject);
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

		void SetRenderOrder(int order) const;
		int GetRenderOrder() const;

		glm::fvec2 GetOrigin() const;

		template<typename T>
		T* GetTypeInstance() const;

	private:

		OwnedPtr<SpriteImpl> m_SpriteImpl;
		WeakPtr<GameObject> m_Owner;
	};

	template <typename T>
	T* SpriteComponent::GetTypeInstance() const
	{
		return static_cast<T*>(m_SpriteImpl.get());
	}
}


