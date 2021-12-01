#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define CLEAR "\033[2J" // clear screen
#define TOP "\033[H" // move to top of screen

#define DEFAULT "\x1B[0m" // default color
#define BLINK "\x1B[5m" // blinking text
#define REVERSE "\x1B[7m" // reverse of default color
#define BLACK "\x1B[30m" // black text
#define COLOR1 "\x1B[100m" // grey background
#define COLOR2 "\x1B[101m" // red background
#define COLOR3 "\x1B[102m" // green background
#define COLOR4 "\x1B[103m" // yellow background
#define COLOR5 "\x1B[104m" // blue background
#define COLOR6 "\x1B[105m" // pink background
#define COLOR7 "\x1B[106m" // sky background
#define COLOR8 "\x1B[41m" // dark red background
#define COLOR9 "\x1B[42m" // dark green background
#define COLOR10 "\x1B[43m" // dark yellow background
#define COLOR11 "\x1B[45m" // dark pink background
#define COLOR12 "\x1B[46m" // dark sky background

void setBufferedInput(int);
void titleScreen();
void resetGame();
void spawnBlock();
void checkBlock();
void changeColor(int);
void drawBoard();
int slideBlock(short*[]);
int mergeBlock(short*[]);
int moveUp();
int moveDown();
int moveLeft();
int moveRight();
void saveScore();

unsigned bestScore;
unsigned score;
short board[4][4];
int numBlocks;
int gameOn;
int winState;
int scoreState;

int main(void){
    setBufferedInput(0);
    titleScreen();

    resetGame();
    spawnBlock();
    spawnBlock();
    drawBoard();

    while(gameOn){
        if (winState) break;
        char c = getchar();
        switch (c)
        {
        case 'w':
        case 65:
            if(moveUp()) spawnBlock();
            drawBoard();
            checkBlock();
            break;
        case 'a':
        case 68:
            if(moveLeft()) spawnBlock();
            drawBoard();
            checkBlock();
            break;
        case 's':
        case 66:
            if(moveDown()) spawnBlock();
            drawBoard();
            checkBlock();
            break;
        case 'd':
        case 67:
            if(moveRight()) spawnBlock();
            drawBoard();
            checkBlock();
            break;
        case 'Q':
            gameOn = 0;
            break;
        default:
            break;
        }
    }

    saveScore();
    drawBoard();
    setBufferedInput(1);
    printf("%s", DEFAULT);
    return 0;
}

void titleScreen(){
    printf("%s%s", CLEAR, TOP);
    printf("\n%*s%s    %s %s    %s %s %s  %s %s %s    %s\n", 8, "", COLOR2, DEFAULT, COLOR4, DEFAULT, COLOR3, DEFAULT, COLOR3, DEFAULT, COLOR7, DEFAULT);
    printf("%*s   %s %s %s %s  %s %s %s %s  %s %s %s %s  %s %s\n", 8, "", COLOR2, DEFAULT, COLOR4, DEFAULT, COLOR4, DEFAULT, COLOR3, DEFAULT, COLOR3, DEFAULT, COLOR7, DEFAULT, COLOR7, DEFAULT);
    printf("%*s%s    %s %s %s  %s %s %s    %s %s    %s\n", 8, "", COLOR2, DEFAULT, COLOR4, DEFAULT, COLOR4, DEFAULT, COLOR3, DEFAULT, COLOR7, DEFAULT);
    printf("%*s%s %s    %s %s  %s %s    %s %s %s %s  %s %s\n", 8, "", COLOR2, DEFAULT, COLOR4, DEFAULT, COLOR4, DEFAULT, COLOR3, DEFAULT, COLOR7, DEFAULT, COLOR7, DEFAULT);
    printf("%*s%s    %s %s    %s    %s %s %s    %s\n\n", 8, "", COLOR2, DEFAULT, COLOR4, DEFAULT, COLOR3, DEFAULT, COLOR7, DEFAULT);
    printf("%s", REVERSE);
    printf("      Use arrow keys or wasd       ");
    printf("%s\n", DEFAULT);
    printf("%s", REVERSE);
    printf("Merge blocks and make 2048 to win!!");
    printf("%s\n", DEFAULT);
    printf("%s", REVERSE);
    printf("     Turn off caps lock first      ");
    printf("%s\n", DEFAULT);
    printf("%s%s", BLINK, COLOR6);
    printf("   Press lowercase 's' to start    ");
    printf("%s\n", DEFAULT);
    char c = 'S';
    while(c != 's'){
        c = getchar();
    }
}

void setBufferedInput(int enable) {
	static int enabled = 1;
	static struct termios old;
	struct termios new;
	if (enable && !enabled) {
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		enabled = 1;
	} else if (!enable && enabled) {
		tcgetattr(STDIN_FILENO,&new);
		old = new;
		new.c_lflag &=(~ICANON & ~ECHO);
		tcsetattr(STDIN_FILENO,TCSANOW,&new);
		enabled = 0;
	}
}

void resetGame(){
    FILE *file;
    if ((file = fopen("best.txt", "r"))) {
        fscanf(file, "%d", &bestScore);
        fclose(file);
    }
    else {
        bestScore = 0;
    }
    score = 0;
    gameOn = 1;
    winState = 0;
    scoreState = 0;
    numBlocks = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            board[i][j] = 0;
        }
    }
}

