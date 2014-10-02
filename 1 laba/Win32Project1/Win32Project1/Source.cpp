
#include <windows.h> 
#define PEN 5
#define LINE 6
#define ELLIPSE 7
#define REC 8
#define POLYLINE 9
#define POLYGON 10
#define TEXT 11
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int Menu(HWND);
BOOL flag=false,Poly_line=false,Zooming=false;
HPEN RedPen=CreatePen(PS_SOLID,1,RGB(255,0,0));
INT startX,endX,startY,endY,Tools;
PAINTSTRUCT ps;
RECT rec;
HDC hDC;
HDC hdcMem;
HBITMAP hbmMem;
HANDLE hOld;

void Draw(HWND,LPARAM);
void DrawPen(HWND,INT,INT,LPARAM,HPEN);
void StoreBitmapFile(LPSTR,HBITMAP);

int WINAPI WinMain(HINSTANCE hInst, 
                   HINSTANCE hPrevInst,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    TCHAR szClassName[] = L"Мой класс";
    HWND hMainWnd; 
    MSG msg; 
    WNDCLASSEX wc;
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClassName;
    wc.cbWndExtra    = NULL;
    wc.cbClsExtra    = NULL;
    wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm       = LoadIcon(NULL, IDI_WINLOGO); 
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); 
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
    wc.hInstance     = hInst; 
    if(!RegisterClassEx(&wc)){
        
        MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
        return NULL; 
    }
    
    hMainWnd = CreateWindow(
        szClassName, 
        L"Paint", 
        WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, 
        CW_USEDEFAULT,
        NULL, 
        CW_USEDEFAULT, 
        NULL, 
        (HWND)NULL, 
        NULL,
        HINSTANCE(hInst), 
        NULL); 
    if(!hMainWnd){
        
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }
	Menu(hMainWnd);
    ShowWindow(hMainWnd, nCmdShow); 
    UpdateWindow(hMainWnd); 
    while(GetMessage(&msg, NULL, NULL, NULL)){ 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
    return msg.wParam; 
}
 INT Menu(HWND hMainWnd)
 {
	 HMENU MainMenu=CreateMenu();
	 HMENU hPopupMenu=CreatePopupMenu();
	 HMENU hToolsMenu=CreatePopupMenu();
	 AppendMenu(MainMenu,MFT_STRING|MF_POPUP,(UINT)hPopupMenu,L"&Файл");
	 {
		 AppendMenu(hPopupMenu,MF_STRING,1,L"Открыть");
		 AppendMenu(hPopupMenu,MF_STRING,2,L"Сохранить");
		 AppendMenu(hPopupMenu,MF_STRING,3,L"Печать");
		 AppendMenu(hPopupMenu,MF_STRING,4,L"Выход");
	 }
	  AppendMenu(MainMenu,MFT_STRING|MF_POPUP,(UINT)hToolsMenu,L"&Инструменты");
	  {
		 AppendMenu(hToolsMenu,MF_STRING,5,L"Карандаш");
		 AppendMenu(hToolsMenu,MF_STRING,6,L"Линия");
		 AppendMenu(hToolsMenu,MF_STRING,7,L"Эллипс");
		 AppendMenu(hToolsMenu,MF_STRING,8,L"Прямоугольник");
		 AppendMenu(hToolsMenu,MF_STRING,9,L"Ломанная");
		 AppendMenu(hToolsMenu,MF_STRING,10,L"Многоугольник");
		 AppendMenu(hToolsMenu,MF_STRING,11,L"Текст");
		 AppendMenu(hToolsMenu,MF_STRING,12,L"Масштабирование");
		 AppendMenu(hToolsMenu,MF_STRING,13,L"Перемещение");
	 }
	  SetMenu(hMainWnd,MainMenu);
	  return 0;
 }
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	GetClientRect(hWnd,&rec);
 INT xscale=0,yscale=0;
    switch(uMsg){
		case WM_COMMAND:
		flag=false;
		switch(wParam){
		case 1:break;
		case 2:break;
		case 3:break;
		case 4:break;
		case 5:Tools=5;break;
		case 6:Tools=6;break;
		case 7:Tools=7;break;
		case 8:Tools=8;break;
		case 9:Tools=9;break;
		case 10:Tools=10;break;
		case 11:Tools=11;break;
		case 12:Tools=12;break;
		case 13:Tools=13;break;
		};
			break;
	case WM_PAINT:
		hDC=BeginPaint(hWnd,&ps);
		GetClientRect(hWnd,&rec);
		BitBlt(hDC,0,0,rec.right,rec.bottom,hdcMem,0,0,SRCCOPY);


		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
				hDC=GetDC(hWnd);
				GetClientRect(hWnd,&rec);
				hdcMem=CreateCompatibleDC(hDC);
				hbmMem=CreateCompatibleBitmap(hDC,rec.right,rec.bottom);
				hOld=SelectObject(hdcMem,hbmMem);
				BitBlt(hdcMem,0,0,rec.right,rec.bottom,hDC,0,0,SRCCOPY);
				ReleaseDC(hWnd,hDC);
				if(Tools==9)
					Poly_line=true;
				else
					Poly_line=false;
			startX=LOWORD(lParam);
			startY=HIWORD(lParam);
			flag=true;
			break;
	case WM_MOUSEMOVE:
		if(flag&&Poly_line)
				Draw(hWnd,lParam);
		if(flag)
			Draw(hWnd,lParam);
			if(Tools==12)
			{
			xscale=0;
			yscale=0;
			}
			break;
	case WM_LBUTTONUP:
		if(Poly_line)
		{
			MoveToEx(hDC,startX,startY,0);
			LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
			startX=LOWORD(lParam);
			startY=HIWORD(lParam);
		}
		else
		flag=false;
		hDC=GetDC(hWnd);		
		GetClientRect(hWnd,&rec);
		BitBlt(hdcMem,0,0,rec.right,rec.bottom,hDC,0,0,SRCCOPY);
		break;
	case WM_RBUTTONDOWN:
		hDC=GetDC(hWnd);
		GetClientRect(hWnd,&rec);
		InvalidateRect(hWnd,&rec,true);
		MoveToEx(hDC,startX,startY,0);
		LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
		flag=false;
		Poly_line=false;
	break;
	case WM_MOUSEWHEEL:
		if(Tools==12)
		if(GET_WHEEL_DELTA_WPARAM(wParam)>0)
		{
			hDC=GetDC(hWnd);
			GetClientRect(hWnd,&rec);
			xscale+=10;
			yscale+=10;
			StretchBlt(hDC, 0, 0, rec.right + xscale, rec.bottom + yscale, hdcMem,
     		0, 0, rec.right, rec.bottom, SRCCOPY);
			BitBlt(hdcMem,0,0,rec.right,rec.bottom,hDC,0,0,SRCCOPY);
			
			//BitBlt(hDC,0,0,rec.right,rec.bottom,hdcMem,0,0,SRCCOPY);
			ReleaseDC(hWnd,hDC);
		}
		else
		{
			hDC=GetDC(hWnd);
			GetClientRect(hWnd,&rec);
			xscale-=10;
			yscale-=10;
			StretchBlt(hDC, 0, 0, rec.right + xscale, rec.bottom + yscale, hdcMem,
     		0, 0, rec.right, rec.bottom, SRCCOPY);
			BitBlt(hdcMem,0,0,rec.right,rec.bottom,hDC,0,0,SRCCOPY);
			//BitBlt(hDC,0,0,rec.right,rec.bottom,hdcMem,0,0,SRCCOPY);
		}
		
		break;
    case WM_DESTROY: 
		DeleteObject(RedPen);
		ReleaseDC(hWnd,hDC);
        PostQuitMessage(NULL);
		SelectObject(hdcMem,hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam); 
    }
    return NULL; 
}

