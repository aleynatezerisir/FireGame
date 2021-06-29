﻿#include <windows.h>  
#include "chmat.h"
#include <stdio.h>


#define IDI_ICON1 101
#define IDC_OPEN	3000
#define IDC_BUTTON1 3001
#define IDC_BUTTON2 3002
#define IDC_BUTTON3 3003

void myThreadFun();
HINSTANCE hInst;   // current instance
HWND  hWnd, hEdit, HButton1, HButton2, HButton3, Hmainbmp;    //parent window
int keypressed;

LPCTSTR lpszAppName = "GEN32";
LPCTSTR lpszTitle = "GENERIC32";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	MSG      msg;
	WNDCLASSEX wc;
	HMENU MainMenu, FileMenu;
	MainMenu = CreateMenu();
	FileMenu = CreatePopupMenu();
	AppendMenu(FileMenu, MF_STRING, IDC_OPEN, "Open");
	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FileMenu, "FILE");

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = lpszAppName;
	wc.lpszClassName = lpszAppName;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImage(hInstance, lpszAppName,
		IMAGE_ICON, 16, 16,
		LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc))
		return(FALSE);

	hInst = hInstance;
	hWnd = CreateWindowEx(0, lpszAppName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		900, 600,
		NULL,
		MainMenu,
		hInstance,
		NULL
	);


	if (!hWnd)
		return(FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(msg.wParam);
}
//   !!!!!!!!!!!!!!!!!!!Global definition makes it easier!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
CHMAT m(300, 300, CHMAT_INT);
HANDLE hTimerQueue = NULL;
int boxlocation = 10;
int Arrowloc = 102;
int counter = 0;
int direction = 0;
int ArrowlocX = 0;
//Thread called by the timer

void loc1() {
	for (int i = 0; i < 10; i++)
	{
		m(boxlocation + i, 100) = 0;
		m(boxlocation + i, 101) = 0;
		m(boxlocation + i, 102) = 0;
		m(boxlocation + i, 103) = 0;
		m(boxlocation + i, 104) = 0;
		m(boxlocation + i, 105) = 0;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}

}
void threadfunction(int au)
{
	int flag = 0;
	int Arrowloc = 102;

	switch (direction)
	{
		case 1: {
			keypressed = 37;
			break;
		}
		case 2: {
			keypressed = 39;
			break;
		}
		default:
			break;
	}

	while (flag < 97) {
		int k = 0, l = 0;
		while (k < 4) {
			m(au, Arrowloc + k) = 0;
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			k++;
		}
		Arrowloc += 2;
		while (l < 4) {
			if (flag == 96) break;
			m(au, Arrowloc + l) = 0xff0000;
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			l++;
		}
		flag++;
	}

}
void keyboard() {
	switch (keypressed)
	{
	case 37: {
		direction = 1;
		boxlocation -= 2;
		break;
	}
	case 39: {

		direction = 2;
		boxlocation += 2;
		break;
	}
	case 32: {
		std::thread threadcreate{ threadfunction,boxlocation };
		if (keypressed == 32) threadcreate.join();
		break;
	}
	default: counter = 0;
	}

}
void loc2() {
	for (int i = 0; i < 10; i++)
	{
		m(boxlocation + i, 100) = 0xff0000;
		m(boxlocation + i, 101) = 0xff0000;
		m(boxlocation + i, 102) = 0xff0000;
		m(boxlocation + i, 103) = 0xff0000;
		m(boxlocation + i, 104) = 0xff0000;
		m(boxlocation + i, 105) = 0xff0000;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}

}
void fire_silme() {
	for (int i = 0; i < 4; i++)
	{
		if (counter == 0) break;
		if (Arrowloc > 299)
		{
			Arrowloc = 102;
			counter = 0;
			break;
		}
		m(ArrowlocX, Arrowloc + i) = 0;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}
	if (counter == 1) Arrowloc += 3;
}
void fire_yazma() {
	for (int i = 0; i < 4; i++)
	{
		if (counter == 0) break;
		if (Arrowloc > 299)
		{
			Arrowloc = 102;
			counter = 0;
			break;
		}
		m(ArrowlocX, Arrowloc + i) = 0xffffff;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}
}
VOID CALLBACK SlidingBox(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	loc1();
	keyboard();
	loc2();
	fire_silme();
	fire_yazma();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_CREATE:
	{

		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "", WS_CHILD | WS_VISIBLE |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 735, 5, 150, 100, hWnd, NULL, hInst, NULL);
		HButton1 = CreateWindowEx(NULL, "BUTTON", "START", WS_CHILD | WS_VISIBLE | SS_CENTER,
			2, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		HButton2 = CreateWindowEx(NULL, "BUTTON", "BUTTON2", WS_CHILD | WS_VISIBLE | SS_CENTER,
			82, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON2, hInst, NULL);
		HButton3 = CreateWindowEx(NULL, "BUTTON", "STOP", WS_CHILD | WS_VISIBLE | SS_CENTER,
			162, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON3, hInst, NULL);
		Hmainbmp = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 23, 600, 500, hWnd, NULL, hInst, NULL);

	}
	break;
	case WM_KEYDOWN:
	{
		keypressed = (int)wParam;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_OPEN:
		{
			OPENFILENAME ofn;

			char szFile[260];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
		}
		break;
		case IDC_BUTTON1:
		{
			//Starting Timer
			HANDLE hTimer = NULL;

			hTimerQueue = CreateTimerQueue();
			if (NULL == hTimerQueue)
			{
				break;
			}
			//starts 1000 ms later calls thread every 30 ms
			CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)SlidingBox, NULL, 1000, 30, 0);
		}
		break;
		case IDC_BUTTON2:
		{

			m = 0xff00;
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			SetWindowText(hEdit, "Green matrix created!");

		}
		break;
		case IDC_BUTTON3:
		{
			//deletes timer
			DeleteTimerQueue(hTimerQueue);
		}
		break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	return(0L);
}

