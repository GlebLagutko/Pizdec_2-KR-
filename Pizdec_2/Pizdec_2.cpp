#include "stdafx.h"
#include "WinApiModule.h"
#include <string>
#define _USE_MATH_DEFINES
#include <ostream>
#include <iostream>
#include <commdlg.h>


#define DLGTITLE  L"My dialog"
#define DLGFONT   L"MS Shell Dlg"

using namespace std;

bool paused = false;
const int g = 1;
int speed = 0;
int t = 0;
bool down = true;
int floor1 = 400;
int h = 340;
int top = floor1 - h;

HINSTANCE hInst;

namespace Color
{
	static COLORREF FromHex(int hex)
	{
		auto r = (hex & 0xFF0000) >> 16;
		auto g = (hex & 0x00FF00) >> 8;
		auto b = (hex & 0x0000FF) >> 0;
		return RGB(r, g, b);
	}

	static const COLORREF Firebrick = FromHex(0xB22222);
	static const COLORREF Goldenrod1 = FromHex(0xFFC125);
	static const COLORREF SpringGreen3 = FromHex(0x00CD66);
	static const COLORREF Black = FromHex(0x000000);
	static const COLORREF White = FromHex(0xFFFFFF);
	static const COLORREF SteelBlue2 = FromHex(0x5CACEE);
	static const COLORREF SlateBlue = FromHex(0x6A5ACD);
	static const COLORREF Sienna1 = FromHex(0xFF8247);
	static const COLORREF Turquoise3 = FromHex(0x00C5CD);
}

namespace Control
{
	int const IDC_TestTextBoxB = 1001;
	int const IDC_TestTextBoxA = 1002;
	int const IDC_TestButtonOk = 1003;
	int const IDC_TestButtonCancel = 1004;
	int const IDC_TestARadioButton = 1005;
	int const IDC_TestBRadioButton = 1006;
	int const IDC_TestComboBox = 1007;
	int const IDC_InvisibleButton = 1008;
	int const IDC_Test2GroupBox = 1009;
	int const IDC_TestCRadioButton = 1010;
	int const IDC_TestDRadioButton = 1011;
	int const IDC_MessageButton = 1012;
	int const IDC_TestLabel = 1013;
}


#pragma pack(push, 4)                 
static struct
{
	DWORD  style;
	DWORD  dwExtendedStyle;
	WORD   ccontrols; // ?????????? ?????????. ???????? ????? ????????? ????? ???, ? ???? ????????? ????? ???? ?????, ?? ??????
	short  x;
	short  y;
	short  cx;
	short  cy;
	WORD   menu;
	WORD   windowClass;
	WCHAR  wszTitle[ARRAYSIZE(DLGTITLE)];
	short  pointsize; // ??????????????? ?????????. ??. FromUnit ??? ?????????????? ?????????.
	WCHAR  wszFont[ARRAYSIZE(DLGFONT)];
}
EmptyDialogTemplate =
{
   DS_SHELLFONT | DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU, // ?????????!
   0x0,
   0,
	0,  0, 150, 80,
   0,                       // menu: none
   0,                       // window class: none
   DLGTITLE,                // Window caption
   8,                       // font pointsize
   DLGFONT,
};
#pragma pack(pop)

void CheckDown()
{
	if(top + g * (t + 1) * (t + 1 ) / 2  > floor1 - 40)
	{
		top = floor1 - 40;
		t = 0;
		down = false;
	}
	if (top + g * (t + 1) * (t + 1) / 2 - speed * (t + 1) < floor1 - h && !down)
	{
		speed = 0;
		top = floor1 - h;
		t = 0;
		down = true;
	}

}

int GetNewHeight(HWND hWnd)
{
	auto textLength = GetWindowTextLengthW(hWnd);
	auto buffer = new wchar_t[textLength + 1];
	GetWindowText(hWnd, buffer, textLength + 1);
	int result = wcstod(buffer, NULL);
	return result;
}



INT_PTR CALLBACK DlgProc(HWND hwndDlg, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		CreateWindowEx(NULL, TEXT("STATIC"), TEXT("Edit Height : "),
			WS_CHILD | WS_VISIBLE, 10, 10, 80,
			20, hwndDlg, nullptr, nullptr, nullptr);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE, 100, 10, 60,
			20, hwndDlg, (HMENU)Control::IDC_TestTextBoxA, nullptr, nullptr);

		


		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Button"), TEXT("Ok"),
			WS_CHILD | WS_VISIBLE, 100, 40, 30,
			20, hwndDlg, (HMENU)Control::IDC_TestButtonOk, NULL, NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Button"), TEXT("Cancel"),
			WS_CHILD | WS_VISIBLE, 150, 40, 50,
			20, hwndDlg, (HMENU)Control::IDC_TestButtonCancel, NULL, NULL);


		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
	{
		if (wParam == Control::IDC_TestButtonOk)
		{
			
			auto TextA = GetDlgItem(hwndDlg, Control::IDC_TestTextBoxA);
			if (GetWindowTextLengthW(TextA) != 0)
				if (GetNewHeight(TextA) > 40)
				{
					h = GetNewHeight(TextA);
					top = floor1 - h;
					if (paused)
						paused = !paused;
				}
				else
					MessageBoxA(hwndDlg, "Error!!! Wrong Height", NULL, MB_OK);

			EndDialog(hwndDlg, IDCANCEL);
		}
		if (wParam == Control::IDC_TestButtonCancel)
			EndDialog(hwndDlg, IDCANCEL);
	}
	break;
	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
		return FALSE;
	}
	}
	return FALSE;
}

