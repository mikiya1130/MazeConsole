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
    if(_getch() == 224){
        switch(_getch()){
            case 72: return UP;    break;
            case 80: return DOWN;  break;
            case 75: return LEFT;  break;
            case 77: return RIGHT; break;
            default: return Invalid; break;
        }
    }
    else{
        return Invalid;
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
                maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].kind != WALL){   //壁かどうか確認
                    *playerRow -= 1;    //移動
                    //printf("\n上に移動しました。\n");
                }
                else{
                    //printf("\n壁です。\n");
                }
            }
            else{
                //printf("\n範囲外です\n");
            }

            break;

        //下移動
        case DOWN:
            if(*playerRow + 1 < mazeRow){    //迷路の範囲外でないことを確認
                maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].kind != WALL){   //壁かどうか確認
                    *playerRow += 1;    //移動
                    //printf("\n下に移動しました。\n");
                }
                else{
                    //printf("\n壁です。\n");
                }
            }
            else{
                //printf("\n範囲外です\n");
            }

            break;

        //左移動
        case LEFT:
            if(*playerColumn - 1 >= 0){    //迷路の範囲外でないことを確認
                maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].kind != WALL){   //壁かどうか確認
                    *playerColumn -= 1;    //移動
                    //printf("\n左に移動しました。\n");
                }
                else{
                    //printf("\n壁です。\n");
                }
            }
            else{
                //printf("\n範囲外です\n");
            }

            break;

        //右移動
        case RIGHT:
            if(*playerColumn + 1 < mazeColumn){    //迷路の範囲外でないことを確認
                maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].kind != WALL){   //壁かどうか確認
                    *playerColumn += 1;    //移動
                    //printf("\n右に移動しました。\n");
                }
                else{
                    //printf("\n壁です。\n");
                }
            }
            else{
                //printf("\n範囲外です\n");
            }

            break;
    }
}
