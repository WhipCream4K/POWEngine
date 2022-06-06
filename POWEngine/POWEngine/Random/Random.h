#pragma once

#include <random>


class Random
{

public:

	template<typename T>
	static T RandInt(T from, T to);
};

template <typename T>
T Random::RandInt(T from, T to)
{
	std::random_device randDevice;
	std::mt19937 randEngine(randDevice());
	const std::uniform_int_distribution<int> dist{from,to};

	return dist(randEngine);
}

