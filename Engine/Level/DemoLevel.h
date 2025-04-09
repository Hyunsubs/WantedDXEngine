#pragma once


#include "Level.h"


namespace Blue
{
	// 데모 레벨
	class DemoLevel : public Level
	{
	public:
		DemoLevel();
		~DemoLevel();

		virtual void Tick(float deltaTime) override;

	private:
		std::shared_ptr<class QuadActor> actor;
	};
}