#include <stdio.h>
#include <conio.h>
#include "MazeCommon.h"
#include "MazeGame.h"

//タイトル
int MazeTitle(){
    int menu = -1;

    cls();

    printf("＊＊＊迷路ゲーム＊＊＊\n\n");

    printf("メニュー\n");
    printf("%d:ゲーム開始\n", GAMESTART);
    printf("%d:終了", EXIT);

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
        MazeGame();
    }

    return 0;
}
