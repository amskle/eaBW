#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <windows.h>
#define SIZE 8

// 棋盘和棋子表示
char board[SIZE][SIZE];
const char EMPTY = '.';
const char BLACK = 'B';
const char WHITE = 'W';

// 玩家当前棋子颜色
char currentPlayer = WHITE;

// 棋子计数器
int blackCount = 0;
int whiteCount = 0;

// 方向数组：8个可能的方向
const int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};


// 菜单显示
void showmenu(){
    printf("欢迎来到黑白棋游戏!\n");
    printf("请选择你所要的操作：\n");
    printf("1.我要开始游戏\n");
    printf("2.我要知道游戏规则\n");
    printf("3.我要设置游戏难度\n");
    printf("4.我要查看上局记录\n")
    printf("5.我要删除上局记录\n");
    printf("6.我要退出游戏\n");
}
// 初始化棋盘
void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
    //设置初始棋子
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
    
    // 初始化计数器
    blackCount = 0;
    whiteCount = 0;
}

// 显示棋盘
void displayBoard() {
    printf("\n   ");
    for (int i = 0; i < SIZE; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");
    
    for (int i = 0; i < SIZE; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

// 在指定位置落子并翻转对手棋子
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

// 主游戏循环
void playGame() {
    bool gameOver = false;
    int passCount = 0;
    
    printf("黑白棋游戏开始！\n");
    printf("白方(%c)先行\n", WHITE);
    
    while (!gameOver) {
        displayBoard();
        printf("当前棋子: %s方(%c)\n", (currentPlayer == WHITE) ? "白" : "黑", currentPlayer);
        printf("黑棋: %d, 白棋: %d\n", blackCount, whiteCount);
        
        if (hasValidMove(currentPlayer)) {
            passCount = 0;
            printf("请输入落子位置(例如: A1): ");
            
            char input[10];
            fgets(input, sizeof(input), stdin);
            
            if (input[0] >= 'A' && input[0] <= 'H') {
                int col = input[0] - 'A';
                int row = input[1] - '1';
                
                if (isValidMove(row, col, currentPlayer)) {
                    makeMove(row, col, currentPlayer);
                    // 切换玩家
                    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
                } else {
                    printf("非法落子位置，请重试！\n");
                }
            } else {
                printf("输入格式错误，请使用字母+数字格式(如A1)\n");
            }
        } else {
            printf("%s方无合法落子位置，跳过回合！\n", 
                  (currentPlayer == WHITE) ? "白" : "黑");
            passCount++;
            currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
            
            // 如果双方都无合法移动，游戏结束
            if (passCount >= 2) {
                gameOver = true;
            }
        }
        
        // 检查棋盘是否已满
        if (blackCount + whiteCount >= SIZE * SIZE) {
            gameOver = true;
        }
    }
    
    // 游戏结束，显示结果
    displayBoard();
    printf("游戏结束！\n");
    printf("黑棋: %d, 白棋: %d\n", blackCount, whiteCount);
    
    if (blackCount > whiteCount) {
        printf("黑方获胜！\n");
    } else if (whiteCount > blackCount) {
        printf("白方获胜！\n");
    } else {
        printf("平局！\n");
    }
}

int main() {
    SetConsoleOutputCP(65001);
    showmenu();
    initializeBoard();
    playGame();
    return 0;
}