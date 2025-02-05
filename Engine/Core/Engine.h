#pragma once

#include <memory>
#include <windows.h>
#include <string>
#include "Type.h"

namespace Blue
{
	// ���� Ŭ����
	class Engine
	{
	public:
		Engine(uint32 width, uint32 height, const std::wstring& title, HINSTANCE hInstance);
		virtual ~Engine();

		void Run();

		// ������ �޽��� ó�� ����
		static LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

		// �̱��� ���� �Լ�
		inline static Engine& Get()
		{
			return *instance;
		}

	protected:
		// â ��ü
		std::shared_ptr<class Window> window;

		// ������ ��ü
		std::shared_ptr<class Renderer> renderer;

		// �̱��� ��ü
		static Engine* instance;

	private:

	};

}
