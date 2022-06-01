#pragma once
#include "BaseComponent.h"
#include "POWEngine/Math/Math.h"

namespace powe
{
	class GameObject;

	enum class DirtyFlag : uint8_t
	{
		Position	= 1,
		Rotation	= (1 << 1),
		Scale		= (1 << 2)
	};

	class Transform2D : public Component<Transform2D>
	{
	public:

		Transform2D();
		explicit Transform2D(const SharedPtr<GameObject>& owner);
		Transform2D(const Transform2D&) = default;
		Transform2D& operator=(const Transform2D&) = default;
		Transform2D(Transform2D&&) noexcept = default;
		Transform2D& operator=(Transform2D&&) noexcept = default;
		~Transform2D() override = default;

		void OnDestroy(WorldEntity&, GameObjectID) override;

		void SetWorldPosition(const glm::vec2& position);
		void SetWorldScale(const glm::vec2& scales);
		void SetWorldRotation(float zRotation);

		void SetLocalPosition(const glm::vec2& position);
		void SetLocalScale(const glm::vec2& scales);
		void SetLocalRotation(float zRotation);

		const glm::vec2& GetWorldPosition();
		const glm::vec2& GetWorldScale();
		float GetWorldRotation();

		float GetLocalRotation() const;
		const glm::vec2& GetLocalPosition() const;
		const glm::vec2& GetLocalScale() const;

		void SetParent(const SharedPtr<GameObject>& gameObject,bool keepWorldPosition = true);
		void UpdateData(DirtyFlag flag);
		
	private:

		void SetDirtyFlag(DirtyFlag flag);
		void SetChildrenDirtyFlag(DirtyFlag flag);
		void AddChild(const SharedPtr<GameObject>& gameObject);
		void RemoveChild(const SharedPtr<GameObject>& gameObject);
		bool IsDirty(DirtyFlag flag) const;

		WeakPtr<GameObject> m_ParentNode{};
		//GameObject* m_ParentNode{};
		std::vector<WeakPtr<GameObject>> m_ChildrenNode{};
		//std::vector<GameObject*> m_ChildrenNode;
		//std::vector<GameObjectID> m_ChildrenNode;
		//SharedPtr<std::vector<GameObject*>> m_ChildrenNode;
		WeakPtr<GameObject> m_Owner{};
		//GameObject* m_Owner{};

		glm::vec2 m_WorldPosition{};
		glm::vec2 m_WorldScales{};
		glm::vec2 m_LocalPosition{};
		glm::vec2 m_LocalScales{};
		float m_WorldZRotation{};
		float m_LocalZRotation{};
		uint8_t m_DirtyFlag{};
	};
	
}


