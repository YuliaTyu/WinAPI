#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR* VALUES[] = { "This", "is", "my", "First", "Combo", "Box" };//для записи в комбобокс создаем массив
//процедура окна:
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC) DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
		for (int i = 0; i < sizeof(VALUES) / sizeof(VALUES[0]); i++)
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)VALUES[i]);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			CONST INT SIZE = 256;     //буффер для сохранения текста
			CHAR sz_buffer[SIZE] = {};
			INT i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);//для того чтобы узнать номер выбранного элемента
			SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_buffer);//для того чтобы взять текст
			CHAR sz_message[SIZE];//для вывода номера и текста объявляем еще один буффер + заг файл <cstdio>
			sprintf(sz_message, "Вы выбрали вариант № со значением ", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);//проверка
		}
		break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}