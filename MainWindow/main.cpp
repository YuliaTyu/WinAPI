#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<stdio.h>
//��� ���� 
CONST CHAR g_sz_WND_CLASS_NAME[] = "My Window Class";

//��������� �������� �������� ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1 ��������� ����������� ����� ����
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	//wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = (HICON) LoadImage(hInstance, "Palm0.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIcon = (HICON) LoadImage(hInstance, "PalmIsland.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE (IDC_CURSOR1));
	wClass.hCursor = (HCURSOR) LoadImage(hInstance, "cat-noir-miraculous\\Cat Noir Busy.ani", IMAGE_CURSOR, LR_DEFAULTSIZE,LR_DEFAULTSIZE,LR_LOADFROMFILE );
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WND_CLASS_NAME;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//�������� ����
	// ������� GetSystemMetrics (winuser.h) https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
	//��������� ������� ��� �������� ������������, ������� ���������� ��������. 
	//���� �������� ����� ��������� ���� �� ��������� ��������. 
	// �������� ��������, ��� ��� �������� SM_CX* � ��� ������, � ��� �������� SM_CY* � ������.
	// ����� �������� ��������, ��� ��� ���������, ��������������� ��� �������� ���������� ������, 
	// ������������ ������ ��� ����� ��������� ��������, � ���� � ��� ������� ��������
	INT screen_width = GetSystemMetrics(SM_CXSCREEN);//������
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);//������
	
	INT window_width = screen_width * .75;
	INT window_height = screen_height * 2/ 3;
	INT window_start_x = screen_width / 80;//���������� 
	INT window_start_y = screen_height / 50;//����������
	HWND hwnd = CreateWindowEx
	(
		//12 ���������� ��� �������� ��� �������� ���� 
		NULL,	
		g_sz_WND_CLASS_NAME,	
		g_sz_WND_CLASS_NAME,	
		WS_OVERLAPPEDWINDOW,
        window_start_x, window_start_y,	
        window_width, window_height,
		NULL,
		NULL,	
        hInstance,	
        NULL
);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);	//������ ����� ����������� ����: ���������� �� ���� �����, �������� � ����, �������� �� ������ �����
	UpdateWindow(hwnd);	//������������� ������� ������� ����

	//������ ����� ���������
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	HWND_TOPMOST;
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hStatic = CreateWindowEx
		(
			NULL,
			"Static",
			"���� ������ ���� ������ ��� ������ ������� CreateWindowEx ",
			WS_CHILD|WS_VISIBLE,
			10, 10,
			500, 25,
			hwnd,
			(HMENU)1000,
			GetModuleHandle(NULL),
			NULL
		);
		HWND hEdit = CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD|WS_VISIBLE | WS_BORDER,
			10,45,
			500,22,
			hwnd,
			(HMENU)1001,
			GetModuleHandle(NULL),
			NULL
		);
		HWND hEdit_1 = CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 65,
			500, 22,
			hwnd,
			(HMENU)1002,
			GetModuleHandle(NULL),
			NULL
		);
		HWND hButton = CreateWindowEx
		(
		NULL,
			"Button",
			"���������",
			WS_CHILD|WS_VISIBLE,//WS_CHILD = ����������� ���� ��� �������� ��������� ������-�� ������� ����
			430,90,
			80,32,
			hwnd,
			(HMENU)1003,
			GetModuleHandle(NULL),
			NULL
		);
	}
		break;

	case WM_MOVE: //��������� WM_MOVE
				  //���������� ����� ����������� ����.
				  //���� �������� ��� ��������� ����� ���� ������� WindowProc.
		          //https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-move 
	case WM_SIZE: //��������� WM_SIZE
				  //������������ � ���� ����� ��������� ��� �������.
				  //���� �������� ��� ��������� ����� ���� ������� WindowProc.
		          //https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-size
	{
		RECT window_rect;	//������� GetWindowRect (winuser.h) https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowrect
		GetWindowRect(hwnd, &window_rect);//????????������� DwmGetWindowAttribute (dwmapi.h)
		INT window_width = window_rect.right - window_rect.left;//������
		INT window_height = window_rect.bottom - window_rect.top;//������
		CONST INT SIZE = 256;
		CHAR sz_home[SIZE] = {};
		sprintf //������ ��������������� ������ � ������
			//https://learn.microsoft.com/ru-ru/cpp/c-runtime-library/reference/sprintf-sprintf-l-swprintf-swprintf-l-swprintf-l?view=msvc-170
		(
			sz_home,
			"�������  , ������  ",
			g_sz_WND_CLASS_NAME,
			window_rect.left, window_rect.top,
			window_width, window_height
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_home);
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1003:
		{
			MessageBox(hwnd, "������", "������", MB_OK | MB_ICONINFORMATION);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hStatic = GetDlgItem(hwnd, 1000);
			HWND hEdit = GetDlgItem(hwnd, 1001);

			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(GetDlgItem (hwnd,1002), WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
			break;
		}

		break;
	case WM_DESTROY:
		//MessageBox(NULL, " ��� �� ������", "������ �� �������", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);//��������� ���� ���������

		break;
	case WM_CLOSE:
		if(MessageBox(hwnd,"�� ������������ ������ ������� ����?","��������?",MB_YESNO | MB_ICONQUESTION)== IDYES)
			SendMessage(hwnd, WM_DESTROY,0,0),
		DestroyWindow(hwnd);// ��������� ��������� - ���������� ����
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}