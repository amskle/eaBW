#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#define SIZE 8
#define RECORD_FILE "record.txt"

//初始化棋盘棋子参数
char board[SIZE][SIZE];
const char EMPTY = '.';
const char BLACK = 'B';
const char WHITE = 'W';

// 玩家当前棋子颜色
char currentPlayer = WHITE;

//难度初始化
int aiLevel = 1; // 1=普通，2=困难

//提示初始化
int hintEnabled = 0;

// 棋子计数器
int blackCount = 0;
int whiteCount = 0;

// 方向数组：8个可能的方向
const int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

//菜单功能
void showmenu(){
    printf("欢迎来到黑白棋游戏!\n");
    printf("请选择你所要的操作：\n");
    printf("1.我要开始游戏\n");
    printf("2.我要知道游戏规则\n");
    printf("3.我要设置游戏难度\n");
    printf("4.我要查看上局记录\n");
    printf("5.我要删除上局记录\n");
    printf("6.我要退出游戏\n");
    printf("0.隐藏选项");
}

//初始化棋盘
void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;

    // 初始化计数器
    blackCount = 2;
    whiteCount = 2;
}

// 检查位置是否在棋盘内
bool isValidPosition(int row, int col) {
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}

// 检查在指定位置落子是否合法
bool isValidMove(int row, int col, char player) {
    // 如果位置已被占用
    if (board[row][col] != EMPTY) {
        return false;
    }
    
    char opponent = (player == BLACK) ? WHITE : BLACK;
    bool valid = false;
    
    // 检查所有8个方向
    for (int dir = 0; dir < 8; dir++) {
        int r = row + dx[dir];
        int c = col + dy[dir];
        bool foundOpponent = false;
        
        // 沿着方向检查
        while (isValidPosition(r, c)) {
            if (board[r][c] == opponent) {
                foundOpponent = true;
            } else if (board[r][c] == player && foundOpponent) {
                valid = true; // 找到可翻转的序列
                break;
            } else {
                break; // 遇到空位或玩家棋子但中间没有对手棋子
            }
            r += dx[dir];
            c += dy[dir];
        }
    }
    
    return valid;
}

void makeMove(int row, int col, char player) {
    board[row][col] = player;
    
    char opponent = (player == BLACK) ? WHITE : BLACK;
    
    // 更新计数器
    if (player == BLACK) blackCount++;
    else whiteCount++;
    
    // 检查所有8个方向并翻转棋子
    for (int dir = 0; dir < 8; dir++) {
        int r = row + dx[dir];
        int c = col + dy[dir];
        bool foundOpponent = false;
        int count = 0;
        
        // 沿着方向检查
        while (isValidPosition(r, c)) {
            if (board[r][c] == opponent) {
                foundOpponent = true;
                count++;
            } else if (board[r][c] == player && foundOpponent) {
                // 翻转中间的所有对手棋子
                int flipR = row + dx[dir];
                int flipC = col + dy[dir];
                
                for (int i = 0; i < count; i++) {
                    board[flipR][flipC] = player;
                    // 更新计数器
                    if (player == BLACK) {
                        blackCount++;
                        whiteCount--;
                    } else {
                        whiteCount++;
                        blackCount--;
                    }
                    flipR += dx[dir];
                    flipC += dy[dir];
                }
                break;
            } else {
                break;
            }
            r += dx[dir];
            c += dy[dir];
        }
    }
}

// 检查玩家是否有合法移动
bool hasValidMove(char player) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (isValidMove(i, j, player)) {
                return true;
            }
        }
    }
    return false;
}
// 记录结构体
typedef struct {
    int blackCount;
    int whiteCount;
    double blackTime;
    double whiteTime;
    char mode[16]; // "人机" 或 "对战"
    char victor[16];
} GameRecord;

// 显示棋盘
void displayBoardEx(double blackTime, double whiteTime) {
    printf("\n  计数 黑:%d 白:%d  时间 黑:%.1fs 白:%.1fs\n", blackCount, whiteCount, blackTime, whiteTime);
    printf("    ");
    for (int i = 0; i < SIZE; i++) printf("%c ", 'A' + i);
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%2d  ", i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == BLACK) printf("○ ");
            else if (board[i][j] == WHITE) printf("● ");
            else if (hintEnabled&&isValidMove(i,j,currentPlayer)){
                printf("? ");
            }
            else printf("· ");
        }
        printf("\n");
    }
    printf("\n");
}

// 规则说明
void showRules() {
    printf("黑白棋规则：\n");
    printf("1. 黑白双方轮流下子，白棋先行。\n");
    printf("2. 每次下子必须翻转对方至少一枚棋子。\n");
    printf("3. 棋盘下满或双方都无合法落子时游戏结束。\n");
    printf("4. 棋子多者获胜。\n\n");
    system("pause");
    system("cls");
    
}

