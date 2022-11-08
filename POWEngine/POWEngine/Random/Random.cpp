#include "pch.h"
#include "Random.h"

float Random::RandFloat(float from, float to)
{
    std::random_device randDevice;
    std::mt19937 randEngine(randDevice());
    
    const std::uniform_real_distribution<float> dist{from,to};
    
    return dist(randEngine);
}
