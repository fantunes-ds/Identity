#include <stdafx.h>
#include <TestDependencies.h>


void Test::TestIrrKlang()
{
    irrklang::ISoundEngine* se = irrklang::createIrrKlangDevice();
    se->play2D("../Engine/Resources/honteux.mp3");
}

void Test::TestGPM()
{
    GPM::Matrix3F m;
    std::cout << m.identity;
}

void Test::TestAssimp()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("../Engine/Resources/Box.fbx", 
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
		std::cout << "Assimp error loading file: " << importer.GetErrorString() << std::endl;
	else
		std::cout << "Assimp successfully loaded file " << "\n";
	
}