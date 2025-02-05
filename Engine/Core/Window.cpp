#include "Window.h"


namespace Blue
{
	Window::Window(uint32 width, uint32 height, const std::wstring& title, HINSTANCE instance, WNDPROC messageProcedure)
		: width(width)
		, height(height)
		, title(title)
		, instance(instance)
	{

		WNDCLASS wc = { };

		// �޽��� ó���� ���� �ݹ� ����
		wc.lpfnWndProc = messageProcedure;

		// ���α׷� �ּ� ����
		wc.hInstance = instance;

		// â ������ ����� Ŭ���� �̸� ����
		wc.lpszClassName = className.c_str();

		// Ŭ���� ���
		if (!RegisterClass(&wc))
		{
			// ���� �޽��� Ȯ��
			// auto error = GetLastError();

			// �޽��� ���
			OutputDebugStringA("â ��� ����\n");

			// �޽��� �ڽ� �̿�
			MessageBoxA(nullptr, "â ���� ����", "�׽�Ʈ �޽���", MB_OK);

			// �ߴ��� ����
			__debugbreak();
		}

		// â�� ���� ũ��
		unsigned int positionX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		unsigned int positionY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		// â ũ�� ����
		RECT rect = { 0, 0, (long)width, (long)height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		unsigned int windowWidth = rect.right - rect.left;
		unsigned int windowHeight = rect.bottom - rect.top;

		// ����
		handle = CreateWindow(
			className.c_str(),                     // Ŭ���� ����
			title.c_str(),			   // ������ �ؽ�Ʈ
			WS_OVERLAPPEDWINDOW,            // â ��Ÿ��

			// â ��ġ�� ������
			positionX, positionY,
			windowWidth,
			windowHeight,

			nullptr,       // Parent window    
			nullptr,       // Menu
			instance,  // Instance handle
			nullptr
		);

		// ���� �� ����
		if (handle == nullptr)
		{
			return;
		}

		// â ���̱�
		ShowWindow(handle, 10);

		// â �޽��� ������Ʈ
		UpdateWindow(handle);
	}

	Window::~Window()
	{
		// Ŭ���� ��� ����
		UnregisterClass(className.c_str(), instance);
	}

	inline void Window::SetWidthHeight(uint32 width, uint32 height)
	{
		this->width = width;
		this->height = height;
	}
}
