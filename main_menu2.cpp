#include "config.h"

#if MENU_2
#include <windows.h>
#include "resource.h"

/*
	LPSTR  = char*     LP = long pointer
*/
const char g_szClassName[] = "myWIndowClass";
#if DIALOG_2
	HWND g_hToolBar = NULL;
#endif


/*
 Callback dialog
*/

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDOK:
				EndDialog(hwnd,IDOK);
				break;
			case IDCANCEL:
				EndDialog(hwnd,IDCANCEL);
				break;
		}
		break;
	default:
			return FALSE;
	}
	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
#if MENU_2
	case WM_CREATE:
		{
			HMENU hMenu, hSubMenu;
			HICON hIcon, hIconSm;
			
			hMenu = CreateMenu();
			//create submenu
			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu,MF_STRING,ID_STUFF_GO, "&Go");
			AppendMenu(hMenu,MF_STRING | MF_POPUP, (UINT)hSubMenu, "&stuff");

			SetMenu(hwnd,hMenu);
			
			hIcon = (HICON)LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
			if(hIcon)	//success
				SendMessage(hwnd,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
			else	//fail
				MessageBox(hwnd,"Could not load large icon","Error",MB_OK | MB_ICONERROR);

			hIconSm = (HICON)LoadImage(NULL, "menu_two.ico", IMAGE_ICON,16,16,LR_LOADFROMFILE);			
			if(hIconSm)	//success
				SendMessage(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)hIconSm);
			else
				MessageBox(hwnd,"Could not load small icon","Error", MB_OK | MB_ICONERROR);			
		}
		break;
#endif
#if DIALOG_2
	case WM_CREATE:
		g_hToolBar = CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_TOOLBAR),	//create toolbar & its loop
			hwnd,ToolDlgProc);
		if(g_hToolBar != NULL)
		{//success
			ShowWindow(g_hToolBar,SW_SHOW);	//need it if use CreateDialog(), not need for DialogBox()
		}
		else
		{//fail
			MessageBox(hwnd,"Tool bar Creation failed!","Warning!",
				MB_OK | MB_ICONINFORMATION);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case ID_FILE_EXIT:
				PostMessage(hwnd,WM_CLOSE,0,0);
				break;
			case ID_DIALOG_SHOW:
				ShowWindow(g_hToolBar,SW_SHOW);
				break;
			case ID_DIALOG_HIDE:
				ShowWindow(g_hToolBar,SW_HIDE);
				break;

		}
		break;
#else
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_HELP_ABOUT:
			{
				int ret = DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_ABOUT),hwnd,AboutDlgProc);
				if(ret == IDOK)
					MessageBox(hwnd,"Dialog exited with OK","Notice",MB_OK | MB_ICONINFORMATION);
				else if(ret == IDCANCEL)
					MessageBox(hwnd,"Dialog exited with Cancel","Notice", MB_OK | MB_ICONINFORMATION);
				else if(ret == -1)
					MessageBox(hwnd,"Dialog Failed!","Error",MB_OK | MB_ICONEXCLAMATION);					
			}
			break;
		case ID_FILE_EXIT:
			PostMessage(hwnd,WM_CLOSE,0,0);
			break;
		case ID_STUFF_GO:
			MessageBox(hwnd,"I go do something.","Do it",MB_OK);
			break;

		}
		break;
#endif

	case WM_LBUTTONDOWN:
		{
			char szFileName[MAX_PATH];
			HINSTANCE hInstance = GetModuleHandle(NULL);		//return a handle to the file used to
																//create a calling process
			GetModuleFileName(hInstance, szFileName, MAX_PATH);		//a pointer to a buffer that recives the path and file name
																	// of the specidied module
			MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);			 
			 break;
		}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
#if DIALOG_2
		DestroyWindow(g_hToolBar);
#endif
		PostQuitMessage(0);	
		break;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);		//most likely does nothing
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	//wc.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
#if MENU_1
	wc.hIcon			= LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_MYICON));			//icon on taskbar	
	wc.hIconSm			= (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);	//icon on top left
	wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MYMENU);	
#endif
#if MENU_2
	wc.hIcon			= NULL;
	wc.hIconSm			= NULL;
	wc.lpszMenuName		= NULL;
#endif
#if DIALOG_1
	wc.hIcon			= NULL;
	wc.hIconSm			= NULL;
	wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MYMENU);
#endif
#if DIALOG_2
	wc.hIcon			= NULL;
	wc.hIconSm			= NULL;
	wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MYMENU);
#endif
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);		
	wc.lpszClassName	= g_szClassName;
	

	//register class
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL,"Window Registration failed!", "Error !",
			MB_ICONEXCLAMATION | MB_OK);
	}

	//create window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"The title on my window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240,120,
		NULL,NULL,hInstance,NULL);

	//check if creating window fail
	if(hwnd == NULL)
	{
		MessageBox(hwnd,"WIndow creattion failed!","Error !",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	
	while(GetMessage(&msg,NULL,0,0) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    
    return 0;
}
#endif	//DIALOG_1