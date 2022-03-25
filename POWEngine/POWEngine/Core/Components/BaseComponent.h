#pragma once
#include "POWEngine/Core/ECS/ECSTypes.h"

namespace powe
{
	class BaseComponent
	{
	public:

		BaseComponent() = default;
		BaseComponent(const BaseComponent&) = default;
		BaseComponent& operator=(const BaseComponent&) = default;
		BaseComponent(BaseComponent&&) noexcept = default;
		BaseComponent& operator=(BaseComponent&&) noexcept = default;
		virtual ~BaseComponent() = default;

	public:

		template<typename T>
		static size_t GetId();

		virtual void DestroyData(uint8_t* address) = 0;
		virtual void MoveData(RawByte* source, RawByte* destination) const = 0;
		[[nodiscard]] virtual SizeType GetSize() const = 0;

	private:

		// Thread safe counter
		static std::atomic_size_t m_ComponentIdCounter;

	};

	template <typename T>
	size_t BaseComponent::GetId()
	{
		static const size_t counter{ m_ComponentIdCounter++ };
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

		Component() = default;
		Component(const Component&) = default;
		Component& operator=(const Component&) = default;
		Component(Component&&) noexcept = default;
		Component& operator=(Component&&) noexcept = default;

	public:

		void DestroyData(uint8_t* address) override;
		void MoveData(RawByte* source, RawByte* destination) const override;
		[[nodiscard]] SizeType GetSize() const override;
		virtual ~Component() override = default;

	};

	template <typename T>
	void Component<T>::DestroyData(uint8_t* address)
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
}



