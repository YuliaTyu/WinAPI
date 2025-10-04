#include<Windows.h>
#include"resource.h"
//имя окна 
CONST CHAR g_sz_WND_CLASS_NAME[] = "My Window Class";

//процедура галвного обычного окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;//структура описывающая класс окна
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

	//Создание окна
	HWND hwnd = CreateWindowExA
	(
		NULL,
		g_sz_WND_CLASS_NAME,
		g_sz_WND_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,//позиция окна
		CW_USEDEFAULT,CW_USEDEFAULT,//размер окна
		NULL,//родиельское окно
		NULL,
		hInstance,
		NULL

	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//Запуск цикла сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

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
			"Этот статик текс создан при помощи функции CreateWindowEx ",
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
			"Применить",
			WS_CHILD|WS_VISIBLE,//WS_CHILD = создаваемое окно явл дочерним элементом какого-то другого окна
			430,90,
			80,32,
			hwnd,
			(HMENU)1003,
			GetModuleHandle(NULL),
			NULL
		);
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1003:
		{
			MessageBox(hwnd, "Привет", "Привет", MB_OK | MB_ICONINFORMATION);
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
		//MessageBox(NULL, " Или не закрой", "финита ля комедия", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);//прерывает цикл сообщений

		break;
	case WM_CLOSE:
		if(MessageBox(hwnd,"Вы дейстительно хотите закрыть окно?","Серьезно?",MB_YESNO | MB_ICONQUESTION)== IDYES)
			SendMessage(hwnd, WM_DESTROY,0,0),
		DestroyWindow(hwnd);// оправляет сообщение - уничтожить окно
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}