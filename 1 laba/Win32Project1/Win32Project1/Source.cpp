
#include <windows.h>
#include <tchar.h>
#include <CommDlg.h>
#include <string.h>
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
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int Menu(HWND);
HINSTANCE hInst;
BOOL flag=false,Poly_line=false,Zooming=false,fText=false,keyflag=false;
BOOL SaveFile(HWND);
BOOL OpenFile(HWND);
BOOL PrintFile(HWND);
HPEN PenColor=CreatePen(PS_SOLID,1,RGB(0,0,0));
DWORD color=RGB(0,0,0);
HBRUSH hBrush=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
int width=1;
INT startX,endX,startY,endY,Tools;
PAINTSTRUCT ps;
RECT rec;
HDC hDC;
HDC hdcMem;
HBITMAP hbmMem;
HANDLE hOld;
HPEN hPen;
char *text;
int i = 0;	
int xscale_last=0;
int yscale_last=0;
int xscale,yscale;
int spacex=0;
int spacey=0;
int xlast=0;
int ylast=0;

void Draw(HWND,LPARAM);
void DrawPen(HWND,INT,INT,LPARAM,HPEN);

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
	 HMENU hColorMenu=CreatePopupMenu();
	 HMENU hFillColorMenu=CreatePopupMenu();
	 HMENU hWidthMenu=CreatePopupMenu();
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
	  	  AppendMenu(MainMenu,MFT_STRING|MF_POPUP,(UINT)hColorMenu,L"&Цвет");
		  {
			  AppendMenu(hColorMenu,MF_STRING,14,L"Красный");
			  AppendMenu(hColorMenu,MF_STRING,15,L"Зелёный");
			  AppendMenu(hColorMenu,MF_STRING,16,L"Синий");
			  AppendMenu(hColorMenu,MF_STRING,17,L"Чёрный");
		  }
		  AppendMenu(MainMenu,MFT_STRING|MF_POPUP,(UINT)hFillColorMenu,L"&Заливка");
		  {
			 
			AppendMenu(hFillColorMenu,MF_STRING,18,L"Красный");
			AppendMenu(hFillColorMenu,MF_STRING,19,L"Зелёный");
			AppendMenu(hFillColorMenu,MF_STRING,20,L"Синий");
			AppendMenu(hFillColorMenu,MF_STRING,21,L"Чёрный");
			AppendMenu(hFillColorMenu,MF_STRING,22,L"Прозрачный");
		  }
		   AppendMenu(MainMenu,MFT_STRING|MF_POPUP,(UINT)hWidthMenu,L"&Толщина");
		   {
			   AppendMenu(hWidthMenu,MF_STRING,23,L"1");
			   AppendMenu(hWidthMenu,MF_STRING,24,L"2");
			   AppendMenu(hWidthMenu,MF_STRING,25,L"3");
			   AppendMenu(hWidthMenu,MF_STRING,26,L"4");
		   }
	  SetMenu(hMainWnd,MainMenu);
	  return 0;
 }
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	GetClientRect(hWnd,&rec);
    switch(uMsg){
		case WM_COMMAND:
		flag=false;
		switch(wParam){
		case OPEN:OpenFile(hWnd);break;
		case SAVE:SaveFile(hWnd);break;
		case PRINT:PrintFile(hWnd);break;
		case EXIT:DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		case PEN:Tools=5;break;
		case LINE:Tools=6;break;
		case ELLIPSE:Tools=7;break;
		case REC:Tools=8;break;
		case POLYLINE:Tools=9;break;
		case POLYGON:Tools=10;break;
		case TEXT:Tools=11;fText=true;i=0;break;
		case ZOOM:Tools=12;xscale=xscale_last;yscale=yscale_last;break;
		case PAN:Tools=13;spacex=xlast;spacey=ylast;break;
		case RED:color=RGB(255,0,0);hPen=CreatePen(PS_SOLID,width,color);break;
		case GREEN:color=RGB(0,255,0);hPen=CreatePen(PS_SOLID,width,color);break;
		case BLUE:color=RGB(0,0,255);hPen=CreatePen(PS_SOLID,width,color);break;
		case BLACK:color=RGB(0,0,0);hPen=CreatePen(PS_SOLID,width,color);break;
		case FILLRED:hBrush=CreateSolidBrush(RGB(255,0,0));break;
		case FILLGREEN:hBrush=CreateSolidBrush(RGB(0,255,0));break;
		case FILLBLUE:hBrush=CreateSolidBrush(RGB(0,0,255));break;
		case FILLBLACK:hBrush=CreateSolidBrush(RGB(0,0,0));break;
		case FILLNONE:hBrush=(HBRUSH)GetStockObject(HOLLOW_BRUSH);break;
		case WIDTH1:width=1;hPen=CreatePen(PS_SOLID,width,color);break;
		case WIDTH2:width=2;hPen=CreatePen(PS_SOLID,width,color);break;
		case WIDTH3:width=3;hPen=CreatePen(PS_SOLID,width,color);break;
		case WIDTH4:width=4;hPen=CreatePen(PS_SOLID,width,color);break;
		};
			break;
	case WM_PAINT:
		hDC=BeginPaint(hWnd,&ps);
		GetClientRect(hWnd,&rec);
		BitBlt(hDC,spacex,spacey,rec.right,rec.bottom,hdcMem,0,0,SRCCOPY);
		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		spacex=0;
		spacey=0;
		hDC=GetDC(hWnd);
		GetClientRect(hWnd,&rec);
		hdcMem=CreateCompatibleDC(hDC);
		hbmMem=CreateCompatibleBitmap(hDC,rec.right,rec.bottom);
		hOld=SelectObject(hdcMem,hbmMem);
		BitBlt(hdcMem,0,0,rec.right,rec.bottom,hDC,0,0,SRCCOPY);
		ReleaseDC(hWnd,hDC);
		if(Tools==11)
			{
				fText=true;
				i=0;
				text=(char *)calloc(256,sizeof(char));
			}
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
			break;
	case WM_LBUTTONUP:
		if(Poly_line)
		{
			SelectObject(hDC,hPen);
			SelectObject(hDC,hBrush);
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
		SelectObject(hDC,hPen);
		SelectObject(hDC,hBrush);
		GetClientRect(hWnd,&rec);
		InvalidateRect(hWnd,&rec,true);
		MoveToEx(hDC,startX,startY,0);
		LineTo(hDC,LOWORD(lParam),HIWORD(lParam));
		flag=false;
		Poly_line=false;
	break;
	case WM_CHAR:
		if(Tools==11)
		{
			hDC=GetDC(hWnd);
			char c=(char)wParam;
			if(c==VK_RBUTTON)
			{
				fText=false;
				text[i]='\0';
			}
			else
				text[i++]=c;
			TextOutA(hDC,startX,startY,(LPSTR)text,strlen((char *)text));
		}
	break;
	case WM_KEYDOWN:
			switch(wParam){
			case 83:
				if(!fText)
					SaveFile(hWnd);
				break;
			}
	break;
	case WM_KEYUP:
		keyflag=false;
	break;
	case WM_MOUSEWHEEL:
		if(GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL && Tools==12)
		{
		if(GET_WHEEL_DELTA_WPARAM(wParam)>0)
		{
			hDC=GetDC(hWnd);
			GetClientRect(hWnd,&rec);
			xscale+=10;
			yscale+=10;
			StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
     		0, 0, rec.right, rec.bottom, SRCCOPY);
			ReleaseDC(hWnd,hDC);
		}
		else
		{
			hDC=GetDC(hWnd);
			GetClientRect(hWnd,&rec);
			xscale-=10;
			yscale-=10;
			StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
     		0, 0, rec.right, rec.bottom, SRCCOPY);
			ReleaseDC(hWnd,hDC);
		}
		xscale_last=xscale;
		yscale_last=yscale;
		}
					if(GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
					{
					if(GET_WHEEL_DELTA_WPARAM(wParam)>0 && Tools==13)
					{
						hDC=GetDC(hWnd);
						GetClientRect(hWnd,&rec);
						InvalidateRect(hWnd,&rec,true);
						UpdateWindow(hWnd);
						spacex+=3;
						StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
						0, 0, rec.right, rec.bottom, SRCCOPY);
						ReleaseDC(hWnd,hDC);
						xlast=spacex;
						ylast=spacey;
					}
					else
						if(GET_WHEEL_DELTA_WPARAM(wParam)<0 && Tools==13)
						{
							hDC=GetDC(hWnd);
							GetClientRect(hWnd,&rec);
							InvalidateRect(hWnd,&rec,true);
							UpdateWindow(hWnd);
							spacex-=3;
							StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
							0, 0, rec.right, rec.bottom, SRCCOPY);
							ReleaseDC(hWnd,hDC);
							xlast=spacex;
							ylast=spacey;
						}
					}
					else
			if(GET_WHEEL_DELTA_WPARAM(wParam)>0 && Tools==13)
			{
					hDC=GetDC(hWnd);
					GetClientRect(hWnd,&rec);
					InvalidateRect(hWnd,&rec,true);
					UpdateWindow(hWnd);
					spacey-=3;
					StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
					ReleaseDC(hWnd,hDC);
					xlast=spacex;
					ylast=spacey;
			}
			else
				if(GET_WHEEL_DELTA_WPARAM(wParam)<0 && Tools==13)
				{
					hDC=GetDC(hWnd);
					GetClientRect(hWnd,&rec);
					InvalidateRect(hWnd,&rec,true);
					UpdateWindow(hWnd);
					spacey+=3;
					StretchBlt(hDC, spacex, spacey, rec.right + xscale, rec.bottom + yscale, hdcMem,
					0, 0, rec.right, rec.bottom, SRCCOPY);
					ReleaseDC(hWnd,hDC);
					xlast=spacex;
					ylast=spacey;
				}
	break;
    case WM_DESTROY: 
		DeleteObject(hPen);
		DeleteObject(hBrush);
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
	HDC hDC=GetDC(hWnd);
	TCHAR szFilters[] = _T("Scribble Files (*.emf)\0*.emf\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = { 0 };

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
	PlayEnhMetaFile(hDC, hemf, &rec);
	DeleteEnhMetaFile(hemf);
	InvalidateRect(hWnd, &rec, false);
	return TRUE;
}

BOOL PrintFile(HWND hWnd)
{
	PRINTDLG pd;
	hDC=GetDC(hWnd);
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

VOID Draw(HWND hWnd,LPARAM lParam)
{
	HDC hDC=GetDC(hWnd);
	GetClientRect(hWnd,&rec);
	SelectObject(hDC,hPen);
	SelectObject(hDC,hBrush);
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
		case POLYGON:
			GetClientRect(hWnd,&rec);
			InvalidateRect(hWnd,&rec,true);
			UpdateWindow(hWnd);

			MoveToEx(hDC,startX,startY,0);
			LineTo(hDC,LOWORD(lParam),HIWORD(lParam));

			MoveToEx(hDC,LOWORD(lParam),HIWORD(lParam),0);
			LineTo(hDC,LOWORD(lParam)-(LOWORD(lParam)-startX)*2,HIWORD(lParam));

			MoveToEx(hDC,LOWORD(lParam)-(LOWORD(lParam)-startX)*2,HIWORD(lParam),0);
			LineTo(hDC,startX,startY);

			break;
		};
DeleteObject(hPen);
DeleteObject(hBrush);
}