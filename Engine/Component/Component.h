#pragma once


namespace Blue
{
	class Component
	{
	public:
		Component();
		virtual ~Component();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		inline class Actor* GetOwner() const { return owner; }
		inline void SetOwner(class Actor* newOwner) { owner = newOwner; }


	protected:
		class Actor* owner = nullptr;
	};

	
}