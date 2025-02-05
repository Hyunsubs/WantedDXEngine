#include <iostream>
#include <windows.h>
#include <crtdbg.h>
#include "Core/Engine.h"

using namespace Blue;

// 창 모드로 할 때 메인 함수(Entry Point)
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Leak Check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 엔진 생성 및 실행
	Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);
	engine.Run();

	return 0;
}