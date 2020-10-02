#include <stdio.h>
#include <conio.h>
#include "MazeCommon.h"
#include "MazePlayer.h"

//プレイヤー初期化
int MazePlayerInit(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn){
    int i, j;

    for(i = 0; i < mazeRow; i++){  //行
        for(j = 0; j < mazeColumn; j++){   //列
            if(maze[mazeColumn * i + j].kind == START){   //スタート地点ならばプレイヤーの位置に設定する
                *playerRow    = i;
                *playerColumn = j;
                return 0;   //関数終了(プレイヤーを設定できれば残りの処理は必要ない)
            }
        }
    }

    //スタート地点がなければ、プレイヤーを設定できずここまでくる
    printf("スタートがありません\n");
    return -1;
}

//矢印キー入力取得用関数
int getArrowKey(){
    int ch;

    ch = _getch();
    if(ch == 0x00 || ch == 0xe0){   //2バイト文字用
        ch = _getch();
    }

    switch(ch){
        case 0x48: return UP;    break;
        case 0x50: return DOWN;  break;
        case 0x4b: return LEFT;  break;
        case 0x4d: return RIGHT; break;
        default: return Invalid; break;
    }
}

//プレイヤー移動
void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn){
    int direction = -1;

    printf("\n矢印キーで移動");

    while(direction < 0 || direction > (Invalid - 1)){  //入力が正しい場合まで繰り返す
        direction = getArrowKey();
    }

    switch(direction){
        //上移動
        case UP:
            if(*playerRow - 1 >= 0){    //迷路の範囲外でないことを確認
                maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].flag = _TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].kind != WALL){   //壁かどうか確認
                    *playerRow -= 1;    //移動
                }
            }
            break;

        //下移動
        case DOWN:
            if(*playerRow + 1 < mazeRow){    //迷路の範囲外でないことを確認
                maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].flag = _TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].kind != WALL){   //壁かどうか確認
                    *playerRow += 1;    //移動
                }
            }
            break;

        //左移動
        case LEFT:
            if(*playerColumn - 1 >= 0){    //迷路の範囲外でないことを確認
                maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].flag = _TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].kind != WALL){   //壁かどうか確認
                    *playerColumn -= 1;    //移動
                }
            }
            break;

        //右移動
        case RIGHT:
            if(*playerColumn + 1 < mazeColumn){    //迷路の範囲外でないことを確認
                maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].flag = _TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].kind != WALL){   //壁かどうか確認
                    *playerColumn += 1;    //移動
                }
            }
            break;
    }
}
