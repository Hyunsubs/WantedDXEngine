#include <iostream>
#include <windows.h>
#include "Core/Window.h"

LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	// �޽��� ó��
	switch (message)
	{
		// â�� �����Ǹ� �����
	case WM_DESTROY:
		// �̶� ���α׷� ���� �޽����� ����
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wparam, lparam);
}

// â ���� �� �� ���� �Լ�(Entry Point)
int WINAPI WinMain
(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd
)
{
	// HINSTANCE :  â�� �����ϴ� �����͸� �����ϴ� ����
	// �ý����� ���� �����ϴ� �߿��� �ڿ��̱� ������ �̰��� �����ϴ� ���Ѹ� ���� �� �ִ� �ּҸ� �ޱ� ���� ����
	Blue::Window mainWnd = Blue::Window(1280, 800, L"DX Engine Demo", hInstance, WindowProc);
	

	// �޽��� ó�� ����
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		// â�� �޽����� ���ö� ����
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// �޽��� ó��
			// �޽��� ����
			TranslateMessage(&msg);
			//�޽��� ����
			DispatchMessage(&msg);
		}

		// â�� �޽����� ���� �� �ٸ� �۾� ó��
		else
		{
			// ���� ���� ����
		}
	}

	return 0;
}