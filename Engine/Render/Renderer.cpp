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
		swapChainDesc.BufferCount = 1; // 백버퍼 갯수
		swapChainDesc.SampleDesc.Count = 1;   // 멀티 샘플링 갯수
		swapChainDesc.SampleDesc.Quality = 0; // 멀티 샘플링 수준(보통 Count - 1)
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


		// 장치 생성
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
		
		// 결과 확인
		if (FAILED(result))
		{
			MessageBoxA(window, "Failed to create devices.", "Error", MB_OK);
			__debugbreak();
		}

		// context 옆에 d3dfeaturelevel은 선택된 버전을 알려주기 위해 주소를 받지만
		// 알 필요가 없으므로 nullptr을 넣음


		// 렌더 타겟 뷰 생성
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

		// 렌더 타겟 뷰 바인딩(연결)
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);
		// context 바인딩은 보통 오류 메시지를 반환하지 않기 때문에
		// GetLastError등의 함수로 오류를 판별하여 예외처리 하는 방법이 있음

		// 뷰포트(화면)
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.f;
		viewport.MinDepth = 0.f;

		// 뷰포트 설정
		context->RSSetViewports(1, &viewport);

		// 정점 데이터 생성
		// vertex -> vertices
		float vertices[] = 
		{
			0.f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f
		};


		// @Temp: 임시 리소스 생성
		// 버퍼(Buffer) - 메모리 덩어리
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


		// 인덱스 버퍼 생성(정점을 조립하는 순서)
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

		// 쉐이더 컴파일
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

		// 쉐이더 생성
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

		// 입력 레이아웃
		// 정점 쉐이더에 전달할 정점 데이터가 어떻게 생겼는지 알려줌
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
		// 그리기 전 작업 (BeginScene)
		float color[4] = { 0.4f, 0.4f, 0.4f, 1.f };
		context->ClearRenderTargetView(renderTargetView, color);

		// Draw, Render


		// 버퍼 교환 EndScene, Present
		swapChain->Present(1u, 0u);
	}
}