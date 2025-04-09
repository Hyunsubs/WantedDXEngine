#include "Renderer.h"
#include <vector>
#include <d3dcompiler.h>

#include "../Math/Vector3.h"
#include "TriangleMesh.h"
#include "QuadMesh.h"
#include "Core/Common.h"

#include "Level/Level.h"
#include "Actor/Actor.h"

namespace Blue
{
	Renderer::Renderer(uint32 width, uint32 height, HWND window)
	{
		// 장치 생성에 사용하는 옵션.
		uint32 flag = 0u;

#if _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// 생성할 라이브러리 버전.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		// 출력할 피쳐레벨
		D3D_FEATURE_LEVEL outFeatureLevel;

		// 장치 생성
		ThrowIfFailed(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&device,
			&outFeatureLevel,
			&context
		), TEXT("Failed to create devices."));

		// 팩토리 리소르 생성
		IDXGIFactory* factory = nullptr;
		// CreateDXGIFactory(__uuidof(factory), reinterpret_cast<void**>(&factory));
		ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&factory)), TEXT("Failed To Create DXGI Factory."));

		// 스왑 체인 정보 구조체.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.Windowed = true;		// 창 모드?.
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;		// 백버퍼 개수.
		swapChainDesc.SampleDesc.Count = 1;	// 멀티 샘플링 개수.
		swapChainDesc.SampleDesc.Quality = 0; // 멀티 샘플링 수준.
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 144;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
		// 그리기 전에 매번 렌더타겟 뷰 바인딩 하도록 하는 옵션
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// SwapChain 생성
		ThrowIfFailed(factory->CreateSwapChain
		(
			device,
			&swapChainDesc,
			&swapChain
		), TEXT("Failed To Create a Swap Chain"));

		// 렌더 타겟 뷰 생성.
		ID3D11Texture2D* backbuffer = nullptr;
		ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)), TEXT("Failed To Get Back Buffer"));
		ThrowIfFailed(device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView), TEXT("Failedd To Create Render Target View"));

		// 사용한 리소스 해제
		backbuffer->Release();
		backbuffer = nullptr;

		// 뷰포트(화면).
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// 뷰포트 설정.
		context->RSSetViewports(1, &viewport);
	}

	Renderer::~Renderer()
	{
		if (renderTargetView)
		{
			renderTargetView->Release();
			renderTargetView = nullptr;
		}

		if (context)
		{
			context->Release();
			context = nullptr;
		}

		if (swapChain)
		{
			swapChain->Release();
			swapChain = nullptr;
		}

		if (device)
		{
			device->Release();
			device = nullptr;
		}
	}

	void Renderer::Draw(std::shared_ptr<class Level> level)
	{
		if (isResizing)
		{
			return;
		}

		// 그리기 전 작업 (BeginScene).
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);

		// 지우기(Clear).
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);

		// Draw
		// 카메라 바인딩
		if (level->GetCamera())
		{
			level->GetCamera()->Draw();
		}


		for (uint32 ix = 0; ix < level->ActorCount(); ix++)
		{
			// 액터 가져오기
			auto actor = level->GetActor(ix);

			// Draw
			if (actor->IsActive())
			{
				actor->Draw();
			}
		}

		// 버퍼 교환. (EndScene/Present).
		swapChain->Present(1u, 0u);
	}

	void Renderer::OnResize(uint32 width, uint32 height)
	{
		// 창 변경으로 인한 리소스 크기 조정
		if (!device || !context || !swapChain)
		{
			return;
		}
		isResizing = true;

		// context 비우기
		context->ClearState();
		context->Flush();

		// 렌더타겟 해제
		if (renderTargetView)
		{
			renderTargetView->Release();
			renderTargetView = nullptr;
		}

		// 스왑체인 백버퍼 크기 변경
		ThrowIfFailed(
			swapChain->ResizeBuffers(0, width, height,
				DXGI_FORMAT_R8G8B8A8_UNORM,0), TEXT("Failed to resize swapchain buffer.")
		);
		// 렌더타겟 재생성
		ID3D11Texture2D* backbuffer = nullptr;
		ThrowIfFailed(
			swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)),
			TEXT("Failed tp get buffer from swapchain")
		);

		ThrowIfFailed(device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView), 
			TEXT("Failed To Create Render Target View"));

		// 사용한 리소스 해제
		backbuffer->Release();
		backbuffer = nullptr;

		// 뷰포트 업데이트
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// 뷰포트 설정
		context->RSSetViewports(1, &viewport);

		isResizing = false;
	}
}