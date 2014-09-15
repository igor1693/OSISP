#include <windows.h> 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int Menu(HWND);
BOOL flag=false,flagPen=false,flagLine=false,flagEllipse=false,flagRec=false,flagPoly=false,flagText=false;
HPEN RedPen=CreatePen(PS_SOLID,1,RGB(255,0,0));
HPEN WhitePen=CreatePen(PS_SOLID,1,RGB(255,255,255));
INT xPos,yPos;
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
        L"1 Laba Osisp Tolstoy", 
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
    HDC hDC=GetDC(hWnd);
    switch(uMsg){
	case WM_COMMAND:
		if(wParam==5)
		{
			flagPen=true;
			flagLine=false;
			flagEllipse=false;
			flagRec=false;
			flagPoly=false;
			flagText=false;
		}
		else
			if(wParam==6)
			{
				flagLine=true;
				flagPen=false;
				flagEllipse=false;
				flagRec=false;
				flagPoly=false;
				flagText=false;
			}
			else
				if(wParam==7)
				{
					flagLine=false;
					flagPen=false;
					flagEllipse=true;
					flagRec=false;
					flagPoly=false;
					flagText=false;
				}
				else
					if(wParam==8)
					{
						flagLine=false;
						flagPen=false;
						flagEllipse=false;
						flagRec=true;
						flagPoly=false;
						flagText=false;
					}
					else
						if(wParam==9)
						{
							flagLine=false;
							flagPen=false;
							flagEllipse=false;
							flagRec=false;
							flagPoly=true;
							flagText=false;
						}
						else
							if(wParam==11)
							{
								flagLine=false;
								flagPen=false;
								flagEllipse=false;
								flagRec=false;
								flagPoly=false;
								flagText=true;
							}
			break;
	case WM_LBUTTONDOWN:
			if(flagPoly && (xPos!=0 && yPos!=0))
			{
				SelectObject(hDC,RedPen);
				MoveToEx(hDC,xPos,yPos,0);
				LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
			};
			xPos=LOWORD(lParam);
			yPos=HIWORD(lParam);
			flag=true;
			break;
	case WM_MOUSEMOVE:
		if (flag && flagPen)
		{
			SelectObject(hDC,RedPen);
			MoveToEx(hDC,xPos,yPos,0);
			LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
			xPos=LOWORD(lParam);
			yPos=HIWORD(lParam);
		}
		else
			if(flag && flagLine)
			{
				SelectObject(hDC,RedPen);
				MoveToEx(hDC,xPos,yPos,0);
				LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
				Sleep(50);
				SelectObject(hDC,WhitePen);
				MoveToEx(hDC,xPos,yPos,0);
				LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
			}
			else
				if(flag && flagEllipse)
				{
					SelectObject(hDC,RedPen);
					Ellipse(hDC,xPos,yPos,LOWORD(lParam),HIWORD(lParam));
					Sleep(50);
					SelectObject(hDC,WhitePen);
					Ellipse(hDC,xPos,yPos,LOWORD(lParam),HIWORD(lParam));
				}
				else
					if(flag && flagRec)
					{
						SelectObject(hDC,RedPen);
						Rectangle(hDC,xPos,yPos,LOWORD(lParam),HIWORD(lParam));
						Sleep(50);
						SelectObject(hDC,WhitePen);
						Rectangle(hDC,xPos,yPos,LOWORD(lParam),HIWORD(lParam));
					}
					else
						if(flag && flagPoly)
						{
							SelectObject(hDC,RedPen);
							MoveToEx(hDC,xPos,yPos,0);
							LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
							Sleep(50);
							SelectObject(hDC,WhitePen);
							MoveToEx(hDC,xPos,yPos,0);
							LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
						}	
			break;
	case WM_LBUTTONUP:
		if(flagLine)
		{
			SelectObject(hDC,RedPen);
			MoveToEx(hDC,xPos,yPos,0);
			LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
		}
		else
			if(flagEllipse)
			{
				SelectObject(hDC,RedPen);
				Ellipse(hDC,xPos,yPos,LOWORD(lParam),HIWORD(lParam));
			}
			else
				if(flagRec)
				{
					SelectObject(hDC,RedPen);
					Rectangle(hDC,xPos,yPos,LOWORD(lParam),HIWORD(lParam));
				}
				else
					if(flagPoly)
					{
						SelectObject(hDC,RedPen);
						MoveToEx(hDC,xPos,yPos,0);
						LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
						xPos=LOWORD(lParam);
						yPos=HIWORD(lParam);
						break;
					}
		flag=false;
		break;
    case WM_DESTROY: 
		DeleteObject(RedPen);
		DeleteObject(WhitePen);
		ReleaseDC(hWnd,hDC);
        PostQuitMessage(NULL); 
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam); 
    }
    return NULL; 
}