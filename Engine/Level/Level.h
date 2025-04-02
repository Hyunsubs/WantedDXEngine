#pragma once

#include <vector>
#include <memory>
#include "Core/Type.h"

namespace Blue
{
	class Actor;
	// 월드에 배치된 액터를 관리
	class Level
	{
	public:
		Level();
		virtual ~Level();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);

		void AddActor(std::shared_ptr<Actor> newActor);

		// Getter
		std::shared_ptr<Actor> GetActor(int index) const;
		const uint32 ActorCount() const;

		inline std::shared_ptr<Actor> GetCamera() const { return cameraActor; }

	protected:
		std::vector<std::shared_ptr<Actor>> actors;

		// 메인 카메라 액터
		std::shared_ptr<Actor> cameraActor;
	};
}