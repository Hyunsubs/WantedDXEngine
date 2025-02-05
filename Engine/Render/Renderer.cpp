#include "Renderer.h"
#include <vector>
#include <d3dcompiler.h>

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
		swapChainDesc.Windowed = true;
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1; // ����� ����
		swapChainDesc.SampleDesc.Count = 1;   // ��Ƽ ���ø� ����
		swapChainDesc.SampleDesc.Quality = 0; // ��Ƽ ���ø� ����(���� Count - 1)
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


		// ��ġ ����
		auto result = D3D11CreateDeviceAndSwapChain(
			nullptr, 
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION,
			&swapChainDesc, &swapChain,
			&device, nullptr, &context
			);
		
		// ��� Ȯ��
		if (FAILED(result))
		{
			MessageBoxA(window, "Failed to create devices.", "Error", MB_OK);
			__debugbreak();
		}

		// context ���� d3dfeaturelevel�� ���õ� ������ �˷��ֱ� ���� �ּҸ� ������
		// �� �ʿ䰡 �����Ƿ� nullptr�� ����


		// ���� Ÿ�� �� ����
		ID3D11Texture2D* backbuffer = nullptr;
		/*swapChain->GetBuffer(
		    0,
			__uuidof(backbuffer),
			reinterpret_cast<void**>(&backbuffer)
		);*/
		result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
		if (FAILED(result))
		{
			MessageBoxA(window, "Failed to get back buffer.", "Error", MB_OK);
			__debugbreak();
		}

		result = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);
		if (FAILED(result))
		{
			MessageBoxA(window, "Failed to create render target view.", "Error", MB_OK);
			__debugbreak();
		}

		// ���� Ÿ�� �� ���ε�(����)
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);
		// context ���ε��� ���� ���� �޽����� ��ȯ���� �ʱ� ������
		// GetLastError���� �Լ��� ������ �Ǻ��Ͽ� ����ó�� �ϴ� ����� ����

		// ����Ʈ(ȭ��)
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.f;
		viewport.MinDepth = 0.f;

		// ����Ʈ ����
		context->RSSetViewports(1, &viewport);

		// ���� ������ ����
		// vertex -> vertices
		float vertices[] = 
		{
			0.f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f
		};


		// @Temp: �ӽ� ���ҽ� ����
		// ����(Buffer) - �޸� ���
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = sizeof(float) * 3 * 3;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA resourceData;
		resourceData.pSysMem = vertices;

		result = device->CreateBuffer(&bufferDesc, &resourceData, &vertexBuffer);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to Create Vertex Buffer", "Error", MB_OK);
			__debugbreak();
		}


		// �ε��� ���� ����(������ �����ϴ� ����)
		int indices[] = { 0, 1, 2 };
		resourceData.pSysMem = indices;

		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(int) * 3;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		result = device->CreateBuffer(&bufferDesc, &resourceData, &indexBuffer);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to Create Index Buffer", "Error", MB_OK);
			__debugbreak();
		}

		// ���̴� ������
		ID3DBlob* vertexShaderBuffer = nullptr;
		result = D3DCompileFromFile(TEXT("VertexShader.hlsl"), nullptr, nullptr, "main", "vs_5_0",0,0, &vertexShaderBuffer, nullptr);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to Compile Vertex Shader", "Error", MB_OK);
			__debugbreak();
		}

		ID3DBlob* pixelShaderBuffer = nullptr;
		result = D3DCompileFromFile(TEXT("PixelShader.hlsl"), nullptr, nullptr, "main", "ps_5_0", 0, 0, &pixelShaderBuffer, nullptr);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to Compile Pixel Shader", "Error", MB_OK);
			__debugbreak();
		}

		// ���̴� ����
		result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to Create Vertex Shader", "Error", MB_OK);
			__debugbreak();
		}

		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to Create Pixel Shader", "Error", MB_OK);
			__debugbreak();
		}

		// �Է� ���̾ƿ�
		// ���� ���̴��� ������ ���� �����Ͱ� ��� ������� �˷���
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		{
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		};

		result = device->CreateInputLayout(inputDesc, 1, vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(), &inputlayout);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to Create Input Layout", "Error", MB_OK);
			__debugbreak();
		}


		//context->VSSetShader(vertexShader, );
		//context->PSSetShader(pixelShader, );
	}


	Renderer::~Renderer()
	{
	}


	void Renderer::Draw()
	{
		// �׸��� �� �۾� (BeginScene)
		float color[4] = { 0.4f, 0.4f, 0.4f, 1.f };
		context->ClearRenderTargetView(renderTargetView, color);

		// Draw, Render


		// ���� ��ȯ EndScene, Present
		swapChain->Present(1u, 0u);
	}
}