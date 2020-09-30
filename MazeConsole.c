#include <stdio.h>
#include <stdlib.h>

//プレイヤー
typedef struct {
    int row;    //プレイヤー位置(行)
    int column; //プレイヤー位置(列)
} MazePlayer;

//迷路の一ブロック
enum MazeKind {PATH, WALL, START, GOAL};    //ブロックの種類(道、壁、スタート、ゴール)
enum MazeFlag {FALSE, TRUE};                //ブロックが判明しているかどうか

typedef struct {
    enum MazeKind kind; //種類(道、壁、スタート、ゴール)
    enum mezeFlag flag; //ブロックが判明しているかどうか
} MazeBlock;

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

//迷路表示
void MazeDraw(int playerRow, int playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn){
    int i, j;

    for(i = 0; i < mazeRow; i++){  //行
        for(j = 0; j < mazeColumn; j++){   //列
            if(i == playerRow && j == playerColumn){    //プレイヤー位置
                printf("Ｐ");
            }
            else if(maze[mazeColumn * i + j].flag == FALSE){   //ブロックが判明していない場合
                printf("？");
            }
            else{
                switch(maze[mazeColumn * i + j].kind){
                    case WALL:
                        printf("□");    //壁
                        break;
                    case GOAL:
                        printf("Ｇ");    //ゴール
                        break;
                    default:
                        printf("　");    //その他(道、スタート)
                        break;
                }
            }
        }
        printf("\n");
    }
}

//方向
enum MazeDirection {UP, DOWN, LEFT, RIGHT, Invalid};

//プレイヤー移動
void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn){
    char buf[100];
    int direction = -1;

    printf("%d:上\n", UP);
    printf("%d:下\n", DOWN);
    printf("%d:左\n", LEFT);
    printf("%d:右\n", RIGHT);
    printf("数字を入力してください：");

    fgets(buf, sizeof(buf), stdin); //文字列で入力を読み込む
    sscanf_s(buf, "%d", &direction);  //数字にできる場合は変換(できなければ何もしない)

    while(direction < 0 || direction > (Invalid - 1)){  //入力が正しい場合まで繰り返す
        printf("入力が不正です。再入力してください：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", &direction);
    }

    switch(direction){
        //上移動
        case UP:
            if(*playerRow - 1 >= 0){    //迷路の範囲外でないことを確認
                maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].kind != WALL){   //壁かどうか確認
                    *playerRow -= 1;    //移動
                    printf("\n上に移動しました。\n");
                }
                else{
                    printf("\n壁です。\n");
                }
            }
            else{
                printf("\n範囲外です\n");
            }

            break;

        //下移動
        case DOWN:
            if(*playerRow + 1 < mazeRow){    //迷路の範囲外でないことを確認
                maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].kind != WALL){   //壁かどうか確認
                    *playerRow += 1;    //移動
                    printf("\n下に移動しました。\n");
                }
                else{
                    printf("\n壁です。\n");
                }
            }
            else{
                printf("\n範囲外です\n");
            }

            break;

        //左移動
        case LEFT:
            if(*playerColumn - 1 >= 0){    //迷路の範囲外でないことを確認
                maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].kind != WALL){   //壁かどうか確認
                    *playerColumn -= 1;    //移動
                    printf("\n左に移動しました。\n");
                }
                else{
                    printf("\n壁です。\n");
                }
            }
            else{
                printf("\n範囲外です\n");
            }

            break;

        //右移動
        case RIGHT:
            if(*playerColumn + 1 < mazeColumn){    //迷路の範囲外でないことを確認
                maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].flag = TRUE;    //ブロックの種類が判明

                if(maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].kind != WALL){   //壁かどうか確認
                    *playerColumn += 1;    //移動
                    printf("\n右に移動しました。\n");
                }
                else{
                    printf("\n壁です。\n");
                }
            }
            else{
                printf("\n範囲外です\n");
            }

            break;
    }
}

//ゴール判定
int MazeGoalCheck(int playerRow, int playerColumn, MazeBlock *maze, int mazeColumn){
    if(maze[mazeColumn * playerRow + playerColumn].kind == GOAL){ //プレイヤー位置がゴール地点に等しい
        printf("ゴール！\n");
        return 1;
    }
    return 0;
}