// 难度设置（这里只做简单提示）
void setDifficulty() {
    printf("请选择人机难度：\n1.普通模式\n2.困难模式\n请输入数字：");
    int level;
    scanf("%d", &level); getchar();
    if (level == 2) aiLevel = 2;
    else aiLevel = 1;
    printf("已设置为%s模式。\n", aiLevel == 2 ? "困难" : "普通");
    system("pause");
    system("cls");
}

// 保存记录
void saveRecord(GameRecord rec) {
    FILE *fp = fopen(RECORD_FILE, "a");
    if(rec.blackCount>rec.whiteCount){
        strcpy(rec.victor,"黑方胜");
    }
    else if (rec.whiteCount>rec.blackCount) {
        strcpy(rec.victor,"白方胜");
    } else {
        strcpy(rec.victor,"平局");
    }
    if (fp) {
        fprintf(fp, "%s,%d,%d,%.1f,%.1f,%s\n", rec.mode, rec.blackCount, rec.whiteCount, rec.blackTime, rec.whiteTime,rec.victor);
        fclose(fp);
    }
}

// 查看记录
void showRecord() {
    FILE *fp = fopen(RECORD_FILE, "r");
    if (!fp) {
        printf("暂无记录。\n");
        system("pause");
        system("cls");
        return;
    }

    char line[128];
    printf("模式,黑棋,白棋,黑用时,白用时,战局状况\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    system("pause");
    system("cls");
}

// 删除记录
void deleteRecord() {
    remove(RECORD_FILE);
    printf("记录已删除。\n");
    system("pause");
    system("cls");
}

// 修改记录
void changeRecord() {
    FILE *fp = fopen(RECORD_FILE,"r");
    if(!fp){
        printf("暂无记录，无法修改");
        system("pause");
        system("cls");
        return;
    }
    char lines[100][100];
    int count = 0;
    while (fgets(lines[count],sizeof(lines[0]),fp)){
        count++;
    }
    fclose(fp);
    if(count == 0) {
        printf("暂无记录，无法修改。\n");
        system("pause");
        system("cls");
        return;
    }
    printf("最后一条记录: \n%s\n",lines[count-1]);
    char mode[32], victor[32];
    int black, white;
    double btime, wtime;
    sscanf(lines[count-1], "%[^,],%d,%d,%lf,%lf,%[^,\n]", mode, &black, &white, &btime, &wtime, victor);
    printf("请输入新的胜者（black/white/draw）：");
    char newVictor[32];
    // scanf("%15s", newVictor); getchar();
    fgets(newVictor, sizeof(newVictor), stdin);
    newVictor[strcspn(newVictor, "\n")] = 0;
    // 重新写入所有记录，修改最后一条
    FILE *fw = fopen(RECORD_FILE, "w");
    for (int i = 0; i < count-1; i++) {
        fputs(lines[i], fw);
    }
    fprintf(fw, "%s,%d,%d,%.1f,%.1f,%s\n", mode, black, white, btime, wtime, newVictor);
    fclose(fw);
    printf("修改成功！\n");
    system("pause");
    system("cls");
}
// 简单AI
void aiMove() {
    int moves[64][2], cnt = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (isValidMove(i, j, currentPlayer)) {
                moves[cnt][0] = i; moves[cnt][1] = j; cnt++;
            }
    if (cnt > 0) {
        srand((unsigned)time(NULL));
        int idx = rand() % cnt;
        makeMove(moves[idx][0], moves[idx][1], currentPlayer);
        printf("AI落子: %c%d\n", 'A' + moves[idx][1], moves[idx][0] + 1);
        system("pause");
        system("cls");
    }
}
// 困难AI
void aiMoveHard() {
    int bestScore = -1000, bestMove[2] = {-1, -1};
    int moves[64][2], cnt = 0;
    // 角落和边权重
    int scoreMap[SIZE][SIZE] = {
        {100, -10, 10, 5, 5, 10, -10, 100},
        {-10, -20, 1, 1, 1, 1, -20, -10},
        {10, 1, 5, 2, 2, 5, 1, 10},
        {5, 1, 2, 1, 1, 2, 1, 5},
        {5, 1, 2, 1, 1, 2, 1, 5},
        {10, 1, 5, 2, 2, 5, 1, 10},
        {-10, -20, 1, 1, 1, 1, -20, -10},
        {100, -10, 10, 5, 5, 10, -10, 100}
    };
    // 找所有可落子点并评分
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (isValidMove(i, j, currentPlayer)) {
                int score = scoreMap[i][j];
                if (score > bestScore) {
                    bestScore = score;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
                moves[cnt][0] = i; moves[cnt][1] = j; cnt++;
            }
    if (cnt > 0) {
        int row, col;
        if (bestMove[0] != -1) {
            row = bestMove[0]; col = bestMove[1];
        } else {
            srand((unsigned)time(NULL));
            int idx = rand() % cnt;
            row = moves[idx][0]; col = moves[idx][1];
        }
        makeMove(row, col, currentPlayer);
        printf("AI(困难)落子: %c%d\n", 'A' + col, row + 1);
        system("pause");
        system("cls");
    }
}

// 主菜单
void menu() {
    int choice;
    while (1) {
        showmenu();
        printf("请输入选项(1-6): ");
        scanf("%d", &choice); getchar();
        if (choice == 1) {
            printf("1.玩家对战  2.人机对战\n请选择: ");
            int mode; 
            scanf("%d", &mode); 
            getchar();
            GameRecord rec = {0};
            // clock_t bStart = 0, wStart = 0;
            double bTime = 0, wTime = 0;
            initializeBoard();
            currentPlayer = WHITE;
            bool gameOver = false;
            int passCount = 0;
            printf("游戏开始！白方先行。\n");
            strcpy(rec.mode, mode == 2 ? "人机" : "对战");
            while (!gameOver) {
                displayBoardEx(bTime, wTime);
                printf("当前棋子: %s方(%c)\n", (currentPlayer == WHITE) ? "白" : "黑", currentPlayer);
                printf("黑棋: %d, 白棋: %d\n", blackCount, whiteCount);
                clock_t t1 = clock();
                if (hasValidMove(currentPlayer)) {
                    passCount = 0;
                    if (mode == 2 && currentPlayer == BLACK) {
                        if(aiLevel == 2){
                            aiMoveHard();
                        }
                        else{
                            aiMove();
                        }
                        bTime += (double)(clock() - t1) / CLOCKS_PER_SEC;
                        currentPlayer = WHITE;
                        continue;
                    }
                    printf("请输入落子位置(如A1): ");
                    char input[10];
                    fgets(input, sizeof(input), stdin);
                    int col = toupper(input[0]) - 'A';
                    int row = input[1] - '1';
                    if (col >= 0 && col < SIZE && row >= 0 && row < SIZE && isValidMove(row, col, currentPlayer)) {
                        makeMove(row, col, currentPlayer);
                        double dt = (double)(clock() - t1) / CLOCKS_PER_SEC;
                        if (currentPlayer == BLACK) bTime += dt; else wTime += dt;
                        currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
                    } else {
                        printf("非法落子位置，请重试！\n");
                        system("pause");
                        system("cls");
                    }
                } else {
                    printf("%s方无合法落子，跳过回合！\n", (currentPlayer == WHITE) ? "白" : "黑");
                    passCount++;
                    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
                    if (passCount >= 2) gameOver = true;
                }
                if (blackCount + whiteCount >= SIZE * SIZE) gameOver = true;
            }
            
            displayBoardEx(bTime, wTime);
            printf("游戏结束！\n黑棋: %d, 白棋: %d\n", blackCount, whiteCount);
            if (blackCount > whiteCount) {printf("黑方获胜！\n");system("pause");system("cls");}
            else if (whiteCount > blackCount) {printf("白方获胜！\n");system("pause");system("cls");}
            else {printf("平局！\n");system("pause");system("cls");}
            rec.blackCount = blackCount; rec.whiteCount = whiteCount;
            rec.blackTime = bTime; rec.whiteTime = wTime;
            saveRecord(rec);
        } else if (choice == 2) {
            showRules();
        } else if (choice == 3) {
            setDifficulty();
        } else if (choice == 4) {
            showRecord();
        } else if (choice == 5) {
            deleteRecord();
        } else if (choice == 6) {
            printf("退出游戏。\n");
            break;
        } else if (choice == 0) {
            printf("1.开启/关闭提示功能\n");
            printf("2.修改上次的记录\n");
            int choice1;
            scanf("%d",&choice1);getchar();
            if (choice1 == 1) {
                hintEnabled = !hintEnabled;
                printf("提示功能已%s!\n", hintEnabled?"开启" : "关闭");
                system("pause");
                system("cls");
            }else if (choice1 == 2){
                changeRecord();
            }
            
        }
        else {
            printf("无效选项，请重试。\n");
            system("pause");
            system("cls");
        }
    }
}

int main() {
    SetConsoleOutputCP(65001);
    menu();
    return 0;
}