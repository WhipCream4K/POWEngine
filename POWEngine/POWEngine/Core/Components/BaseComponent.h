#pragma once
#include "POWEngine/Core/ECS/ECSTypes.h"

namespace powe
{
	class BaseComponent
	{
	public:

		friend class WorldEntity;

		BaseComponent(const BaseComponent&) = default;
		BaseComponent& operator=(const BaseComponent&) = default;
		BaseComponent(BaseComponent&&) noexcept = default;
		BaseComponent& operator=(BaseComponent&&) noexcept = default;
		virtual ~BaseComponent() = default;

	public:

		template<typename T>
		static ComponentTypeID GetId();

		virtual void DestroyData(RawByte* address) = 0;
		virtual void MoveData(RawByte* source, RawByte* destination) const = 0;
		[[nodiscard]] virtual SizeType GetSize() const = 0;

	protected:

		explicit BaseComponent() = default;

	private:

		// Thread safe counter
		static std::atomic<ComponentTypeID> m_ComponentIdCounter;
	};

	template <typename T>
	ComponentTypeID BaseComponent::GetId()
	{
		static const ComponentTypeID counter{ m_ComponentIdCounter++ };
		return counter;
	}

	/**
	 * \brief Every component should inherit from this and make sure that the component is moveable
	 * \tparam T Struct of components, prefer POD over object class
	 */
	template<typename T>
	class Component : public BaseComponent
	{

	public:

		Component(const Component&) = default;
		Component& operator=(const Component&) = default;
		Component(Component&&) noexcept = default;
		Component& operator=(Component&&) noexcept = default;
		~Component() override = default;

	public:

		void DestroyData(RawByte* address) override;

		virtual void OnDestroy() {}
		virtual void OnCreate() {}
		
		void MoveData(RawByte* source, RawByte* destination) const override;
		[[nodiscard]] SizeType GetSize() const override;

	protected:

		explicit Component() = default;
	};

	template <typename T>
	void Component<T>::DestroyData(RawByte* address)
	{
		T* object{ reinterpret_cast<T*>(address) };
		object->~T();
	}

	template <typename T>
	void Component<T>::MoveData(RawByte* source, RawByte* destination) const
	{
		new (destination) T{ std::move(*reinterpret_cast<T*>(source)) };
	}

	template <typename T>
	SizeType Component<T>::GetSize() const
	{
		return SizeType(sizeof(T));
	}

	class SparseComponent : public BaseComponent
	{
	public:

		SparseComponent(const SparseComponent&) = default;
		SparseComponent& operator=(const SparseComponent&) = default;
		SparseComponent(SparseComponent&&) = default;
		SparseComponent& operator=(SparseComponent&&) = default;
		~SparseComponent() override = default;

	public:

		void DestroyData(RawByte* address) override {}
		void MoveData(RawByte* source, RawByte* destination) const override
		{
			new (destination) SizeType{*reinterpret_cast<SizeType*>(source)};
		}

		SizeType GetSize() const override { return sizeof(SizeType); }

	protected:
		
		explicit SparseComponent() = default;
	};

	namespace StaticComponent
	{
		static SharedPtr<SparseComponent> SparseTrait = std::make_shared<SparseComponent>();
	}

}



