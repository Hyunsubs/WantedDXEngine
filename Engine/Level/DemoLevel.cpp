#include "DemoLevel.h"

#include "Actor/Actor.h"
#include "Math/Transform.h"
#include "Render/QuadMesh.h"

#include "Resource/ShaderLoader.h"
#include "Resource/ModelLoader.h"
#include "Resource/TextureLoader.h"

#include "Shader/TextureMappingShader.h"
#include "Component/StaticMesh.h"
#include "Component/CameraComponent.h"
#include "Actor/QuadActor.h"

namespace Blue
{
	DemoLevel::DemoLevel()
	{
		// 데모 씬(레벨) 구성

		// 액터 생성
		std::shared_ptr<QuadActor> actor = std::make_shared<QuadActor>();
		//actor->transform.position.x = 0.5f;
		actor->transform.scale.x = 0.5f;

		// 카메라 액터 생성
		std::shared_ptr<Actor> cameraActor = std::make_shared<Actor>();
		cameraActor->transform.position.x = -0.5f;
		cameraActor->AddComponent(std::make_shared<CameraComponent>());


		// 액터를 레벨에 추가
		AddActor(actor);
		AddActor(cameraActor);
	}

	DemoLevel::~DemoLevel()
	{
	}
}