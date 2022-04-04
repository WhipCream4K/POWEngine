#pragma once
#include "BaseComponent.h"

namespace powe
{
	class GameObject;
	class Transform2D : public Component<Transform2D>
	{
	public:

		Transform2D() = default;

		void SetPosition(const glm::vec2& position);
		void SetScales(const glm::vec2& scales);
		void SetRotation(float zRotation);
		void SetDepth(float depth);

		const glm::vec2& GetPosition();
		const glm::vec2& GetScales();
		float GetDepth();
		float GetRotation();
		
	private:

		glm::vec2 m_WorldPosition{};
		glm::vec2 m_WorldScales{};
		glm::vec2 m_LocalPosition{};
		glm::vec2 m_LocalScales{};
		float m_WorldZRotation{};
		float m_LocalZRotation{};
		float m_Depth{};
	};
	
}


