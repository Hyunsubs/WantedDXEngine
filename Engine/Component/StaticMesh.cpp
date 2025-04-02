#include "StaticMesh.h"
#include "Render/Mesh.h"
#include "Shader/Shader.h"
#include "Core/Engine.h"

namespace Blue
{
	StaticMeshComponent::StaticMeshComponent()
	{
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
	}

	void StaticMeshComponent::Draw()
	{
		uint32 meshCount = mesh->SubMeshCount();

		// ����ó��
		if (meshCount != (uint32)shaders.size())
		{
			return;
		}

		for (int ix = 0; ix < meshCount; ix++)
		{
			// ���� �޽� ��������
			auto subMesh = mesh->GetSubMesh(ix);

			// �޽ð� ��ȿ�ϸ� Draw
			if (subMesh.lock() && shaders[ix].lock())
			{
				// ���� �޽� ���ε�
				subMesh.lock()->Bind();

				// ���̴� ���ε�
				shaders[ix].lock()->Bind();

				// DrawCall
				static ID3D11DeviceContext& context = Engine::Get().Context();
				context.DrawIndexed(subMesh.lock()->IndexCount(), 0u, 0u);
			}
		}
	}

	void StaticMeshComponent::SetMesh(std::shared_ptr<class Mesh> newMesh)
	{
		mesh = newMesh;
	}

	void StaticMeshComponent::AddShader(std::weak_ptr<class Shader> newShader)
	{
		shaders.emplace_back(newShader);
	}
}