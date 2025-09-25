#include<Windows.h>
#include"resource.h"//у каждого окна есть процедура окна(это обычная функция, кот неявно вызывается при запуске окна)

//любая процедура окна всегда принимает 4 параметра :
//1 окно
//2 сообщение
//3 параметры сообщения WPARAM
//4 параметры сообщения LPARAM
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//точка входа ГЛАВНАЯ ФУНКЦИЯ принимает 4 параметра
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	/*MessageBox(NULL, "Hello WinAPI", "Hello",
		MB_YESNOCANCEL | MB_ICONINFORMATION | MB_HELP | MB_DEFBUTTON3 | MB_SYSTEMMODAL);//|-побитовое "или"
	//1 окно
	//2 текст сообщения
	//3 заголовок
	//4 содержимое поведения окна - влияет на внешний вид окна, определяет значек, набор кнопок, активную кнопку, модальность
	*/
	
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,(DLGPROC) DlgProc, NULL);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//функция SendMessage - так же принимает 4 параметра
	//1) hwnd - окно
	//2) uMsg - сообщение
	//3) wParam - параметры сообщения
	//4) lParam - параметр сообщения
	switch (uMsg)
	{
	case WM_INITDIALOG:		          //эта секция выполняется один раз , нужна для добавления элеметов в окно диалога
	{
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		//SetFocus(hEditLogin);      //при запуске окна мигает курсор в нашем окне
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));// сайт icon-icons.com - выбрать иконку
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
	break;
	case WM_COMMAND:		         //обрабатывает команды с клавиатуры и мыши
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_COPY:
		{   //объявили переменные в одной области видимости:{ ...... }
			CONST INT SIZE = 256;//объявили обычную строку 
			CHAR sz_buffer[SIZE] = {};
			//HWND window_handler = GetDlgItem(parent_window_handler, ELEMENT_ID);
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);      //обработчик окна, куда мы пишем текст
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);//обработчик окна куда мы хотим скопировать текст
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);//попросить какое-либо окно что-либо сделать , эта функция принимает 4 параметра
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);//окно куда копируем 
		}
		break;
		//если указано родительское окно hwnd - блокируется окно сообщений
		case IDOK:		MessageBox(hwnd, "Была нажата кнопка OK", "Info", MB_OK | MB_ICONINFORMATION);	break;
		case IDCANCEL:	EndDialog(hwnd, 0); break;
		}
	break;
	case WM_CLOSE:			//закрывает окно
		EndDialog(hwnd, 0);
	break;
	}
	return FALSE;

}