//迷路ゲーム
void MazeGame(){
    char fileName[100];
    char buf[100];
    FILE *fp;
    int i, kind;
    int goalCheck = 0;
    int mazeRow, mazeColumn;

    //プレイヤー
    MazePlayer player;

    //迷路
    MazeBlock *maze;

    //ファイル読み込み
    printf("ファイル名を入力してください：");

    fgets(fileName, sizeof(fileName), stdin);

    //改行を終端文字に書き換える
    for(i = 0; i < 100; i++){
        if(fileName[i] == '\n'){
            fileName[i] = '\0';
        }
    }

    if(fopen_s(&fp, fileName, "r") != 0){
        //ファイルのオープンに失敗した場合、ゲームを開始できないので関数を終了する。
        printf("ファイルが存在しません。\n");
        return;
    }

    //迷路サイズ取得
    if(fgets(buf, sizeof(buf), fp) != NULL){    //1行読み込み
        if(sscanf_s(buf, "%d,%d", &mazeRow, &mazeColumn) != 2){
            // 2つの数字を読み込めなかった場合失敗
            printf("サイズが読み込めません。\n");
            fclose(fp);
            return;
        }
    }
    else{
        //データがない場合、fgetsが失敗する
        printf("データがありません。\n");
        fclose(fp);
        return;
    }

    //配列の動的確保
    maze = (MazeBlock *)malloc(sizeof(MazeBlock) * mazeRow * mazeColumn);

    if(maze == NULL){
        printf("メモリの確保に失敗しました。");
        fclose(fp);
        return;
    }

    //迷路設定
    for(i = 0; i < mazeRow * mazeColumn; i++){
        if(fgets(buf, sizeof(buf), fp) != NULL){    //1行読み込み
            if(sscanf_s(buf, "%d", &kind) == 1){
                switch(kind){
                    case PATH:
                    case WALL:
                        maze[i].kind = kind;
                        maze[i].flag = FALSE;
                        break;

                    case GOAL:
                        goalCheck = 1;
                    case START:
                        maze[i].kind = kind;
                        maze[i].flag = TRUE;
                        break;

                    default:
                        //ファイルのデータが正しい数値ではなかった場合
                        printf("不正なデータが存在します。\n");
                        fclose(fp);
                        free(maze);
                        return;
                        break;
                }
            }
            else{
                //数字がなかった場合、sscanfが失敗する
                printf("不正なデータが存在します。\n");
                fclose(fp);
                free(maze);
                return;
            }
        }
        else{
            //データが途中で終わっている場合、fgetsが失敗する
            printf("データ数が足りません。\n");
            fclose(fp);
            free(maze);
            return;
        }
    }

    fclose(fp);

    //ゴールが存在するか確認
    if(goalCheck == 0){
        printf("ゴールが存在しません。\n");
        free(maze);
        return;
    }

    //プレイヤー初期化
    if(MazePlayerInit(&player.row, &player.column, maze, mazeRow, mazeColumn) == -1){
        //関数MazePlayerInitが-1を返すとき初期化に失敗している
        //よって、この時点でプログラムを終了し、迷路の表示は行わない
        return;
    }

    while(MazeGoalCheck(player.row, player.column, maze, mazeColumn) != 1){ //ゴールするまで移動を繰り返す
        //迷路表示
        MazeDraw(player.row, player.column, maze, mazeRow, mazeColumn);
        //プレイヤー移動
        MazePlayerMove(&player.row, &player.column, maze, mazeRow, mazeColumn);
    }

    //迷路最終結果表示
    MazeDraw(player.row, player.column, maze, mazeRow, mazeColumn);

    free(maze);
}

//メニュー
enum MazeMenu {GAMESTART, EXIT};

//タイトル
int MazeTitle(){
    char buf[100];
    int menu = -1;

    printf("\n\n＊＊＊迷路ゲーム＊＊＊\n\n");

    printf("メニュー\n");
    printf("%d:ゲーム開始\n", GAMESTART);
    printf("%d:終了\n", EXIT);

    printf("メニューを選んでください：");

    fgets(buf, sizeof(buf), stdin);
    sscanf_s(buf, "%d", &menu);

    while(menu < 0 || menu > EXIT){
        printf("入力が不正です。再入力してください：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", &menu);
    }

    return menu;
}

int main(void){
    int menu;

    while(1){
        //メニュー選択
        menu = MazeTitle();
        printf("\n");

        if(menu == EXIT){   //EXITならwhileループを抜けて終了
            break;
        }

        //迷路ゲーム
        MazeGame();
    }

    return 0;
}
