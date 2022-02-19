// POWTestEnv.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <POWEngine/Core/LockFree/LFQueue.h>
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

TEST_CASE("Logger")
{
	std::cout << "------- Test logger using thread pool -------\n";

	//powe::SimpleThreadPool threadPool{};
	//SharedPtr<powe::SimpleThreadPool> threadPool{std::make_shared<powe::SimpleThreadPool>()};
	powe::Logger logger{};

	std::cout << sizeof(SharedPtr<powe::Logger>) << '\n';
	std::cout << sizeof(SharedPtr<int>) << '\n';

	const int pumpCnt{ 20 };

	std::vector<std::future<void>> pumpMsgThread{ pumpCnt };

	for (int i = 0; i < pumpCnt; ++i)
	{
		std::string msg{ "Hello From logger thread: " };
		msg.append(std::to_string(i));
		pumpMsgThread[i] = std::async(std::launch::async, &PumpMessage, std::ref(logger), msg);
	}

	for (auto & thread : pumpMsgThread)
	{
		thread.get();
	}

	//logger.AddMessage("Hello from logger");
}

//int main()
//{
//	powe::LFQueue<std::string> lfTest{};
//
//	const int pushTime{ 200 };
//
//	std::vector<std::future<void>> pushThread{};
//
//	for (int i = 0; i < pushTime; ++i)
//	{
//		std::string inString{"some"};
//		inString.append(std::to_string(i));
//		pushThread.emplace_back(std::async(std::launch::async, &Push, std::ref(lfTest), inString));
//	}
//
//	for (auto& lthread : pushThread)
//	{
//		lthread.get();
//	}
//
//	for (int i = 0; i < pushTime; ++i)
//	{
//		auto val{ lfTest.PopReturn() };
//		if (val.has_value())
//			std::cout << val.value() << '\n';
//	}
//}