VOID Draw(HWND hWnd,LPARAM lParam)
{
	HDC hDC=GetDC(hWnd);
	GetClientRect(hWnd,&rec);
	SelectObject(hDC,RedPen);
switch(Tools)
		{
		case PEN:
			MoveToEx(hDC,startX,startY,0);
			LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
			startX=LOWORD(lParam);
			startY=HIWORD(lParam);
		break;
		case LINE:
			GetClientRect(hWnd,&rec);
			InvalidateRect(hWnd,&rec,true);
			UpdateWindow(hWnd);
			MoveToEx(hDC,startX,startY,0);
			LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
		break;
		case ELLIPSE:
			GetClientRect(hWnd,&rec);
			InvalidateRect(hWnd,&rec,true);
			UpdateWindow(hWnd);
			Ellipse(hDC,startX,startY,LOWORD(lParam),HIWORD(lParam));
		break;
		case REC:
			GetClientRect(hWnd,&rec);
			InvalidateRect(hWnd,&rec,true);
			UpdateWindow(hWnd);
			Rectangle(hDC,startX,startY,LOWORD(lParam),HIWORD(lParam));
		break;
		case POLYLINE:
			GetClientRect(hWnd,&rec);
			InvalidateRect(hWnd,&rec,true);
			UpdateWindow(hWnd);
			MoveToEx(hDC,startX,startY,0);
			LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
		break;
		case TEXT:
		break;
		};
}