#pragma once

#include "Component.h"
#include "Math/Matrix4.h"
#include <d3d11.h>

namespace Blue
{
	// ī�޶� ���� (��� ����)
	struct CameraBuffer
	{
		Matrix4 viewMatrix;
	};

	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		~CameraComponent() = default;

		virtual void Draw() override;

	private:
		// ī�޶� �Ӽ�
		CameraBuffer data;

		// DX Buffer
		ID3D11Buffer* cameraBuffer = nullptr;
	};
}