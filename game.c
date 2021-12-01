#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

void setBufferedInput(int);
void titleScreen();
void resetGame();
void spawnBlock();
void checkBlock();
void drawBoard();
int slideBlock(unsigned*[]);
int mergeBlock(unsigned*[]);
int moveUp();
int moveDown();
int moveLeft();
int moveRight();

unsigned score;
unsigned board[4][4];
int gameOn;
int numBlocks;

int main(void){
    setBufferedInput(0);
    titleScreen();

    resetGame();
    spawnBlock();
    spawnBlock();
    drawBoard();

    while(gameOn){
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

    return 0;
}

void titleScreen(){
    char c = 'S';
    printf("Use arrow keys or wasd\n");
    printf("Please turn off caps lock first\n");
    printf("Press lowercase 's' to start\n");
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
    score = 0;
    gameOn = 1;
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

void drawBoard(){
    // temporary
    printf("\nscore : %d\n", score);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int slideBlock(unsigned *line[]){
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

int mergeBlock(unsigned *line[]){
    int boardChanged = 0;
    for(int k = 0; k < 3; k++){
        if (*line[k] == 0) continue;
        if (*line[k] == *line[k+1]) {
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
    unsigned *line[4];
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
    unsigned *line[4];
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
    unsigned *line[4];
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
    unsigned *line[4];
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

/* IDEAS */
// best score or score board => file I/O