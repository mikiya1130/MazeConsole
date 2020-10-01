#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

//マス目(プレイヤー座標、壁生成開始座標)
typedef struct {
    int row;
    int column;
} MazeCell;

//迷路の一ブロック
enum MazeKind {PATH, WALL, EXWALL, START, GOAL};    //ブロックの種類(道、壁、スタート、ゴール)
enum MazeFlag {FALSE, TRUE};                //ブロックが判明しているかどうか

typedef struct {
    enum MazeKind kind; //種類(道、壁、スタート、ゴール)
    enum MazeFlag flag; //ブロックが判明しているかどうか
} MazeBlock;

//方向
enum MazeDirection {UP, DOWN, LEFT, RIGHT, Invalid};

//壁伸ばし法で迷路生成
void mazeCreate(MazeBlock *maze, int mazeRow, int mazeColumn){
    int i, j;
    int index;
    int x, y;
    int dir;

    //壁生成開始候補座標リスト
    MazeCell *mazeStartCells;

    //壁生成開始候補座標リストの長さ
    int mazeCellLength = 0;

    //壁拡張可能方向リスト
    int directions[4];

    //配列の動的確保
    mazeStartCells = (MazeCell *)malloc(sizeof(MazeCell) * ((mazeRow - 3) / 2) * ((mazeColumn - 3) / 2));

    //マス目の初期化、壁伸ばし開始点の登録
    for(i = 0; i < mazeRow; i++){
        for(j = 0; j < mazeColumn; j++){
            //外周を道、他を道で初期化
            if(i == 0 || j == 0 || i == mazeRow - 1 || j == mazeColumn - 1){
                maze[mazeColumn * i + j].kind = WALL;
            }
            else{
                maze[mazeColumn * i + j].kind = PATH;

                //外周ではない偶数座標を壁伸ばし開始点にしておく
                if(i % 2 == 0 && j % 2 == 0){
                    mazeStartCells[mazeCellLength].row    = i;
                    mazeStartCells[mazeCellLength].column = j;
                    mazeCellLength++;
                }
            }
        }
    }

    //乱数の初期化
    srand((unsigned)time(NULL));

    //壁が拡張できなくなるまでループ
    while(mazeCellLength > 0){
        //ランダムに開始セルを取得
        index = rand() % mazeCellLength;
        x = mazeStartCells[index].row;
        y = mazeStartCells[index].column;

        //配列末尾の要素で上書きして、開始候補から削除
        mazeStartCells[index].row = mazeStartCells[mazeCellLength - 1].row;
        mazeStartCells[index].column = mazeStartCells[mazeCellLength - 1].column;

        //リストの長さ更新
        mazeCellLength--;

        //既に壁の場合は何もしない
        if(maze[mazeColumn * x + y].kind == WALL){
            continue;
        }

        //拡張開始地点を壁にする
        maze[mazeColumn * x + y].kind = EXWALL;

        //壁拡張可能方向リストの初期化
        for(i = 0; i < 4; i++){
            directions[i] = TRUE;
        }

        //壁を生成拡張
        while(  //全ての方向が探索済みになるまで(1つ以上TRUEの間ループ)
            directions[0] == TRUE
            || directions[1] == TRUE
            || directions[2] == TRUE
            || directions[3] == TRUE
        ){
            //拡張する方向
            dir = rand() % 4;

            //辿ってきた道 or 探索済みでないか判定
            if(directions[dir] == FALSE){
                continue;
            }

            switch(dir){
                //上に移動
                case UP:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * x + (y - 1)].kind == PATH
                        && maze[mazeColumn * x + (y - 2)].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y - 1)].kind = EXWALL;
                        maze[mazeColumn * x + (y - 2)].kind = EXWALL;

                        //座標更新
                        y -= 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //下は辿ってきた道
                        directions[DOWN] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * x + (y - 1)].kind == PATH
                        && maze[mazeColumn * x + (y - 2)].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y - 1)].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //上は拡張不可
                        directions[UP] = FALSE;
                    }
                    break;

                //下に移動
                case DOWN:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * x + (y + 1)].kind == PATH
                        && maze[mazeColumn * x + (y + 2)].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y + 1)].kind = EXWALL;
                        maze[mazeColumn * x + (y + 2)].kind = EXWALL;

                        //座標更新
                        y += 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //上は辿ってきた道
                        directions[UP] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * x + (y + 1)].kind == PATH
                        && maze[mazeColumn * x + (y + 2)].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y + 1)].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //下は拡張不可
                        directions[DOWN] = FALSE;
                    }
                    break;

                //左に移動
                case LEFT:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * (x - 1) + y].kind == PATH
                        && maze[mazeColumn * (x - 2) + y].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x - 1) + y].kind = EXWALL;
                        maze[mazeColumn * (x - 2) + y].kind = EXWALL;

                        //座標更新
                        x -= 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //右は辿ってきた道
                        directions[RIGHT] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * (x - 1) + y].kind == PATH
                        && maze[mazeColumn * (x - 2) + y].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x - 1) + y].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //左は拡張不可
                        directions[LEFT] = FALSE;
                    }
                    break;

                //右に移動
                case RIGHT:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * (x + 1) + y].kind == PATH
                        && maze[mazeColumn * (x + 2) + y].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x + 1) + y].kind = EXWALL;
                        maze[mazeColumn * (x + 2) + y].kind = EXWALL;

                        //座標更新
                        x += 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //左は辿ってきた道
                        directions[LEFT] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * (x + 1) + y].kind == PATH
                        && maze[mazeColumn * (x + 2) + y].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x + 1) + y].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //右は拡張不可
                        directions[RIGHT] = FALSE;
                    }
                    break;
            }
        }

        //拡張中だった壁を確定させる
        for(i = 0; i < mazeRow; i++){
            for(j = 0; j < mazeColumn; j++){
                if(maze[mazeColumn * i + j].kind == EXWALL){
                    maze[mazeColumn * i + j].kind = WALL;
                }
            }
        }
    }
}

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

