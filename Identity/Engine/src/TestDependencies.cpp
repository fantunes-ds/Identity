#include <stdafx.h>
#include <TestDependencies.h>
#include <3DLoader/ObjectLoader.h>

void Test::TestIrrKlang()
{
    irrklang::ISoundEngine* se = irrklang::createIrrKlangDevice();
    se->play2D("../Engine/Resources/honteux.mp3");
}

void Test::TestGPM()
{
    GPM::Matrix3F m;
	GPM::Vector2D e(1, 5);
	GPM::Vector2D a = e;
	
    std::cout << m.identity;
}

void Test::TestAssimp()
{	
	Engine::ObjectLoader* loader = Engine::ObjectLoader::GetInstance();
	std::shared_ptr<Engine::ObjectElements::Model> model = Engine::ObjectLoader::LoadModel("../Engine/Resources/Box.fbx");

	if (model == nullptr)
		TestIrrKlang();

}