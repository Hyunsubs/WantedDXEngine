#pragma once

#include <memory>
#include <windows.h>
#include <string>
#include "Type.h"

namespace Blue
{
	// ¿£Áø Å¬·¡½º
	class Engine
	{
	public:
		Engine(uint32 width, uint32 height, const std::wstring& title, HINSTANCE hInstance);
		virtual ~Engine();

		void Run();

		// À©µµ¿ì ¸Þ½ÃÁö Ã³¸® ·çÇÁ
		static LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

		// ½Ì±ÛÅæ Á¢±Ù ÇÔ¼ö
		inline static Engine& Get()
		{
			return *instance;
		}

	protected:
		// Ã¢ °´Ã¼
		std::shared_ptr<class Window> window;

		// ·»´õ·¯ °´Ã¼
		std::shared_ptr<class Renderer> renderer;

		// ½Ì±ÛÅæ °´Ã¼
		static Engine* instance;

	private:

	};

}
