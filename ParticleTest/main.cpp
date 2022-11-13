
#define RELEASE_WITH_LOGINFO
#include <iostream>
#include "ParticleApp.h"


int main()
{
    SharedPtr<ParticleApp> particleApp{ std::make_shared<ParticleApp>() };
    
    particleApp->Run();
    return 0;
}

