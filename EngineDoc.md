After, my last take on the game engine I wasn’t really thinking of programming patterns at all and focus on something else that doesn’t fit the whole topic. 
    
I finally have a chance to rethink and do the thing I was interested and suppose to do in the first time.


# Lite ECS
This is my take on ECS (Entity Component System). I’ll try to explain what I’ve done for this game engine

## **GameObject (Entity)**
Speaking of ECS GameObject doesn’t really hold any state or variables. It only consists of a reference to world and unique Id to tell the world to point to the correct GameObject in the system although I put some functionality for ease of use and **of course it’s final.**

```
	class GameObject final
	{
	public:

		explicit GameObject(WorldEntity& world);
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) noexcept = delete;
		GameObject& operator=(GameObject&&) noexcept = delete;
		~GameObject() = default;

	public:

		GameObjectID GetID() const { return m_Id; }
		WorldEntity& GetWorld() const { return m_World; }

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponent(
			ComponentType&& component,
			ComponentFlag flag = ComponentFlag::Default);

		template<typename ComponentType>
		void RemoveComponent() const;

	private:

		WorldEntity& m_World; // GameObject cannot exist without world entity
		GameObjectID m_Id;
	};

 ```

<!-- ![GameObjectClass](https://i.imgur.com/Jav9NDR.png) -->

## **Component**
Component usually holds data for one aspect of a GameObject. This data will then be used in the “System” or the logic. It has a unique Id type for every created Component in the world and needed to register to the world to use some move or destruct operations.

Every component that the user has created must inherited from ```class Component<typename T>``` to be able to register in the world and


```	
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
		static const ComponentTypeID counter{ m_ComponentIdCounter++ };
		return counter;
	}
 ```



**Note**: In POW, BaseComponent uses an atomic id counter for thread-safety purposes.



```
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
```
Now there are **virtual functions for destruction and creation**. This is useful when your component has to initialize some values or call any other services before making itself active. Keep in mind that these two mentions only get call once in its life time.

<!-- ![Screenshot 2021-08-16 163249](https://i.imgur.com/5ScqX39.png)
![Screenshot 2021-08-16 163304](https://i.imgur.com/QVcnWSX.png) -->

## **Archetype**

So we got a GameObject and a component now how do we access and give it functionality for it. Enters ```Archetype``` I got interested in this idea when I stumble upon [Unity ECS Concepts](https://docs.unity3d.com/Packages/com.unity.entities@0.2/manual/ecs_core.html) and [Fast LightWeighted Entity Component System (flecs)](https://github.com/SanderMertens/flecs) a structure that groups all sorts of component types in one "Archetype" with a key generate from unique Id of components.

![Ec4KB](https://i.stack.imgur.com/Ec4KB.png)

From this diagram if two entities have the same type of components both entities will share the same Archetype and only one Archetype will be generated.

```
	// POD for component data
	struct Archetype final
	{
		Archetype();
		Archetype(const Archetype&) = default;
		Archetype& operator=(const Archetype&) = default;
		Archetype(Archetype&&) noexcept = default;
		Archetype& operator=(Archetype&&) noexcept = default;
		~Archetype() = default;

		static SharedPtr<Archetype> Create(const WorldEntity& world,const std::vector<ComponentTypeID>& types);

		SharedPtr<RawByte[]> CopyComponentData(const Archetype& other, const WorldEntity& world) const;
		void BuryBlock(WorldEntity& world,int index) const;
		void CleanUp(const WorldEntity& world) const;
		void AllocateComponentData(SizeType newSize, const WorldEntity& world);

		std::vector<ComponentTypeID> Types; // types of components of this archetypes
		std::vector<GameObjectID> GameObjectIds; // GameObjectIds that has this archetype
		ECSComponentMap<SizeType> ComponentOffsets;
		SharedPtr<RawByte[]> ComponentData{}; // Array of component struct
		SizeType SizeOfComponentsBlock{};
		SizeType TotalAllocatedData{};
	};
```

**NOTE:** Archetype now has member functions to facilitates manual memory handling.
Before, the archetype has to iterate over its types to get the correct offset of the component data but now it has ```ComponentOffsets``` to lookup the offset of desire component in a constant time.


<!-- ![Screenshot 2021-08-16 171533](https://i.imgur.com/W3nzoag.png) -->

As you can see that the "types" of Archetype is just a vector of ComponentTypeId so this can be easily checked against ```System``` or anything for that matter.

### SoA structure

my Archetype owns ```componentData``` which is and array of ```std::byte``` or ```byte``` to store an actual data existing components. Now, for most ECS this ```componentData``` will be structured as vector of **Array of Struct** so it's easily accessd and keep track of but I decided to go with **Structure of Array** instead. From this point on I will go by my assumptions because I didn't profile both of them to compare to really checked but I think this is the most suitable structure

Why? 
- During System updates. The ```System``` will go through each archetype and then ```componentData``` if the update logic uses data directly from the memory "I most likely get the benefits from **Data Locality** and didn't waste the wait time to throw the whole **Cache line** and catch a new one for next component, basically every component laid out after one another like the exercise1 that we did."

```SizeOfStruct``` and ```TotalLength``` is used in rumtime operations where you need to know the size of Archetype and the whole structure of components in this Archetype.

![Note 16 Aug BE 2564 (1)](https://i.imgur.com/sjRzrJx.jpg)

<!-- The Bad
- It is more challenge though to keep track of the memory and to picture how it's occupied the memory and move stuff around.
- It is slower when randomly access because it has to iterate through every type first if it's matched then return the pointer to data with offset -->

.

## **Problem with SOA**
Using this type of memory ordering is sometimes not so good. If you have a lot of systems that don't use or rarely used all the types of a given entity then you will end up with a lot of cache misses because of how it structued. 

So, in a lot of scenarios using ```Array of structrues``` will sometimes yield a better result. However, why not use the good in both worlds? If you knew for a fact that some of the components might not get used a lot during run-time then why not put it aside somewhere else and let the others go.



## Introducing Sparse Component

![Imgur](https://imgur.com/pwDM4OE.png)

Sparse set is a way to store component data in a contigous array and using a handle to map the location (offset) of the data [More on sparse set](https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html)

```
	enum class ComponentFlag : ComponentTypeID
	{
		Default,
		Sparse = (1u << 31u),

		Count = Sparse
	};
```

```
const ComponentTypeID componentId{ BaseComponent::GetId<ComponentType>() | ComponentTypeI(componentFlag) }; 
```
Now, every component will have a tag that indicates whether the component is a default SOA component or Sparse Component.

With that we can specialized how to put these component into where although it reduces the maximum number of generated component to 16bit (65,536), for now it's not a problem.

How does it works? 

When you add a component it will determine the flag of the component then depending on the flag the component will register differently.

![Note 16 Aug BE 2564 (1)](https://imgur.com/PhGvbIX.png)

Every component that registered in sparse set will not be in the current component data array thus getting rid of the in-active component completely.

Thanks to [Andrew Kelley](https://media.handmade-seattle.com/practical-data-oriented-design/) on his talk on [A Practical Guide to Applying Data-Oriented Design](https://media.handmade-seattle.com/practical-data-oriented-design/) and [David Colson](https://www.david-colson.com/about.html) on his [How to make a simple entity-component-system in C++](https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html) that give me insight on the idea of how to optimize cache misses and the idea of using sparse set.

Using this new method, I conduct a little benchmark that compares between the old ECS that I have and the new one. The result although is not that pleasing but it indicates that this structure does help when you have a system that doesn't use all the component that the entity has.

![Benchmark](https://imgur.com/bJRmPhk.png)

**NOTE**: 

The blue line indicates continous SOA structure.

The orange line indicates SOA and Sparse structure.

 **Bonus Comparing with Minigin**

 ![Benchmark2](https://imgur.com/xNUmDVZ.png)


## **System**

System, the logic of the world, The system will iterate all of the archetype given by ```WorldEntity``` and process the matched ```Archetype``` . The system consists of the layer that this system will activate, system key this key will be used to check against ```Archetpye``` type that basically if the key doesn't match the range of component, it will not get called, lastly a reference to the world.

```
	class SystemBase
	{
		friend class WorldEntity;

	public:

		SystemBase();
		SystemBase(const SystemBase&) = delete;
		SystemBase& operator=(const SystemBase&) = delete;
		SystemBase(SystemBase&&) noexcept = delete;
		SystemBase& operator=(SystemBase&&) noexcept = delete;
		virtual ~SystemBase() = default;

	public:

		[[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }

	protected:

		WorldEntity* GetWorld() const { return m_World; }

		void InternalUpdate(const Archetype&, float);
		void InternalCreate(const Archetype&);
		void InternalDestroy(const Archetype&);

		virtual void OnUpdate(float, powe::GameObjectID) = 0;
		virtual void OnCreate(GameObjectID) {}
		virtual void OnDestroy(GameObjectID) {}

		template<typename ...Args>
		void InternMakeKeys();

		// Specialize GetComponent from iteration
		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent() const;

		// Specialize GetComponent from iteration
		template<typename ...Args>
		std::tuple<std::add_pointer_t<Args>...> GetComponentsView() const;

	private:

		void SetWorld(WorldEntity* world);

		template<typename T>
		T* GetComponent(const Archetype& archetype) const;

		WorldEntity* m_World; // using pointer to be more flexible. Doesn't really need smartpointer here there's no ownership changing anyway
		std::unordered_set<powe::ComponentTypeID> m_Keys;
		const Archetype* m_CurrentArchetype;
		uint32_t m_UpdateCountPerArchetype;

	};

```
<!-- ![Screenshot 2021-08-16 174352](https://i.imgur.com/aWLxfkJ.png) -->

# Runtime

## **Game Loop**

I keep the same game loop where the user has to specify their own loop and then they need to manually tell the engine that it has to step somewhere kind of like a Physics system

User
```
	while (!engineCore->TranslateWindowInputs(*window,*worldEntity))
	{
		engineCore->Step(*worldEntity);

		engineCore->Draw(*window, *worldEntity);
	}
```

Engne Query user input and start the clock

```
bool powe::Core::TranslateWindowInputs(const Window& window, WorldEntity& worldEntt) const
{
	m_WorldClock->Start();

	bool isEarlyExit{};
	bool ignoreInputs{};

	HardwareMessages hwMessages{};
	window.PollHardwareMessages(hwMessages, isEarlyExit, ignoreInputs);

	if (!ignoreInputs)
	{
		const float deltaTime{ m_WorldClock->GetDeltaTime() };
		m_InputManager->PollHardWareMessages(hwMessages, deltaTime);
		worldEntt.GetInputSettings().ParseHWMessages(hwMessages);
	}

	return isEarlyExit;
}

```

Step part

```
void powe::Core::Step(WorldEntity& worldEntity) const
{
	worldEntity.ResolveEntities();

	const float deltaTime{ m_WorldClock->GetDeltaTime() };

	worldEntity.UpdatePipeline(PipelineLayer::InputValidation, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::Update, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::PhysicsValidation, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::PostUpdate, deltaTime);

}

```