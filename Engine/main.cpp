#include <iostream>
#include <windows.h>
#include "Core/Window.h"

LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	// 메시지 처리
	switch (message)
	{
		// 창이 삭제되면 실행됨
	case WM_DESTROY:
		// 이때 프로그램 종료 메시지를 발행
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wparam, lparam);
}

// 창 모드로 할 때 메인 함수(Entry Point)
int WINAPI WinMain
(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd
)
{
	// HINSTANCE :  창을 제어하는 포인터를 관리하는 변수
	// 시스템이 직접 관리하는 중요한 자원이기 때문에 이것을 제어하는 권한만 받을 수 있는 주소를 받기 위한 변수
	Blue::Window mainWnd = Blue::Window(1280, 800, L"DX Engine Demo", hInstance, WindowProc);
	

	// 메시지 처리 루프
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		// 창에 메시지가 들어올때 실행
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 메시지 처리
			// 메시지 번역
			TranslateMessage(&msg);
			//메시지 전달
			DispatchMessage(&msg);
		}

		// 창에 메시지가 없을 때 다른 작업 처리
		else
		{
			// 게임 엔진 루프
		}
	}

	return 0;
}