//プレイヤー移動
void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn){
    int direction = -1;

    printf("%d:上\n", UP);
    printf("%d:下\n", DOWN);
    printf("%d:左\n", LEFT);
    printf("%d:右\n", RIGHT);
    printf("数字を入力してください：");

    direction = _getch() - '0';

    while(direction < 0 || direction > (Invalid - 1)){  //入力が正しい場合まで繰り返す
        printf("入力が不正です。再入力してください：");
        direction = _getch() - '0';
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
    int goalCheck = 0;
    int mazeRow, mazeColumn;
    char buf[100];

    //プレイヤー
    MazeCell player;

    //迷路
    MazeBlock *maze;

    //迷路サイズ入力
    printf("迷路の高さを入力してください(5以上99以下の奇数)：");
    fgets(buf, sizeof(buf), stdin);
    sscanf_s(buf, "%d", &mazeRow);
    while(mazeRow < 5 || mazeRow > 99 || mazeRow % 2 == 0){
        printf("入力値が不正です。迷路の高さを入力してください(5以上99以下の奇数)：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", &mazeRow);
    }

    printf("迷路の幅を入力してください(5以上99以下の奇数)：");
    fgets(buf, sizeof(buf), stdin);
    sscanf_s(buf, "%d", &mazeColumn);
    while(mazeColumn < 5 || mazeColumn > 99 || mazeColumn % 2 == 0){
        printf("入力値が不正です。迷路の幅を入力してください(5以上99以下の奇数)：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", &mazeColumn);
    }

    //配列の動的確保
    maze = (MazeBlock *)malloc(sizeof(MazeBlock) * mazeRow * mazeColumn);

    if(maze == NULL){
        printf("メモリの確保に失敗しました。");
        return;
    }

    //壁伸ばし法で迷路生成
    mazeCreate(maze, mazeRow, mazeColumn);

    //スタートとゴール位置
    maze[mazeColumn * 1 + 1].kind = START;
    maze[mazeColumn * (mazeRow - 2) + (mazeColumn - 2)].kind = GOAL;

    //ゴールが存在するか確認
    // if(goalCheck == 0){
    //     printf("ゴールが存在しません。\n");
    //     free(maze);
    //     return;
    // }

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

    menu = _getch() - '0';

    while(menu < 0 || menu > EXIT){
        printf("入力が不正です。再入力してください：");
        menu = _getch() - '0';
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
