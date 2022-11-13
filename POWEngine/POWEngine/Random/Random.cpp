#include "pch.h"
#include "Random.h"

std::random_device Random::m_RandDevice{};
std::mt19937 Random::m_RandEngine{m_RandDevice()};

float Random::RandFloat(float from, float to)
{
    // std::random_device randDevice;
    // std::mt19937 randEngine(randDevice());
    
    const std::uniform_real_distribution<float> dist{from,to};
    return dist(m_RandEngine);
}

float ThreadSafeRandom::RandFloat(float from, float to)
{
    std::random_device randDevice;
    std::mt19937 randEngine(randDevice());
    
    const std::uniform_real_distribution<float> dist{from,to};
    return dist(randEngine);
}
