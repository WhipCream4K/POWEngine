#pragma once

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
		virtual void MoveData(uint8_t* source, uint8_t* destination) const = 0;
		[[nodiscard]] virtual size_t GetSize() const = 0;

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
		void MoveData(uint8_t* source, uint8_t* destination) const override;
		[[nodiscard]] size_t GetSize() const override;
		virtual ~Component() override = default;

	};

	template <typename T>
	void Component<T>::DestroyData(uint8_t* address)
	{
		T* object{ reinterpret_cast<T*>(address) };
		object->~T();
	}

	template <typename T>
	void Component<T>::MoveData(uint8_t* source, uint8_t* destination) const
	{
		new (destination) T{ std::move(*reinterpret_cast<T*>(source)) };
	}

	template <typename T>
	size_t Component<T>::GetSize() const
	{
		return sizeof(T);
	}
}



