#include <stdafx.h>
#include <TestDependencies.h>


void Test::TestIrrKlang()
{
    irrklang::ISoundEngine* se = irrklang::createIrrKlangDevice();
    se->play2D("../Dependencies/honteux_ACACNOH.mp3");
}

void Test::TestGPM()
{
    GPM::Matrix3F m;
    std::cout << m.identity;
}
