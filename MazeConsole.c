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
    printf("%d:NORMAL\n", NORMAL);
    printf("%d:HARD\n", HARD);
    printf("%d:CUSTOM\n", CUSTOM);
    printf("%d:EXIT\n\n", EXIT);
    printf("数字で選択");

    while(menu < 0 || menu > EXIT){
        menu = _getch() - '0';
    }

    return menu;
}

int main(void){
    int menu;

    while(1){
        //メニュー選択
        menu = MazeTitle();

        if(menu == EXIT){   //EXITならwhileループを抜けて終了
            break;
        }

        //迷路ゲーム
        MazeGame(menu);
    }

    return 0;
}
