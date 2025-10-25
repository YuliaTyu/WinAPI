#define _CRT_SECURE_NO_WARNINGS 
#include<Windows.h>
#include<float.h>
#include<stdio.h>
#include"resource.h"



CONST CHAR g_sz_CLASS_NAME[] = "Calc_SPU_411";

CONST INT g_i_BUTTON_SIZE = 80;//размер кнопки
CONST INT g_i_INTERVAL = 2;//размер интервала между кнопками
CONST INT g_i_BUTTIN_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;//двойная кнопка с интервалом

CONST INT g_i_START_X = 10;//начальные координаты
CONST INT g_i_START_Y = 10;
CONST INT g_i_SCREEN_WIDTH = (g_i_BUTTON_SIZE + g_i_INTERVAL) * 5 - g_i_INTERVAL;//коичесвто колонок
CONST INT g_i_SCREEN_HEIGHT = g_i_BUTTON_SIZE;

CONST INT g_i_BUTTON_START_X = g_i_START_X; //выравнивание кнопок 
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_SCREEN_HEIGHT + g_i_INTERVAL * 4;

//массив с кнопками
CONST CHAR g_OPERATIONS[] = "+-*/";
CONST INT g_i_OPERATIONS_START_X = g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3;//начало коррдинат операционных +-*/кнопок

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetSkin(HWND hwnd, CONST CHAR SZ_SKIN[]);

