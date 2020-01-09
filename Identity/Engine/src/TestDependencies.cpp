#include <TestDependencies.h>

#include <GPM/GPM.h>
#include <irrKlang.h>

void Test::TestIrrKlang()
{
    irrklang::ISoundEngine* se = irrklang::createIrrKlangDevice();
    se->play2D("../Dependencies/honteux_ACACNOH.mp3", true);
}

void Test::TestGPM()
{
    GPM::Matrix3F m;
    std::cout << m.identity;
}
