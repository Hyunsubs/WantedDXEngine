#include "Renderer.h"

namespace Blue
{
	Renderer::Renderer(uint32 width, uint32 height, HWND window)
	{
		uint32 flag = 0u;

#ifdef _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		//swapChainDesc.BufferCount = ;

		// 장치 생성
		D3D11CreateDeviceAndSwapChain(
			nullptr, 
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION,
			&swapChainDesc, &swapChain,
			&device, nullptr, &context
			);

		// context 옆에 d3dfeaturelevel은 선택된 버전을 알려주기 위해 주소를 받지만
		// 알 필요가 없으므로 nullptr을 넣음

	}


	Renderer::~Renderer()
	{
	}


	void Renderer::Draw()
	{
	}
}