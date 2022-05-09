// POWTestEnv.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <POWEngine/LockFree/LFQueue.h>
#include <vld.h>
#include <random>
#include <future>
#include <POWEngine/Core/Thread/SimpleThreadPool.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

std::random_device g_RandDevice{};
std::mt19937 engine{ g_RandDevice() };

void Push(powe::LFQueue<std::string>& lf, const std::string& some)
{
	const std::uniform_int_distribution<int> uniIntDist{ 1,6 };
	std::this_thread::sleep_for(std::chrono::milliseconds(uniIntDist(engine)));
	lf.Push(some);
}

std::optional<std::string> Pop(powe::LFQueue<std::string>& lf)
{
	const std::uniform_int_distribution<int> uniIntDist{ 1,6 };
	std::this_thread::sleep_for(std::chrono::milliseconds(uniIntDist(engine)));
	return lf.PopReturn();
}

TEST_CASE("Lock-Free queue Push test")
{
	powe::LFQueue<std::string> lfTest{};

	SECTION("Push")
	{
		const int pushTime{ 200 };

		std::vector<std::future<void>> pushThread{};

		for (int i = 0; i < pushTime; ++i)
		{
			std::string inString{ "some" };
			inString.append(std::to_string(i));
			pushThread.emplace_back(std::async(std::launch::async, &Push, std::ref(lfTest), inString));
		}

		for (auto& lThread : pushThread)
		{
			lThread.get(); // thread join
		}

		for (int i = 0; i < pushTime; ++i)
		{
			auto val{ lfTest.PopReturn() };
			REQUIRE(val.has_value() == true);
			if (val.has_value())
				std::cout << val.value() << '\n';
		}

	}

}

TEST_CASE("Lock-Free queue Pop test")
{
	std::cout << "---------- Lock-Free queue pop test -------------\n";

	powe::LFQueue<std::string> lfTest{};

	const int popTime{ 10 };

	std::vector<std::future<std::optional<std::string>>> popThread{ };

	for (int i = 0; i < popTime; ++i)
	{
		std::string inString{ "some" };
		inString.append(std::to_string(i));
		lfTest.Push(inString);
	}

	for (int i = 0; i < popTime; ++i)
	{
		std::string inString{ "some" };
		inString.append(std::to_string(i));
		popThread.emplace_back(std::async(std::launch::async, &Pop, std::ref(lfTest)));
	}

	for (auto& thr : popThread)
	{
		auto val{ thr.get() };
		REQUIRE(val.has_value() == true);
		std::cout << val.value_or("") << '\n';
	}

	REQUIRE_THROWS(lfTest.Front());
}

#include <POWEngine/Logger/Logger.h>

void PumpMessage(powe::Logger& logger, const std::string& msg)
{
	const std::uniform_int_distribution<int> uniIntDist{ 1,6 };
	std::this_thread::sleep_for(std::chrono::milliseconds(uniIntDist(engine)));
	logger.AddMessage(msg);
}

#include <POWEngine/Core/IdGenerator/UniqueIdGenerator.h>

TEST_CASE("Logger")
{
	std::cout << "------- Test logger using thread pool -------\n";

	powe::Logger logger{};

	std::cout << powe::UniqueIdGenerator<powe::Logger>::GetNewId<double>() << std::endl;
	std::cout << powe::UniqueIdGenerator<powe::Logger>::GetNewId<float>() << std::endl;
	//std::cout << powe::UniqueIdGenerator<powe::>::GetNewId() << std::endl;

	//std::cout << sizeof(SharedPtr<powe::Logger>) << '\n';
	//std::cout << sizeof(SharedPtr<int>) << '\n';

	const int pumpCnt{ 20 };

	std::vector<std::future<void>> pumpMsgThread{ pumpCnt };

	for (int i = 0; i < pumpCnt; ++i)
	{
		std::string msg{ "Hello From logger thread: " };
		msg.append(std::to_string(i));
		pumpMsgThread[i] = std::async(std::launch::async, &PumpMessage, std::ref(logger), msg);
	}

	for (auto& thread : pumpMsgThread)
	{
		thread.get();
	}

}

#include <POWEngine/LockFree/LFStack.h>

void PushStack(powe::LFStack<std::string>& stack, const std::string& msg)
{
	const std::uniform_int_distribution<int> uniIntDist{ 1,6 };
	std::this_thread::sleep_for(std::chrono::milliseconds(uniIntDist(engine)));
	stack.Push(msg);
}

