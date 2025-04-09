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
		// ���� ��(����) ����

		// ���� ����
		actor = std::make_shared<QuadActor>();
		actor->transform.scale.x = 0.5f;
		actor->transform.scale.y = 0.5f;



		// ī�޶� ���� ����
		std::shared_ptr<Actor> cameraActor = std::make_shared<Actor>();
		cameraActor->transform.position.z = 0.f;
		cameraActor->AddComponent(std::make_shared<CameraComponent>());


		// ���͸� ������ �߰�
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