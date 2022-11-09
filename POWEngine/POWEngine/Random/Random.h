#pragma once

#include <random>


class Random
{

public:

	Random();
	
	template<typename T>
	static T RandInt(T from, T to);

	static float RandFloat(float from,float to);

private:
	
	static std::random_device m_RandDevice;
	static std::mt19937 m_RandEngine;
};

template <typename T>
T Random::RandInt(T from, T to)
{
	std::random_device randDevice;
	std::mt19937 randEngine(randDevice());
	const std::uniform_int_distribution<int> dist{from,to};

	return dist(randEngine);
}

