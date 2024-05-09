#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;




class Window {
public:
	Window(uint32_t width, uint32_t height) {
		RegWndClass();
		_hwnd = CreateWindowEx(0,
			CLASS_NAME.c_str(),
			L"WIND_NAME",
			WS_OVERLAPPEDWINDOW |
			WS_VISIBLE | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width,
			height,
			nullptr,
			nullptr,
			GetModuleHandle(nullptr), nullptr);
	};

	bool update() {
		MSG msg;
		if (PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				return false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}

	~Window() {
		CloseWindow(_hwnd);
		update();
		UnregisterClass(CLASS_NAME.c_str(), GetModuleHandle(nullptr));
	}

private:

	void RegWndClass() {
		WNDCLASSEX win_class = { 0 };
		if (GetClassInfoEx(GetModuleHandle(nullptr), CLASS_NAME.c_str(), &win_class))
			return;

		win_class.cbSize = sizeof(WNDCLASSEX);
		win_class.style = CS_HREDRAW | CS_VREDRAW;
		win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		win_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		win_class.lpszClassName = CLASS_NAME.c_str();
		win_class.lpfnWndProc = mywndproc;

		if (!RegisterClassEx(&win_class))
			std::cout << "Unexpected error trying to start the application!\n" << std::endl;
	}

	static LRESULT CALLBACK mywndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		switch (uMsg) {

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps);

			Graphics gr(hdc);
			Pen pen(Color(255, 0, 0));

			gr.DrawLine(&pen, 0, 0, 100, 100);
			EndPaint(hWnd, &ps);


			/*WCHAR string[] = L"tau^-1";
			Font myFont(L"TimesNewRoman", 16);
			PointF origin(16.0f, 16.0f);
			SolidBrush blackBrush(Color(255, 0, 0));

			gr.DrawString(string, 6, &myFont, origin, &blackBrush);*/


			return 0;
		}
		break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	static const std::wstring CLASS_NAME;

public:
	HWND _hwnd;
};

const std::wstring Window::CLASS_NAME = L"MY_CLASS_NAME";






int main() {

	Window w(600, 300);
	while (w.update()) {


	}



	return 0;
}