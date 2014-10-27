// MiniPaint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "MiniPaint.h"
#include <windows.h>
#include <tchar.h>
#include <CommDlg.h>
#include <string.h>

#define MAX_LOADSTRING 100
#define OPEN 1
#define SAVE 2
#define PRINT 3
#define EXIT 4
#define PEN 5
#define LINE 6
#define ELLIPSE 7
#define REC 8
#define POLYLINE 9
#define POLYGON 10
#define TEXT 11
#define ZOOM 12
#define PAN 13
#define RED 14
#define GREEN 15
#define BLUE 16
#define BLACK 17
#define FILLRED 18
#define FILLGREEN 19
#define FILLBLUE 20
#define FILLBLACK 21
#define FILLNONE 22
#define WIDTH1 23
#define WIDTH2 24
#define WIDTH3 25
#define WIDTH4 26
#define ABOUT 27

#define MAX_LOADSTRING 100

// Глобальные переменные:
int Menu(HWND);
HINSTANCE hInst;// текущий экземпляр
BOOL flag = false, Poly_line = false, Zooming = false, fText = false, keyflag = false;
HPEN PenColor = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
DWORD color = RGB(0, 0, 0);
HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
int width = 1;
INT startX, endX, startY, endY, Tools;
PAINTSTRUCT ps;
RECT rec;
HDC hDC;
HDC hdcMem;
HBITMAP hbmMem;
HANDLE hOld;
HPEN hPen;
char *text;
int i = 0;
int xscale_last = 0;
int yscale_last = 0;
int xscale, yscale;
int spacex = 0;
int spacey = 0;
int xlast = 0;
int ylast = 0;
BOOL SaveFile(HWND);
BOOL OpenFile(HWND);
BOOL PrintFile(HWND);
VOID ClearWindow(HWND);
void Draw(HWND, LPARAM);
void DrawPen(HWND, INT, INT, LPARAM, HPEN);
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MINIPAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIPAINT));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINIPAINT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MINIPAINT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   Menu(hWnd);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
INT Menu(HWND hMainWnd)
{
	HMENU MainMenu = CreateMenu();
	HMENU hPopupMenu = CreatePopupMenu();
	HMENU hToolsMenu = CreatePopupMenu();
	HMENU hColorMenu = CreatePopupMenu();
	HMENU hFillColorMenu = CreatePopupMenu();
	HMENU hWidthMenu = CreatePopupMenu();
	AppendMenu(MainMenu, MFT_STRING | MF_POPUP, (UINT)hPopupMenu, L"&Файл");
	{
		AppendMenu(hPopupMenu, MF_STRING, 28, L"Очистить");
		AppendMenu(hPopupMenu, MF_STRING, OPEN, L"Открыть");
		AppendMenu(hPopupMenu, MF_STRING, SAVE, L"Сохранить");
		AppendMenu(hPopupMenu, MF_STRING, PRINT, L"Печать");
		AppendMenu(hPopupMenu, MF_STRING, EXIT, L"Выход");
	}
	AppendMenu(MainMenu, MFT_STRING | MF_POPUP, (UINT)hToolsMenu, L"Инструменты");
	{
		AppendMenu(hToolsMenu, MF_STRING, PEN, L"Карандаш");
		AppendMenu(hToolsMenu, MF_STRING, LINE, L"Линия");
		AppendMenu(hToolsMenu, MF_STRING, ELLIPSE, L"Эллипс");
		AppendMenu(hToolsMenu, MF_STRING, REC, L"Прямоугольник");
		AppendMenu(hToolsMenu, MF_STRING, POLYLINE, L"Ломанная");
		AppendMenu(hToolsMenu, MF_STRING, POLYGON, L"Многоугольник");
		AppendMenu(hToolsMenu, MF_STRING, TEXT, L"Текст");
		AppendMenu(hToolsMenu, MF_STRING, ZOOM, L"Масштабирование");
		AppendMenu(hToolsMenu, MF_STRING, PAN, L"Перемещение");
	}
	AppendMenu(MainMenu, MFT_STRING | MF_POPUP, (UINT)hColorMenu, L"&Цвет");
	{
		AppendMenu(hColorMenu, MF_STRING, RED, L"Красный");
		AppendMenu(hColorMenu, MF_STRING, GREEN, L"Зелёный");
		AppendMenu(hColorMenu, MF_STRING, BLUE, L"Синий");
		AppendMenu(hColorMenu, MF_STRING, BLACK, L"Чёрный");
	}
	AppendMenu(MainMenu, MFT_STRING | MF_POPUP, (UINT)hFillColorMenu, L"&Заливка");
	{

		AppendMenu(hFillColorMenu, MF_STRING, FILLRED, L"Красный");
		AppendMenu(hFillColorMenu, MF_STRING, FILLGREEN, L"Зелёный");
		AppendMenu(hFillColorMenu, MF_STRING, FILLBLUE, L"Синий");
		AppendMenu(hFillColorMenu, MF_STRING, FILLBLACK, L"Чёрный");
		AppendMenu(hFillColorMenu, MF_STRING, FILLNONE, L"Прозрачный");
	}
	AppendMenu(MainMenu, MFT_STRING | MF_POPUP, (UINT)hWidthMenu, L"&Толщина");
	{
		AppendMenu(hWidthMenu, MF_STRING, WIDTH1, L"1");
		AppendMenu(hWidthMenu, MF_STRING, WIDTH2, L"2");
		AppendMenu(hWidthMenu, MF_STRING, WIDTH3, L"3");
		AppendMenu(hWidthMenu, MF_STRING, WIDTH4, L"4");
	}
	AppendMenu(MainMenu, MFT_STRING | MF_POPUP, ABOUT, L"О программе");
	SetMenu(hMainWnd, MainMenu);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		flag = false;
		switch (wmId)
		{
		case 28:ClearWindow(hWnd); break;
		case OPEN:OpenFile(hWnd); break;
		case SAVE:SaveFile(hWnd); break;
		case PRINT:PrintFile(hWnd); break;
		case PEN:Tools = PEN; break;
		case LINE:Tools = LINE; break;
		case ELLIPSE:Tools = ELLIPSE; break;
		case REC:Tools = REC; break;
		case POLYLINE:Tools = POLYLINE; break;
		case POLYGON:Tools = POLYGON; break;
		case TEXT:Tools = TEXT; fText = true; i = 0; break;
		case ZOOM:Tools = ZOOM; xscale = xscale_last; yscale = yscale_last; break;
		case PAN:Tools = PAN; spacex = xlast; spacey = ylast; break;
		case RED:color = RGB(255, 0, 0); hPen = CreatePen(PS_SOLID, width, color); break;
		case GREEN:color = RGB(0, 255, 0); hPen = CreatePen(PS_SOLID, width, color); break;
		case BLUE:color = RGB(0, 0, 255); hPen = CreatePen(PS_SOLID, width, color); break;
		case BLACK:color = RGB(0, 0, 0); hPen = CreatePen(PS_SOLID, width, color); break;
		case FILLRED:hBrush = CreateSolidBrush(RGB(255, 0, 0)); break;
		case FILLGREEN:hBrush = CreateSolidBrush(RGB(0, 255, 0)); break;
		case FILLBLUE:hBrush = CreateSolidBrush(RGB(0, 0, 255)); break;
		case FILLBLACK:hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
		case FILLNONE:hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH); break;
		case WIDTH1:width = 1; hPen = CreatePen(PS_SOLID, width, color); break;
		case WIDTH2:width = 2; hPen = CreatePen(PS_SOLID, width, color); break;
		case WIDTH3:width = 3; hPen = CreatePen(PS_SOLID, width, color); break;
		case WIDTH4:width = 4; hPen = CreatePen(PS_SOLID, width, color); break;
		case ABOUT:	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
		case EXIT:DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rec);
		BitBlt(hDC, spacex, spacey, rec.right, rec.bottom, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		spacex = 0;
		spacey = 0;
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rec);
		hdcMem = CreateCompatibleDC(hDC);
		hbmMem = CreateCompatibleBitmap(hDC, rec.right, rec.bottom);
		hOld = SelectObject(hdcMem, hbmMem);
		BitBlt(hdcMem, 0, 0, rec.right, rec.bottom, hDC, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hDC);
		if (Tools == TEXT)
		{
			fText = true;
			i = 0;
			text = (char *)calloc(256, sizeof(char));
		}
		if (Tools == POLYLINE)
			Poly_line = true;
		else
			Poly_line = false;
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		flag = true;
		break;
	case WM_MOUSEMOVE:
		if (flag&&Poly_line)
			Draw(hWnd, lParam);
		if (flag)
			Draw(hWnd, lParam);
		break;
	case WM_LBUTTONUP:
		if (Poly_line)
		{
			SelectObject(hDC, hPen);
			SelectObject(hDC, hBrush);
			MoveToEx(hDC, startX, startY, 0);
			LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
			startX = LOWORD(lParam);
			startY = HIWORD(lParam);
		}
		else
			flag = false;
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rec);
		BitBlt(hdcMem, 0, 0, rec.right, rec.bottom, hDC, 0, 0, SRCCOPY);
		break;
	case WM_RBUTTONDOWN:
		hDC = GetDC(hWnd);
		SelectObject(hDC, hPen);
		SelectObject(hDC, hBrush);
		GetClientRect(hWnd, &rec);
		InvalidateRect(hWnd, &rec, true);
		MoveToEx(hDC, startX, startY, 0);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		flag = false;
		Poly_line = false;
		break;
	case WM_CHAR:
		if (Tools == TEXT)
		{
			hDC = GetDC(hWnd);
			char c = (char)wParam;
			if (c == VK_RBUTTON)
			{
				fText = false;
				text[i] = '\0';
			}
			else
				text[i++] = c;
			TextOutA(hDC, startX, startY, (LPSTR)text, strlen((char *)text));
		}
		break;
	case WM_KEYUP:
		keyflag = false;
		break;
	case WM_MOUSEWHEEL:
		if (GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL)
		{
			if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
			{
				hDC = GetDC(hWnd);
				GetClientRect(hWnd, &rec);
				xscale += 10;
				yscale += 10;
				StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
				ReleaseDC(hWnd, hDC);
			}
			else
			{
				hDC = GetDC(hWnd);
				GetClientRect(hWnd, &rec);
				xscale -= 10;
				yscale -= 10;
				StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
				ReleaseDC(hWnd, hDC);
			}
			xscale_last = xscale;
			yscale_last = yscale;
		}
		else
		if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
		{
			if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
			{
				hDC = GetDC(hWnd);
				GetClientRect(hWnd, &rec);
				InvalidateRect(hWnd, &rec, true);
				UpdateWindow(hWnd);
				spacex += 3;
				StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
				ReleaseDC(hWnd, hDC);
			}
			else
			{
				hDC = GetDC(hWnd);
				GetClientRect(hWnd, &rec);
				InvalidateRect(hWnd, &rec, true);
				UpdateWindow(hWnd);
				spacex -= 3;
				StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
				ReleaseDC(hWnd, hDC);
			}
			xlast = spacex;
			ylast = spacey;
		}
		else
		{
			if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
			{
				hDC = GetDC(hWnd);
				GetClientRect(hWnd, &rec);
				InvalidateRect(hWnd, &rec, true);
				UpdateWindow(hWnd);
				spacey -= 3;
				StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
				ReleaseDC(hWnd, hDC);

			}
			else
			{
				hDC = GetDC(hWnd);
				GetClientRect(hWnd, &rec);
				InvalidateRect(hWnd, &rec, true);
				UpdateWindow(hWnd);
				spacey += 3;
				StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
				ReleaseDC(hWnd, hDC);

			}
			xlast = spacex;
			ylast = spacey;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL SaveFile(HWND hWnd)
{
	RECT _rect = rec;
	hDC = GetDC(hWnd);
	int Width_mm = GetDeviceCaps(hDC, HORZSIZE);
	int Height_mm = GetDeviceCaps(hDC, VERTSIZE);
	int Width_pcs = GetDeviceCaps(hDC, HORZRES);
	int Height_pcs = GetDeviceCaps(hDC, VERTRES);

	_rect.left = (_rect.left * Width_mm * 100) / Width_pcs;
	_rect.top = (_rect.top * Height_mm * 100) / Height_pcs;
	_rect.right = (_rect.right * Width_mm * 100) / Width_pcs;
	_rect.bottom = (_rect.bottom * Height_mm * 100) / Height_pcs;

	TCHAR szFilters[] = _T("Scribble Files (*.emf)\0*.emf\0\0");
	TCHAR szFilePathName[_MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	szFilePathName[0] = '\0';
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.lpstrDefExt = _T("emf");
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Save File");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	GetSaveFileName(&ofn);
	HDC hdcMeta = CreateEnhMetaFile(NULL, (LPTSTR)ofn.lpstrFile, &_rect, L"Create Meta File\0");
	StretchBlt(hdcMeta, 0, 0, rec.right, rec.bottom, hDC, 0, 0, rec.right, rec.bottom, SRCCOPY);
	CloseEnhMetaFile(hdcMeta);

	return TRUE;
}

BOOL OpenFile(HWND hWnd)
{
	HDC hDC = GetDC(hWnd);
	GetClientRect(hWnd, &rec);
	TCHAR szFilters[] = _T("Scribble Files (*.emf)\0*.emf\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = { 0 };
	if (hdcMem == NULL)
	{
		hdcMem = CreateCompatibleDC(hDC);
		hbmMem = CreateCompatibleBitmap(hDC, rec.right, rec.bottom);
		hOld = SelectObject(hdcMem, hbmMem);
	}
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.lpstrDefExt = _T("emf");
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Open File");
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	HENHMETAFILE hemf = GetEnhMetaFile(ofn.lpstrFile);
	if (hemf!=NULL)
		ClearWindow(hWnd);
	PlayEnhMetaFile(hDC, hemf, &rec);
	DeleteEnhMetaFile(hemf);

	return TRUE;
}

BOOL PrintFile(HWND hWnd)
{
	PRINTDLG pd;
	hDC = GetDC(hWnd);
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner = hWnd;
	pd.hDevMode = NULL;
	pd.hDevNames = NULL;
	pd.Flags = PD_RETURNDC | PD_NOSELECTION | PD_PRINTTOFILE | PD_NOPAGENUMS;
	pd.nMinPage = 1;
	pd.nMaxPage = 1;
	pd.nCopies = 1;

	if (PrintDlg(&pd) == TRUE)
	{
		DOCINFO doc;
		GlobalUnlock(pd.hDevMode);
		DEVNAMES * pdn = (DEVNAMES *)GlobalLock(pd.hDevNames);
		ZeroMemory(&doc, sizeof(doc));
		doc.cbSize = sizeof(doc);
		doc.lpszDatatype = _T("RAW");
		doc.lpszOutput = (TCHAR *)pdn + pdn->wOutputOffset;
		int sd = StartDoc(pd.hDC, &doc);
		StartPage(pd.hDC);
		BitBlt(pd.hDC, 0, 0, rec.right, rec.bottom, hDC, 0, 0, SRCCOPY);
		EndPage(pd.hDC);
		DeleteDC(pd.hDC);
	}

	return TRUE;
}

VOID Draw(HWND hWnd, LPARAM lParam)
{
	HDC hDC = GetDC(hWnd);
	GetClientRect(hWnd, &rec);
	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush);
	switch (Tools)
	{
	case PEN:
		MoveToEx(hDC, startX, startY, 0);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		break;
	case LINE:
		GetClientRect(hWnd, &rec);
		InvalidateRect(hWnd, &rec, true);
		UpdateWindow(hWnd);
		MoveToEx(hDC, startX, startY, 0);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		break;
	case ELLIPSE:
		GetClientRect(hWnd, &rec);
		InvalidateRect(hWnd, &rec, true);
		UpdateWindow(hWnd);
		Ellipse(hDC, startX, startY, LOWORD(lParam), HIWORD(lParam));
		break;
	case REC:
		GetClientRect(hWnd, &rec);
		InvalidateRect(hWnd, &rec, true);
		UpdateWindow(hWnd);
		Rectangle(hDC, startX, startY, LOWORD(lParam), HIWORD(lParam));
		break;
	case POLYLINE:
		GetClientRect(hWnd, &rec);
		InvalidateRect(hWnd, &rec, true);
		UpdateWindow(hWnd);
		MoveToEx(hDC, startX, startY, 0);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		break;
	case POLYGON:
		GetClientRect(hWnd, &rec);
		InvalidateRect(hWnd, &rec, true);
		UpdateWindow(hWnd);

		MoveToEx(hDC, startX, startY, 0);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));

		MoveToEx(hDC, LOWORD(lParam), HIWORD(lParam), 0);
		LineTo(hDC, LOWORD(lParam) - (LOWORD(lParam) - startX) * 2, HIWORD(lParam));

		MoveToEx(hDC, LOWORD(lParam) - (LOWORD(lParam) - startX) * 2, HIWORD(lParam), 0);
		LineTo(hDC, startX, startY);

		break;
	};
	DeleteObject(hPen);
	DeleteObject(hBrush);
}
VOID ClearWindow(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	GetClientRect(hWnd, &rec);
	FillRect(hdc, &rec, (HBRUSH)(COLOR_WINDOW + 1));
	BitBlt(hdcMem, 0, 0, rec.right, rec.bottom, hdc, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}
