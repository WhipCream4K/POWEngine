// POWTestEnv.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <POWEngine/Core/LockFree/LFQueue.h>
#include <vld.h>
#include <random>
#include <future>

void Push(powe::LFQueue<std::string>& lf, const std::string& some)
{
	lf.Push(some);
}

int main()
{
	powe::LFQueue<std::string> lfTest{};

	const int pushTime{ 200 };

	std::vector<std::future<void>> pushThread{};

	for (int i = 0; i < pushTime; ++i)
	{
		std::string inString{"some"};
		inString.append(std::to_string(i));
		pushThread.emplace_back(std::async(std::launch::async, &Push, std::ref(lfTest), inString));
	}

	for (auto& lthread : pushThread)
	{
		lthread.get();
	}

	for (int i = 0; i < pushTime; ++i)
	{
		auto val{ lfTest.PopReturn() };
		if (val.has_value())
			std::cout << val.value() << '\n';
	}
}