#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

void setBufferedInput(int);
void titleScreen();
void resetGame();
void spawnBlock();
void drawBoard();
void mergeBlock();

unsigned score;
unsigned board[4][4];

int main(void){
    setBufferedInput(0);
    titleScreen();

    resetGame();
    spawnBlock();
    spawnBlock();
    drawBoard();

    while(1){
        char c = getchar();
        switch (c)
        {
        case 'w':
        case 65:
            printf("up\n"); // implement this
            break;
        case 'a':
        case 68:
            printf("left\n"); // implement this
            break;
        case 's':
        case 66:
            printf("down\n"); // implement this
            break;
        case 'd':
        case 67:
            printf("right\n"); // implement this
            break;
        case 'q':
            printf("quit\n"); // implement this
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
		// restore the former settings
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		// set the new state
		enabled = 1;
	} else if (!enable && enabled) {
		// get the terminal settings for standard input
		tcgetattr(STDIN_FILENO,&new);
		// we want to keep the old setting to restore them at the end
		old = new;
		// disable canonical mode (buffered i/o) and local echo
		new.c_lflag &=(~ICANON & ~ECHO);
		// set the new settings immediately
		tcsetattr(STDIN_FILENO,TCSANOW,&new);
		// set the new state
		enabled = 0;
	}
}

void resetGame(){
    score = 0;
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
}

void drawBoard(){
    // temporary
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

/* IDEAS */
// best score or score board => file I/O