class Application
{
	static const int MinDialogWidth = 300;
	static const int MinDialogHeight = 250;

	static const int Timer1ID = 1;

	static const int Timer1Interval = 17;

	static const int Timer1MaxValue = 1;
	
	static int _timer1Ticks;

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			OnCreate(hWnd);
			return true;
		}

		case WM_GETMINMAXINFO:
		{
			OnGetMinMaxInfo((LPMINMAXINFO)lParam);
			return TRUE;
		}

		case WM_TIMER:
		{
			OnTimer1(hWnd, (int)wParam);
			return true;
		}

		case WM_KEYDOWN:
		{
			
			if (wParam == VK_F5)
			{
				top = floor1 - h;
			}
		    if(wParam == VK_SPACE)
		    {
				paused = !paused;
		    }
			break;
		}

		case WM_RBUTTONDOWN :
		{
			DialogBoxIndirectParamW(hInst, (LPCDLGTEMPLATEW)&EmptyDialogTemplate, hWnd, DlgProc, NULL);
			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps);
			OnPaint(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
		}


		case WM_DESTROY:
			PostQuitMessage(0);
			break;



		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

private:
	static void OnCreate(HWND hWnd)
	{
		SetTimer(hWnd, Timer1ID, Timer1Interval, nullptr);
	}

	static void OnGetMinMaxInfo(LPMINMAXINFO minMaxInfo)
	{
		minMaxInfo->ptMinTrackSize.x = MinDialogWidth;
		minMaxInfo->ptMinTrackSize.y = MinDialogHeight;
	}

	static void CalculatePosition()
	{
		if (_timer1Ticks == 0 && !paused)
		{
			t++;
			if (down)
			{
				top += g * t * t / 2;
				speed = g * t;
			}
			if (!down)
			{
				top = top + g * t * t / 2 - speed * t;
			}
		}
	}

	static void OnTimer1(HWND hWnd, int timerID)
	{
		if (timerID != Timer1ID) return;
		if (!paused)
			++_timer1Ticks;

		if (_timer1Ticks > Timer1MaxValue)
		{
			_timer1Ticks = 0;
		}

		InvalidateRect(hWnd, nullptr, true);
	}


	static void OnPaint(HWND hWnd, HDC hdc)
	{

		auto clientRect = GetClientRect(hWnd);
		auto width = (clientRect.right - clientRect.left) / 2;
		auto height = (clientRect.bottom - clientRect.top) / 2;
		
		CheckDown();
		CalculatePosition();
		
		SelectObject(hdc, CreateSolidBrush(Color::SteelBlue2));
		Rectangle(hdc, width - 200, floor1, width + 200,floor1 + 50);
		SelectObject(hdc, CreateSolidBrush(Color::Firebrick));
		Ellipse(hdc, width - 20, top, width + 20, top + 40 ) ;
		auto pen3 = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
		SelectObject(hdc, pen3);
		LineTo(hdc, width, top + 40 );
		auto pen2 = CreatePen(PS_SOLID, 1, Color::Sienna1);
		SelectObject(hdc, pen2);
		LineTo(hdc, width, floor1);

	}


	static RECT GetClientRect(HWND hWnd)
	{
		RECT rect;
		::GetClientRect(hWnd, &rect);
		return rect;
	}

	static HFONT CreateFont(std::wstring name, int size, int weight)
	{
		auto lf = LOGFONT{};
		wcscpy_s(lf.lfFaceName, name.data());
		lf.lfHeight = size;
		lf.lfWeight = weight;
		lf.lfCharSet = DEFAULT_CHARSET;
		return CreateFontIndirect(&lf);
	}

	static void SelectObjectAndDeletePrevious(HDC hDeviceContext, HGDIOBJ gdiObject)
	{
		auto previousGdiObject = SelectObject(hDeviceContext, gdiObject);
		DeleteObject(previousGdiObject);
	}

};

int Application::_timer1Ticks = 0;

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto winApiModule = WinApiModule(
		L"9_Timer_and_Drawing",
		L"9. ?????? ? ?????????",
		hInstance,
		Application::WndProc);

	return winApiModule.Run(nCmdShow);
}