#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

int point = 0;

void printUI(int board[4][4]) {
	printf("\033[32m\033[H\n\n\t\t\t");
	printf("Let's play 2048 Game!\n\t\t\t");
    printf("your score : %d\033[0m\n\n", point);
    for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) printf("  .  ");
			else printf("  %d  ", board[i][j]);
		}
		printf("\t\t\t\n\n");
	}
	printf("\033[34m[ASDW] : move boxes\t\t\t\n");
	printf("[q] : QUIT, [r] : RESTART\t\t\t\n");
	printf("\033[0m\033[A\n");
}

bool sorted(int arr[4]) {
	for (int i = 0; i < 3; i++) {
		if (!arr[i]) {
			for (int j = i + 1; j < 4; j++) {
				if (arr[j]) {
					return false;
				}
			}
		}
	}
	return true;
}

void slide(int arr[4]) {
	if (arr[0] + arr[1] + arr[2] + arr[3] && !(arr[0] * arr[1] * arr[2] * arr[3])) {
		while (!sorted(arr)) {
			if (!arr[0]) {
				arr[0] = arr[1];
				arr[1] = arr[2];
				arr[2] = arr[3];
				arr[3] = 0;
			}
			if (!arr[1]) {
				arr[1] = arr[2];
				arr[2] = arr[3];
				arr[3] = 0;
			}
			if (!arr[2]) {
				arr[2] = arr[3];
				arr[3] = 0;
			}
		}
	}
}

bool merge_check(int arr[4]) {
	return (arr[0] == arr[1]||arr[1] == arr[2]||arr[2] == arr[3]);
}

void merge(int arr[4]) {
	if (arr[0] + arr[1] + arr[2] + arr[3]) {
		slide(arr);
		if (arr[0] == arr[1] && arr[0]) {
			point += 2 * arr[0];
			arr[0] += arr[1];
			arr[1] = 0;
		}
		if (arr[1] == arr[2] && arr[1]) {
			point += 2 * arr[1];
			arr[1] += arr[2];
			arr[2] = 0;
		}
		if (arr[2] == arr[3] && arr[2]) {
			point += 2 * arr[1];
			arr[2] += arr[3];
			arr[3] = 0;
		}
		slide(arr);
	}
}

void newTile(int board[4][4]) {
	int i = rand() % 4;
	int j = rand() % 4;
	int k;
	while (board[i][j]) {
		i = rand() % 4;
		j = rand() % 4;
	}
	k = rand() % 2;
	board[i][j] = k ? 2 : 4;
}
void initGame(int board[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			board[i][j] = 0;
		}
	}
	point = 0;
	newTile(board);
}
bool emptyCnt(int board[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) {
				return true;
			}
		}
	}
	return false;
}
bool fullCnt(int board[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2048) {
				return true;
			}
		}
	}
	return false;
}
void setBufferedInput(bool enable) {
	static bool enabled = true;
	static struct termios old;
	struct termios new;

	if (enable && !enabled) {
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
		enabled = true;
	}
	else if (!enable && enabled) {
		tcgetattr(STDIN_FILENO, &new);
		old = new;
		new.c_lflag &= (~ICANON & ~ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &new);
		enabled = false;
	}
}

void rotate(int board[4][4]) {
	int tmp;
	for (int i = 0; i < 2; i++) {
		for (int j = i; j < 3 - i; j++) {
			tmp = board[i][j];
			board[i][j] = board[j][3 - i];
			board[j][3 - i] = board[3 - i][3 - j];
			board[3 - i][3 - j] = board[3 - j][i];
			board[3 - j][i] = tmp;
		}
	}
}

void left(int board[4][4]) {
	for (int i = 0; i < 4; i++) {
		merge(board[i]);
	}
}
void down(int board[4][4]) {
	rotate(board);
	rotate(board);
	rotate(board);
	left(board);
	rotate(board);
}
void right(int board[4][4]) {
	rotate(board);
	rotate(board);
	left(board);
	rotate(board);
	rotate(board);
}
void up(int board[4][4]) {
	rotate(board);
	left(board);
	rotate(board);
	rotate(board);
	rotate(board);
}

bool gameOver(int board[4][4]) {
	if (emptyCnt(board)){
		return false;
  }
	for (int i = 0; i < 4; i++){
		if (merge_check(board[i])){
			return false;
    }
  }
  bool over = true;
	rotate(board);
	for (int i = 0; i < 4; i++){
		if (merge_check(board[i])){
			over = false;
    }
  }
	rotate(board);
	rotate(board);
	rotate(board);

	return over;
}

int main() {
	int board[4][4];
	initGame(board);
	printUI(board);
	setBufferedInput(false);
  char c;
  bool go;
  int key[] = {97, 115, 100, 119, 113, 114};
  bool counted = false;

	while (true) {
    go = false;
    c = getchar();
      
		switch (c) {
		case 97:
			left(board);
			break;
		case 115:
			down(board);
			break;
		case 100:
			right(board);
			break;
		case 119:
			up(board);
			break;
		case 113:
			printf("Do you want to QUIT? (y or n)\n");
			if (getchar() == 'y') {
				exit(0);
			}
			printUI(board);
			break;
		case 114:
			printf("Do you want to RESTART? (y or n)       \n");
			if (getchar() == 'y') {
				initGame(board);
			}
			printUI(board);
			break;
		default:
			break;
		}
		if (gameOver(board)) {
			printf("Your game is OVER!                 \n");
			exit(0);
		}
    if (!counted && fullCnt(board)) {
      printUI(board);
      printf("You win!                      \n");
      printf("Keep going? (y or n)                             \n");
      char ans = getchar();
      if (ans == 'y'){
        counted = true;
        printf("\033[A                             \n");
        continue;
      }
      else if (ans == 'n'){
        exit(0);
      }
    }
    
    for (int i=0; i<6; i++){
      if (c==key[i]){
        go = true;
        break;
      }
    }

    if (go && emptyCnt(board)){
  		newTile(board);
	  	printUI(board);
    }
	}
	setBufferedInput(true);
	return 0;
}