//shift - количество кнопок
#define BUTTON_SHIFT_X(shift) g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL)*shift
#define BUTTON_SHIFT_Y(shift) g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*(shift)

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1 Регистрация класса окна
	WNDCLASSEX wClass;//структура описывающая класс окна
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));//добавление иконки
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_BUTTON_0));//добавление иконки
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = WndProc;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;//зайти в свойства и поменять язык

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class reg failed", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	//2 Создание окна
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,//WIN 32 исключаем изменение размера окна
		CW_USEDEFAULT, CW_USEDEFAULT,//позиция окна
		g_i_SCREEN_WIDTH + 2 * g_i_START_X + 16,//размер окна //выравнивание по ширине окна до пискеля +16!!!
		g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 42,
		NULL,
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

	//3 звапуск цикла сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DOUBLE a = DBL_MIN;
	static DOUBLE b = DBL_MIN;
	static INT operation = 0;
	static BOOL	input = FALSE;
	static BOOL operation_input = FALSE;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx
		(
			NULL,
			"Edit",
			"0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			10, 10,
			g_i_SCREEN_WIDTH, g_i_SCREEN_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		//создание кнопок от 1 до 9
		INT digit = 1;
		CHAR sz_digit[2] = "0";

		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = digit + '0';
				CreateWindowEx
				(
					NULL, "BUTTON", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * i / 3,
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0 + digit),
					GetModuleHandle(NULL),
					NULL
				);
				digit++;
			}
		}
		//создание 0
		HWND hButtoh0 = CreateWindowEx
		(
			NULL, "BUTTON", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_BUTTON_START_X, //g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			BUTTON_SHIFT_Y(3),
			g_i_BUTTIN_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		/*HBITMAP hImage0 = (HBITMAP)LoadImage
		(
			GetDriverModuleHandle(NULL),
			"ButtonBMP\\square_blue\\button_o.bmp",
			IMAGE_BITMAP,
			g_i_BUTTIN_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
			);
		SendMessage(hButtoh0, BM_SETIMAGE, 0, (LPARAM)hImage0);*/
			
		//создание точки
		CreateWindowEx
		(
			NULL, "BUTTON", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			//g_i_BUTTON_START_X + g_i_BUTTIN_DOUBLE_SIZE + g_i_INTERVAL,
			//g_i_BUTTON_START_Y +(g_i_BUTTON_SIZE+ g_i_INTERVAL)*3,
			BUTTON_SHIFT_X(2), BUTTON_SHIFT_Y(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		//создание кнопок операторов
		CHAR operation[2] = "";
		for (int i = 0; i < 4; i++)
		{
			operation[0] = g_OPERATIONS[i];
			CreateWindowEx
			(
				NULL, "BUTTON", operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|BS_BITMAP,
				BUTTON_SHIFT_X(3),
				BUTTON_SHIFT_Y(3 - i),
				//g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL, "BUTTON", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		//C
		CreateWindowEx
		(
			NULL, "BUTTON", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL),
			NULL
		);
		//=
		CreateWindowEx
		(
			NULL, "BUTTON", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(2),
			g_i_BUTTON_SIZE, g_i_BUTTIN_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);

		SetSkin(hwnd, "metal_mistral");//вызов кнопок из файла
	}

	break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE] = {};
		CHAR sz_digit[2] = {};
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_display);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)//показывает цифры в экране
		{
			if (operation_input)sz_display[0] = 0;
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + 48; //ASCII 0 - 48 ПОЭТОМУ ДЛЯ ПЕРЕХОДА А ДРУГУЮ ЦИФРУ +48
			if (strcmp(sz_display, "0"))
				strcat(sz_display, sz_digit);
			else
				strcpy(sz_display, sz_digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			operation_input = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (strchr(sz_display, '.'))break;
			strcat(sz_display, ".");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			if (strlen(sz_display) == 1)
				sz_display[0] = '0';
			else
				sz_display[strlen(sz_display) - 1] = 0;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = DBL_MIN;
			operation = 0;
			input = operation_input = FALSE;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
		}
		//обработка операций
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (input)
				(a == DBL_MIN ? a : b) = atof(sz_display);
			if (input)
				SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			input = FALSE;
			operation = LOWORD(wParam);
			operation_input = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input)b = atof(sz_display);
			input = FALSE;
			switch (operation)
			{
			case IDC_BUTTON_PLUS:	a += b;	break;
			case IDC_BUTTON_MINUS:	a -= b;	break;
			case IDC_BUTTON_ASTER:	a *= b;	break;
			case IDC_BUTTON_SLASH:	a /= b;	break;
			}
			operation_input = FALSE;
			sprintf(sz_display, "%g", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		
	}
	break;


	case WM_KEYDOWN:
	{
		if (GetKeyState(VK_SHIFT) < 0 && wParam == 0x38)// '*'она же 8 - В контексте ASCII 0x38 — это код символа «8»
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam >= 0x60 && wParam <= 0x69)
		{
			SendMessage(GetDlgItem(hwnd, wParam - 0x60 + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		}
		switch (wParam)
		{
		case VK_OEM_PLUS:
		case VK_ADD:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0);break;
		case VK_OEM_MINUS:
		case VK_SUBTRACT:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, TRUE, 0);break;
		case VK_MULTIPLY:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);break;
		case VK_OEM_2:
		case VK_DIVIDE:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, TRUE, 0);break;

		case VK_OEM_PERIOD:
		case VK_DECIMAL:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, TRUE, 0);break;
		case VK_BACK:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, TRUE, 0);break;
		case VK_ESCAPE:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, TRUE, 0);break;
		case VK_RETURN:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0);break;
		}
	}
	break;
	case WM_KEYUP:
	{
		if (GetKeyState(VK_SHIFT) < 0 && wParam == 0x38)
		{
			SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - '0' + IDC_BUTTON_0), 0);
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
		}
		else if (wParam >= 0x60 && wParam <= 0x69)
			//что числа от 60 до 69 в контексте нумерологии могут означать этапы перехода сознания 
			// от примитивного, почти животного уровня, 
			// до состояния готовности к духовному восхождению.!!!!!!))))) 

		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x60 + IDC_BUTTON_0), 0);
			SendMessage(GetDlgItem(hwnd, wParam - 0x60 + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
		}
		switch (wParam)
		{
		case VK_OEM_PLUS:
		case VK_ADD:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			break;
		case VK_OEM_MINUS:
		case VK_SUBTRACT:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			break;
		case VK_MULTIPLY:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			break;
		case VK_OEM_2:
		case VK_DIVIDE:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			break;

		case VK_OEM_PERIOD:
		case VK_DECIMAL:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			break;
		case VK_BACK:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			break;
		case VK_ESCAPE:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
			break;
		case VK_RETURN:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			break;
		}
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
VOID SetSkin(HWND hwnd, CONST CHAR SZ_SKIN[])
{
	for (int i = 0; i < 10; i++)
	{
		CHAR sz_filename[FILENAME_MAX] = {};
		sprintf(sz_filename, "ButtonsBMP\\%s\\button_%i.bmp",SZ_SKIN,i);
		HBITMAP hBitmap = NULL;
		hBitmap = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_filename,
			IMAGE_BITMAP,
			i > 0 ? g_i_BUTTON_SIZE : g_i_BUTTIN_DOUBLE_SIZE,
			g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_0 + i), BM_SETIMAGE, 0, (LPARAM)hBitmap);
	}
}