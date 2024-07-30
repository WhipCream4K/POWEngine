#pragma once


#include "POWEngine/Renderer/Components/Sprite/SpriteImpl.h"


#include <SFML/Graphics.hpp>

namespace powe
{
	struct SFMLSpriteComponent;
	class GameObject;
	class SFMLSprite : public SpriteImpl
	{
	public:

		SFMLSprite();
		SFMLSprite(const SharedPtr<GameObject>& owner);
		~SFMLSprite() override;
		void SetOrigin(float x, float y) override;
		void SetTexture(const Texture& texture) override;
		void SetOrigin(const glm::fvec2& pos) override;
		glm::fvec2 GetOrigin() const override;
		void SetRect(const glm::fvec4& rect) override;
		SFMLSpriteComponent* GetSfSprite() const;
		int GetRenderOrder() const override;
		void SetRenderOrder(int order) override;
		void SetTint(const glm::uvec4&) override;
		void Destroy(powe::WorldEntity&) override;

	private:

		WeakPtr<GameObject> m_Owner;
	};
}


