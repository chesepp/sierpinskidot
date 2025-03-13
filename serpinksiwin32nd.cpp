#include <Windows.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <iostream>
#include "WindowsMessageMap.h"

COLORREF colors[] = {
	RGB(0, 0, 0),       // 0 - Black (not used in cycling)
	RGB(0, 0, 255),     // 1 - Blue
	RGB(0, 255, 0),     // 2 - Green
	RGB(0, 255, 255),   // 3 - Cyan
	RGB(255, 0, 0),     // 4 - Red
	RGB(255, 0, 255),   // 5 - Magenta
	RGB(255, 255, 0),   // 6 - Yellow
	RGB(255, 255, 255), // 7 - White
	RGB(128, 128, 128), // 8 - Gray
	RGB(0, 128, 128),   // 9 - Dark Cyan
	RGB(128, 0, 0),     // 10 - Dark Red
	RGB(128, 0, 128),   // 11 - Dark Magenta
	RGB(128, 128, 0),   // 12 - Olive
	RGB(0, 128, 0),     // 13 - Dark Green
	RGB(0, 0, 128),     // 14 - Dark Blue
	RGB(255, 165, 0)    // 15 - Orange
};

void drawEqualateralPoints(HDC hdc) {
	//HPEN hPen = CreatePen(PS_SOLID, 1, colors[2]);
	//HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	SetPixel(hdc, 0, 700, colors[2]);
	SetPixel(hdc, 800, 700, colors[2]);
	SetPixel(hdc, 400, 7, colors[2]);
	
}
//LEFT POINT IS (0,700)
//MIDDLE POINT IS (400,7)
//RIGHT POINT IS (800,700);
std::pair<int, int> findMidPoint(int x1, int y1, int x2, int y2) {
	int midpointx;
	int midpointy;
	midpointx = (x1 + x2) / 2;
	midpointy = (y1 + y2) / 2;
	return std::make_pair(midpointx, midpointy);
}
void drawPoints(HDC hdc, int numPoints)
{
	if (numPoints >= 1)
	{
		int x = 400;
		int y = 396;
		std::pair<int, int> midpoint;
		for (int i = 0; i < numPoints; i++)
		{
			int anchorpt = (rand() % 3) + 1;
			if (anchorpt % 3 == 0) {
				//if i mod 3 = 0, it finds midpoint to left point
				midpoint = findMidPoint(0, 700, x, y);
				x = midpoint.first;
				y = midpoint.second;
				std::cout << "MIDPOINT TO LEFT POINT: " << x << ", " << y << std::endl;
			}
			if (anchorpt % 3 == 1) {
				//if i mod 3 = 1, it finds midpoint to middle point
				midpoint = findMidPoint(400, 7, x, y);
				x = midpoint.first;
				y = midpoint.second;
				std::cout << "MIDPOINT TO MIDDLE POINT: " << x << ", " << y << std::endl;
			}
			if (anchorpt % 3 == 2) {
				//if i mod 3 = 2, it finds midpoint to right point
				midpoint = findMidPoint(800, 700, x, y);
				x = midpoint.first;
				y = midpoint.second;
				std::cout << "MIDPOINT TO RIGHT POINT: " << x << ", " << y << std::endl;
			}
			SetPixel(hdc, x, y, colors[rand() % 15]);
		}
	}
	
}
void SetupConsole()
{
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$","w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);

}
std::string inputNumber = "";
int inputNum = 0;
bool enterpressed = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	printf(mm(msg, lParam, wParam).c_str());
	printf(inputNumber.c_str());
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
		case WM_CHAR:
			if (wParam >= '0' && wParam <= '9')
			{
				enterpressed = false;
				inputNumber += (char)wParam;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case WM_KEYDOWN:
			if (wParam == VK_RETURN)
			{
				printf("ENTER PRESSED");
				enterpressed = true;
				inputNum = std::stoi(inputNumber);
				inputNumber = "";
				InvalidateRect(hWnd, NULL, TRUE);
			}
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			//SetTextColor(hdc, RGB(255, 255, 255));
			TextOutA(hdc, 50, 50, inputNumber.c_str(), inputNumber.size());
			drawEqualateralPoints(hdc);
			if (enterpressed == true)
			{
				drawPoints(hdc, inputNum);
			}
			EndPaint(hWnd, &ps);
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

	
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetupConsole();
	const auto pClassName = L"windowwindow";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindowEx(0, pClassName, L"windowwindow", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW, 200, 200, 800, 800, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}