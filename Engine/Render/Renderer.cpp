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
		// ��ġ ������ ����ϴ� �ɼ�.
		uint32 flag = 0u;

#if _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// ������ ���̺귯�� ����.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		// ����� ���ķ���
		D3D_FEATURE_LEVEL outFeatureLevel;

		// ��ġ ����
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

		// ���丮 ���Ҹ� ����
		IDXGIFactory* factory = nullptr;
		// CreateDXGIFactory(__uuidof(factory), reinterpret_cast<void**>(&factory));
		ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&factory)), TEXT("Failed To Create DXGI Factory."));

		// ���� ü�� ���� ����ü.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.Windowed = true;		// â ���?.
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;		// ����� ����.
		swapChainDesc.SampleDesc.Count = 1;	// ��Ƽ ���ø� ����.
		swapChainDesc.SampleDesc.Quality = 0; // ��Ƽ ���ø� ����.
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 144;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
		// �׸��� ���� �Ź� ����Ÿ�� �� ���ε� �ϵ��� �ϴ� �ɼ�
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// SwapChain ����
		ThrowIfFailed(factory->CreateSwapChain
		(
			device,
			&swapChainDesc,
			&swapChain
		), TEXT("Failed To Create a Swap Chain"));

		// ���� Ÿ�� �� ����.
		ID3D11Texture2D* backbuffer = nullptr;
		ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)), TEXT("Failed To Get Back Buffer"));
		ThrowIfFailed(device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView), TEXT("Failedd To Create Render Target View"));

		// ����� ���ҽ� ����
		backbuffer->Release();
		backbuffer = nullptr;

		// ����Ʈ(ȭ��).
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// ����Ʈ ����.
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

		// �׸��� �� �۾� (BeginScene).
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);

		// �����(Clear).
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);

		// Draw
		// ī�޶� ���ε�
		if (level->GetCamera())
		{
			level->GetCamera()->Draw();
		}


		for (uint32 ix = 0; ix < level->ActorCount(); ix++)
		{
			// ���� ��������
			auto actor = level->GetActor(ix);

			// Draw
			if (actor->IsActive())
			{
				actor->Draw();
			}
		}

		// ���� ��ȯ. (EndScene/Present).
		swapChain->Present(1u, 0u);
	}

	void Renderer::OnResize(uint32 width, uint32 height)
	{
		// â �������� ���� ���ҽ� ũ�� ����
		if (!device || !context || !swapChain)
		{
			return;
		}
		isResizing = true;

		// context ����
		context->ClearState();
		context->Flush();

		// ����Ÿ�� ����
		if (renderTargetView)
		{
			renderTargetView->Release();
			renderTargetView = nullptr;
		}

		// ����ü�� ����� ũ�� ����
		ThrowIfFailed(
			swapChain->ResizeBuffers(0, width, height,
				DXGI_FORMAT_R8G8B8A8_UNORM,0), TEXT("Failed to resize swapchain buffer.")
		);
		// ����Ÿ�� �����
		ID3D11Texture2D* backbuffer = nullptr;
		ThrowIfFailed(
			swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)),
			TEXT("Failed tp get buffer from swapchain")
		);

		ThrowIfFailed(device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView), 
			TEXT("Failed To Create Render Target View"));

		// ����� ���ҽ� ����
		backbuffer->Release();
		backbuffer = nullptr;

		// ����Ʈ ������Ʈ
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// ����Ʈ ����
		context->RSSetViewports(1, &viewport);

		isResizing = false;
	}
}