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
#include "Core/InputController.h"

namespace Blue
{
	DemoLevel::DemoLevel()
	{
		// 데모 씬(레벨) 구성

		// 액터 생성
		actor = std::make_shared<QuadActor>();
		actor->transform.scale.x = 0.5f;
		actor->transform.scale.y = 0.5f;



		// 카메라 액터 생성
		std::shared_ptr<Actor> cameraActor = std::make_shared<Actor>();
		cameraActor->transform.position.z = 0.f;
		cameraActor->AddComponent(std::make_shared<CameraComponent>());


		// 액터를 레벨에 추가
		AddActor(actor);
		AddActor(cameraActor);
	}

	DemoLevel::~DemoLevel()
	{
	}

	void DemoLevel::Tick(float deltaTime)
	{
		Level::Tick(deltaTime);
		if (InputController::Get().IsKey('X'))
		{
			actor->transform.rotation.y += 20.f * deltaTime;
		}
	}
}