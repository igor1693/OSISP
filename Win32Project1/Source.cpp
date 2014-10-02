#include <windows.h> 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

VOID DrawLine(HDC hDC,INT start_x,INT end_x,INT start_y,INT end_y)
{
				MoveToEx(hDC,start_x,start_y,0);
				LineTo(hDC,end_x,end_y);	
}

VOID DrawPen(HDC hDC,HWND hWnd,INT start_x,INT start_y,LPARAM lParam,HPEN ColorPen)
{
	hDC=GetDC(hWnd);
	SelectObject(hDC,ColorPen);
	MoveToEx(hDC,start_x,start_y,0);
	LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
	ReleaseDC(hWnd,hDC);
}

VOID ChooseTool()
{

}

int Menu(HWND);
BOOL flag=false,Tools,flagPen=false,flagLine=false,flagEllipse=false,flagRec=false,flagPoly=false,flagText=false;
HPEN RedPen=CreatePen(PS_SOLID,1,RGB(255,0,0));
INT startX,endX,startY,endY;
PAINTSTRUCT ps;
RECT rec;
HDC hDC;
HDC hdcMem;
HBITMAP hbmMem;
HANDLE hOld;
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
	 }
	  SetMenu(hMainWnd,MainMenu);
	  return 0;
 }
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	GetClientRect(hWnd,&rec);
    switch(uMsg){
		case WM_COMMAND:
		flag=false;
		flagPen=false;
		flagLine=false;
		flagEllipse=false;
		flagRec=false;
		flagPoly=false;
		flagText=false;
		switch(wParam){
		case 1:break;
		case 2:break;
		case 3:break;
		case 4:break;
		case 5:flagPen=true;break;
		case 6:flagLine=true;break;
		case 7:flagEllipse=true;break;
		case 8:flagRec=true;break;
		case 9:flagPoly=true;break;
		case 10:break;
		case 11:break;
		};
			break;
	case WM_PAINT:
		hDC=BeginPaint(hWnd,&ps);
		hdcMem=CreateCompatibleDC(GetDC(hWnd));
		hbmMem=CreateCompatibleBitmap(hDC,rec.right,rec.bottom);
		hOld=SelectObject(hdcMem,hbmMem);
		SelectObject(hdcMem,RedPen);
		BitBlt(hdcMem,0,0,rec.right,rec.bottom,hDC,0,0,SRCCOPY);
		Ellipse(hdcMem,400,300,500,400);
		BitBlt(hDC,0,0,rec.right,rec.bottom,hdcMem,0,0,SRCCOPY);
		SelectObject(hdcMem,hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);
		ValidateRect(hWnd,NULL);
		EndPaint(hWnd,&ps);
		break;
    case WM_DESTROY: 
		DeleteObject(RedPen);
		ReleaseDC(hWnd,hDC);
        PostQuitMessage(NULL); 
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam); 
    }
    return NULL; 
}