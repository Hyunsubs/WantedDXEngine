#pragma once

#include "Core/Type.h"
#include <string>
#include "Core/Engine.h"

namespace Blue
{
	// 텍스처 파일을 저장할 구조체
	struct TextureData
	{
		TextureData() = default;
		~TextureData()
		{
			free(data);
			data = nullptr;

			shaderResourceView->Release();
			shaderResourceView = nullptr;

			samplerState->Release();
			samplerState = nullptr;
		}

		// 텍스처 원시 데이터
		int32 width = 0;
		int32 height = 0;
		int32 channelCount = 0;
		void* data = nullptr;

		// DX리소스
		ID3D11ShaderResourceView* shaderResourceView = nullptr;
		ID3D11SamplerState* samplerState = nullptr;

	};

	// 텍스처 클래스
	class Texture
	{
		// 바인딩할 셰이더
		enum class BindType
		{
			VertexShader,
			PixelShader
		};

	public:
		Texture();
		Texture(const std::string& name, BindType bindType = BindType::PixelShader,
			uint32 index = 0u);
		~Texture();

		void Bind();

	private:
		void LoadTexture(const std::string& name);

	private:
		// 이미지 이름
		std::string name;

		// 텍스처 순번
		uint32 index = 0u;

		// 바인딩 셰이더 타입
		BindType bindType = BindType::PixelShader;

		// 텍스처 데이터
		std::unique_ptr<TextureData> textureData;
	};

}