void spawnBlock(){
    int i, j;
    srand(time(NULL));
    do
    {
        i = rand() % 4;
        j = rand() % 4;
    } while (board[i][j] != 0);
    board[i][j] = 2;
    numBlocks++;
}

void checkBlock(){
    if (numBlocks < 16) return;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            if (board[i][j] == board[i][j+1]) return;
        }
    }
    for(int j = 0; j < 4; j++){
        for(int i = 0; i < 3; i++){
            if (board[i][j] == board[i+1][j]) return;
        }
    }
    gameOn = 0;
}

void changeColor(int num){
    switch (num)
    {
        case 0:
            printf("%s%s", BLACK, COLOR1); break;
        case 2:
            printf("%s%s", BLACK, COLOR2); break;
        case 4:
            printf("%s%s", BLACK, COLOR3); break;
        case 8:
            printf("%s%s", BLACK, COLOR4); break;
        case 16:
            printf("%s%s", BLACK, COLOR5); break;
        case 32:
            printf("%s%s", BLACK, COLOR6); break;
        case 64:
            printf("%s%s", BLACK, COLOR7); break;
        case 128:
            printf("%s%s", BLACK, COLOR8); break;
        case 256:
            printf("%s%s", BLACK, COLOR9); break;
        case 512:
            printf("%s%s", BLACK, COLOR10); break;
        case 1024:
            printf("%s%s", BLACK, COLOR11); break;
        case 2048:
            printf("%s%s", BLACK, COLOR12); break;
        default:
            printf("%s", DEFAULT);
            break;
    }
}

void drawBoard(){
    printf("%s%s", CLEAR, TOP);
    printf("%s********%s%s %d %s %d %s %d %s %d %s%s********%s\n\n", REVERSE, DEFAULT, COLOR2, 2, COLOR4, 0, COLOR3, 4, COLOR7, 8, DEFAULT, REVERSE, DEFAULT);
    printf("best : %d\n", bestScore);
    printf("score : %d\n", score);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            changeColor(board[i][j]);
            printf("       ");
        }
        printf("%s\n", DEFAULT);
        for(int j = 0; j < 4; j++){
            changeColor(board[i][j]);
            if (board[i][j] == 0){
                printf("   *   ");
                continue;
            }
            int num = board[i][j];
            int length = 0;
            do
            {
                num /= 10;
                length++;
            } while (num != 0);
            int front = (7 - length) / 2;
            int back = 7 - length - front;
            printf("%*s%d%*s", front, "", board[i][j], back, "");
        }
        printf("%s\n", DEFAULT);
        for(int j = 0; j < 4; j++){
            changeColor(board[i][j]);
            printf("       ");
        }
        printf("%s\n", DEFAULT);
    }
    printf("\n");
    printf("%s", REVERSE);
    if (gameOn) printf(" Press uppercase 'Q' to quit\n");
    else {
        if (winState) printf("%s%s%s          YOU WON!!         %s\n", DEFAULT, BLINK, COLOR6, DEFAULT);
        else printf("         GAME OVER          \n");
        if (scoreState) printf("%s%s%s      NEW BEST SCORE!!      %s\n", DEFAULT, BLINK, COLOR7, DEFAULT);
    }
    printf("%s\n", DEFAULT);
}

int slideBlock(short *line[]){
    int boardChanged = 0;
    int index = 0;
    for(int k = 0; k < 4; k++){
        if (*line[k] != 0) {
            if (index != k) {
                *line[index] = *line[k];
                *line[k] = 0;
                boardChanged = 1;
            }
            index++;
        }
    }
    return boardChanged;
}

int mergeBlock(short *line[]){
    int boardChanged = 0;
    for(int k = 0; k < 3; k++){
        if (*line[k] == 0) continue;
        if (*line[k] == *line[k+1]) {
            if (*line[k] << 1 == 2048) winState = 1;
            score += *line[k] << 1;
            *line[k] = *line[k] << 1;
            *line[k+1] = 0;
            boardChanged = 1;
            numBlocks--;
        }
    }
    return boardChanged;
}

int moveUp(){
    int boardChanged = 0;
    short *line[4];
    for(int j = 0; j < 4; j++){
        for(int i = 0; i < 4; i++){
            line[i] = &board[i][j];
        }
        boardChanged += slideBlock(line);
        boardChanged += mergeBlock(line);
        boardChanged += slideBlock(line);
    }
    return boardChanged;
}

int moveDown(){
    int boardChanged = 0;
    short *line[4];
    for(int j = 0; j < 4; j++){
        for(int i = 0; i < 4; i++){
            line[i] = &board[3-i][j];
        }
        boardChanged += slideBlock(line);
        boardChanged += mergeBlock(line);
        boardChanged += slideBlock(line);
    }
    return boardChanged;
}

int moveLeft(){
    int boardChanged = 0;
    short *line[4];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            line[j] = &board[i][j];
        }
        boardChanged += slideBlock(line);
        boardChanged += mergeBlock(line);
        boardChanged += slideBlock(line);
    }
    return boardChanged;
}

int moveRight(){
    int boardChanged = 0;
    short *line[4];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            line[j] = &board[i][3-j];
        }
        boardChanged += slideBlock(line);
        boardChanged += mergeBlock(line);
        boardChanged += slideBlock(line);
    }
    return boardChanged;
}

void saveScore(){
    if (score <= bestScore) return;
    FILE *file;
    if ((file = fopen("best.txt", "w"))) {
        fprintf(file, "%d", score);
        fclose(file);
    }
    bestScore = score;
    scoreState = 1;
}