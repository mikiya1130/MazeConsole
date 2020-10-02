#include <stdio.h>
#include <conio.h>
#include "MazeCommon.h"
#include "MazeGame.h"

//タイトル
int MazeTitle(){
    int menu = -1;

    //コンソールクリア
    cls();

    printf("＊＊＊迷路ゲーム＊＊＊\n\n");

    printf("メニュー\n");
    printf("%d:EASY\n", EASY);
    printf("%d:EASY(HIDE)\n", EASY_H);
    printf("%d:NORMAL\n", NORMAL);
    printf("%d:NORMAL(HIDE)\n", NORMAL_H);
    printf("%d:HARD\n", HARD);
    printf("%d:HARD(HIDE)\n", HARD_H);
    printf("%d:CUSTOM\n", CUSTOM);
    printf("%d:CUSTOM(HIDE)\n", CUSTOM_H);
    printf("%d:EXIT\n\n", EXIT);
    printf("数字で選択");

    while(menu < 0 || menu > EXIT){
        menu = _getch() - '0';
    }

    return menu;
}

int main(void){
    int menu;

    printf("\033[?25l");    //カーソル非表示

    while(1){
        //メニュー選択
        menu = MazeTitle();

        if(menu == EXIT){   //EXITならwhileループを抜けて終了
            break;
        }

        //迷路ゲーム
        MazeGame(menu);
    }

    printf("\033[?25h");    //カーソル表示

    return 0;
}
