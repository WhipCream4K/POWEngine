// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vld.h>

//#include "../POWEngine/POWEngine/Window/Window.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Core.h"
#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/Core/LockFree/LFQueue.h"


struct SomeClass
{
	bool PrintThis(const std::string& title)
	{
		std::cout << title << '\n';
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return true;
	}
};

//template<typename Func,typename ...Args>
//std::invoke_result_t<Func,Args...> PrintT(Func&& fn,Args&&... args)
//{
//	return fn(std::forward<Args...>(args)...);
//}

template<typename Func,typename UserClass = typename FnTraits<Func>::class_type,typename ...Args>
std::invoke_result_t<Func,UserClass,Args...> PrintT(Func&& fn,UserClass& obj,Args&&... args)
{
	return (&obj->*fn)(std::forward<Args>(args)...);
}

bool Print(const std::string& title)
{
	std::cout << title << '\n';
	return true;
}

int main()
{
	powe::Core engineCore{};
	powe::Window window{ 640,480,"NewTitle" };
	//SharedPtr<powe::Window> window{ powe::Window{640,480,"NewWindow"} };
	powe::SimpleThreadPool threadPool{};

	//std::string lmao{ "lmao" };
	SharedPtr<SomeClass> obj{std::make_shared<SomeClass>()};

	powe::LFQueue<std::string> lfTest{};

	lfTest.Push("Some");
	lfTest.Push("Some2");
	
	//std::cout << lfTest.Front() << '\n';

	std::cout << sizeof(std::promise<bool>) << std::endl;
	std::cout << sizeof(std::packaged_task<void()>) << std::endl;

	while (!engineCore.TranslateWindowInputs(window))
	{

	}

	//some.get();
	//std::cout << some.get() << std::endl;

}