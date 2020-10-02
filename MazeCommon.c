#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//コンソールクリア用関数
void cls(){
    system("cls");
    printf("\n\n");
}

//カーソル非表示用関数
void hideCursor(){
    HANDLE hOut;
	CONSOLE_CURSOR_INFO cci;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}

//カーソル表示用関数
void showCursor(){
    HANDLE hOut;
	CONSOLE_CURSOR_INFO cci;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = TRUE;
	SetConsoleCursorInfo(hOut, &cci);
}
