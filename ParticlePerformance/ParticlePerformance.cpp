#include "ParticleTest.h"

int main()
{
    
    const SharedPtr<ParticleTest> particleApp{std::make_shared<ParticleTest>()};
    particleApp->Run();
    return 0;
} 