#pragma once

#include <string>
#include "Math/Transform.h" // 설계 시 고려할 사항
#include <vector>
#include <memory>


namespace Blue
{
	class Actor
	{
		friend class Renderer;
		friend class Level;
	public:
		Actor();
		virtual ~Actor();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		void Destroy();

		// 컴포넌트 추가 함수
		void AddComponent(std::shared_ptr<class Component> newComponent);
		const bool IsActive() const;
		const bool HasInitialized() const;


	public:
		// 액터의 TRS 정보를 관리하는 트랜스폼()
		Transform transform;

	protected:
		// 액터의 이름 (일반적으로는 Hash로 변환해서 사용)
		std::wstring name = TEXT("Actor");

		// 액터의 초기화 여부
		bool hasInitialzied = false;

		// 액터의 활성화 여부
		bool isActive = true;

		// 삭제 여부
		bool hasDestroyed = false;

		// 컴포넌트 배열
		std::vector<std::shared_ptr<class Component>> components;
	};
}