TEST_CASE("Lock free stack push")
{
	std::cout << "--------- Test lock free stack push ----------------\n";

	powe::LFStack<std::string> lfTest{};

	const int pushTime{ 100 };

	std::vector<std::future<void>> pushThread{ pushTime };

	lfTest.Push("some");
	auto test = lfTest.PopReturn();
	test = lfTest.PopReturn();
	test;
	for (int i = 0; i < pushTime; ++i)
	{
		std::string inString{ "some" };
		inString.append(std::to_string(i));
		pushThread[i] = std::async(std::launch::async, &PushStack, std::ref(lfTest), inString);
	}

	for (auto& thread : pushThread)
	{
		thread.get();
	}

	for (int i = 0; i < pushTime; ++i)
	{
		auto val{ lfTest.PopReturn() };
		REQUIRE(val.has_value() == true);
		if (val.has_value())
			std::cout << val.value() << '\n';
	}

}

struct Base
{

};

template<typename T>
struct Comp : Base
{

};

struct Some : Comp<Some>
{
};

TEST_CASE("For loop calculation or unordered set")
{
	std::cout << "------ For loop calculation or unordered set --------\n";

	//std::chrono::high_resolution_clock::time_point startTime{};


	std::cout << std::is_base_of_v<Base, Some> << std::endl;

	const uint32_t some{ 1 | (1u << 31u) };

	std::cout << (some & ~(1u << 31u)) << std::endl;
}

// TODO: Create Lock-free stack ABA test

#define ComponentTest

#ifdef ComponentTest

#define RUNTIME_TEST

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/BaseComponent.h"
//#include "POWEngine/Core/Components/Transform2D.h"

struct TestComponent : powe::Component<TestComponent>
{
	int some{ 42 };
};

struct TestComponent2 : powe::Component<TestComponent2>
{
	int sol{ 42 };
};

struct MoreVirtual : powe::Component<MoreVirtual>
{
	virtual void Update()
	{
		std::cout << "Update";
	}
};

TEST_CASE("Component")
{
	std::cout << "------ Component System Test --------\n";

	using namespace powe;

	SECTION("Test Adding Components")
	{
		SharedPtr<WorldEntity> worldEnt{ std::make_shared<WorldEntity>() };
		SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(*worldEnt) };

		REQUIRE(gameObject->GetID() == 0);

		TestComponent* test1{ worldEnt->AddComponentToGameObject(gameObject->GetID(), TestComponent{}) };
		TestComponent2* test2{ worldEnt->AddComponentToGameObject(gameObject->GetID(), TestComponent2{}) };

		REQUIRE(test1 != nullptr);
		REQUIRE(test1->some == 42);
		REQUIRE(test2 != nullptr);
		REQUIRE(test2->sol == 42);
	}

	SECTION("Test adding same components on different gameobjects")
	{
		SharedPtr<WorldEntity> worldEnt{ std::make_shared<WorldEntity>() };
		SharedPtr<GameObject> gameObject1{ std::make_shared<GameObject>(*worldEnt) };

		TestComponent* test1 = worldEnt->AddComponentToGameObject(gameObject1->GetID(), TestComponent{});

		REQUIRE(test1 != nullptr);
		REQUIRE(test1->some == 42);

		SharedPtr<GameObject> gameObject3{ std::make_shared<GameObject>(*worldEnt) };
		test1 = worldEnt->AddComponentToGameObject(gameObject3->GetID(), TestComponent{});

		REQUIRE(test1 != nullptr);
		REQUIRE(test1->some == 42);

		MoreVirtual* moreVirtual = worldEnt->AddComponentToGameObject(gameObject3->GetID(), MoreVirtual{});
		moreVirtual->Update();

	}

	SECTION("Runtime adding components")
	{
		SharedPtr<WorldEntity> worldEnt{ std::make_shared<WorldEntity>() };
		SharedPtr<GameObject> gameObject1{ std::make_shared<GameObject>(*worldEnt) };

		worldEnt->AddComponentToGameObject(gameObject1->GetID(), TestComponent{});
		worldEnt->AddComponentToGameObject(gameObject1->GetID(), TestComponent2{}, ComponentFlag::Sparse);

		worldEnt->InternalRemoveGameObjectFromPipeline();
		worldEnt->InternalRemoveComponentFromGameObject();
		worldEnt->InternalAddGameObjectToPipeline();

		TestComponent* test{ worldEnt->GetComponent<TestComponent>(gameObject1->GetID()) };
		TestComponent2* test2{ worldEnt->GetComponent<TestComponent2>(gameObject1->GetID()) };

		REQUIRE(test != nullptr);
		REQUIRE(test->some == 42);

		REQUIRE(test2 != nullptr);
		REQUIRE(test2->sol == 42);
	}

}

#endif
