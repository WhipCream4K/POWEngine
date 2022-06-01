#pragma once
#include "POWEngine/ECS/ECSTypes.h"

namespace powe
{
	class WorldEntity;
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

		//template<typename T>
		//static SharedPtr<T> Create();
			
		virtual void DestroyData(RawByte* address) = 0;
		virtual void MoveData(RawByte* source, RawByte* destination) const = 0;
		virtual void InternalCreate(RawByte* , WorldEntity&, GameObjectID) {}  
		virtual void InternalDestroy(RawByte* , WorldEntity&, GameObjectID) {}

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
		//static const ComponentTypeID counter{ m_ComponentIdCounter++ };
		static const ComponentTypeID counter{ m_ComponentIdCounter.load() };
		++m_ComponentIdCounter;
		return counter;
	}

	//template <typename T>
	//SharedPtr<T> BaseComponent::Create()
	//{
	//	return SharedPtr<T>(new T());
	//}

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
		void MoveData(RawByte* source, RawByte* destination) const override;
		void InternalCreate(RawByte* source, WorldEntity&, GameObjectID) final;
		void InternalDestroy(RawByte* source, WorldEntity&, GameObjectID) final;
		[[nodiscard]] SizeType GetSize() const override;

	protected:

		virtual void OnDestroy(WorldEntity&, GameObjectID) {}
		virtual void OnCreate(WorldEntity&,GameObjectID) {}

	protected:

		Component() = default;

	};

	template <typename T>
	void Component<T>::DestroyData(RawByte* address)
	{
		reinterpret_cast<T*>(address)->~T();
	}

	template <typename T>
	void Component<T>::MoveData(RawByte* source, RawByte* destination) const
	{
		new (destination) T{ std::move(*reinterpret_cast<T*>(source)) };
		reinterpret_cast<T*>(source)->~T(); // Invalidate the old memory
	}

	template <typename T>
	void Component<T>::InternalCreate(RawByte* source, WorldEntity& worldEntity, GameObjectID gameObjectId)
	{
		reinterpret_cast<T*>(source)->OnCreate(worldEntity, gameObjectId);
	}

	template <typename T>
	void Component<T>::InternalDestroy(RawByte* source, WorldEntity& worldEntity, GameObjectID gameObjectId)
	{
		reinterpret_cast<T*>(source)->OnDestroy(worldEntity, gameObjectId);
	}

	template <typename T>
	SizeType Component<T>::GetSize() const
	{
		return SizeType(sizeof(T));
	}

	class SparseComponent : public BaseComponent
	{
		friend class WorldEntity;

	public:

		SparseComponent(const SparseComponent&) = default;
		SparseComponent& operator=(const SparseComponent&) = default;
		SparseComponent(SparseComponent&&) = default;
		SparseComponent& operator=(SparseComponent&&) = default;
		~SparseComponent() override = default;

		static SharedPtr<SparseComponent> GetStatic();

	public:

		void DestroyData(RawByte*) override {}

		void MoveData(RawByte* , RawByte*) const override
		{
			//new (destination) SparseComponent{std::move(* reinterpret_cast<SparseComponent*>(source))};
			//new (destination) SparseHandle(*reinterpret_cast<SparseHandle*>(source));
		}

		SizeType GetSize() const override { return 0; }

	protected:

		explicit SparseComponent() = default;
	};


}



