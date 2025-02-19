#pragma once

#include "Shader.h"
#include "Render/Texture.h"
#include <memory>

namespace Blue
{
	class TextureMappingShader : public Shader
	{
	public:
		TextureMappingShader();
		TextureMappingShader(const std::string& textureName = "");

		virtual void Bind() override;

	private:
		// �ؽ�ó ���ҽ�
		std::unique_ptr<Texture> texture;
	};
}


