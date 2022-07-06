#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

// #define FIELD_WIDTH 20;
// #define FIELD_HEIGHT 20;
//#define SNAKE 1;
//#define APPLE 2;
unsigned int TIMEPAUSE = 100;
unsigned int EMPTY = 0, SNAKE = 1, APPLE = 2;
unsigned int FIELD_WIDTH = 21, FIELD_HEIGHT = 21;

typedef struct Node Node;
typedef struct List List;

struct Node {
  unsigned int x;
  unsigned int y;
  Node *node;
};

struct List {
  Node *head;
  Node *end;
};

//void display(int **field);
void textcolor(int color);
int random(int min, int max);
void display(int **field);
Node * create_Node(unsigned int x, unsigned int y);
void field_put(int **field, List *list, unsigned int value);



int main() {
  // Creating 2D array with dynamically allocated memory
  int **field = (int **) malloc(sizeof(int *) * FIELD_HEIGHT);
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    field[i] = (int *) malloc(sizeof(int) * FIELD_WIDTH);
  }

  // Initialising array;
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    for (unsigned int j = 0; j < FIELD_WIDTH; j++) {
      field[i][j] = 0;
    }
  }
  // Creating linked list for snake
  List *snake = (List *) malloc(sizeof(List));
  // Creating head of the snake/list
  snake->head = snake->end = create_Node(FIELD_WIDTH/2, FIELD_HEIGHT/2);//(Node *) malloc(sizeof(Node));
  // Place the snake in the middle of the field
  //snake->head->x = FIELD_WIDTH/2;
  //snake->head->y = FIELD_HEIGHT/2;

  // Put snake on the field
  char ch = 0;
  unsigned int loop = 1;
  //timeout(0);
  while (loop) {
    if (kbhit()) {
      ch = getch();
      //if (ch == 0 || ch == 0xE0) {
      //  ch = getch();
      //}
      //if (ch == 'q') {
      //  break;
      //}
      switch (ch) {
        case 27:
          loop = 0;
          break;
        case 'w':
        case 72 :
          snake->head->y--;
          break;
        case 'd':
        case 77 :
          snake->head->x++;
          break;
        case 's':
        case 80 :
          snake->head->y++;
          break;
        case 'a':
        case 75 :
          snake->head->x--;
          break;
      }
    }
    field_put(field, snake, SNAKE);
    display(field);
    Sleep(TIMEPAUSE);
  }
  //snake->head->node = NULL;



  //field [0][0] = field [0][1] = field [1][0] = field[0][2] = 1;
  display(field);
  return 0;
}



Node * create_Node(unsigned int x, unsigned int y) {
  if (x >= FIELD_WIDTH || y >= FIELD_HEIGHT) {
    return NULL;
  }
  Node *node = (Node *) malloc(sizeof(Node));
  //node->x = node->y = 0;
  node->x = x;
  node->y = y;
  node->node = NULL;
  return node;
}

int random(int min, int max) {
  return rand() % max + min;
}

void textcolor(int color) {
    static int __BACKGROUND;

    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


    GetConsoleScreenBufferInfo(h, &csbiInfo);

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
      color + (__BACKGROUND << 4));
}

void field_put(int **field, List *list, unsigned int value) {
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    for (unsigned int j = 0; j < FIELD_WIDTH; j++) {
      field[i][j] = EMPTY;
    }
  }

  for (Node *i = list->head; i != NULL; i = i->node) {
    field[i->y][i->x] = value;
  }
}

void display(int **field){
  // char vert_line = '║', hor_line = '═',
  //   top_left = '╔', top_right = '╗',
  //   bottom_left = '╚', bottom_right = '╝';
  int vert_line = 186, hor_line = 205,
    top_left = 201, top_right = 187,
    bottom_left = 200, bottom_right = 188;
  int big_block = 219, small_block = 254; //small_block = snake block
  int hor_border = FIELD_HEIGHT + 1, vert_border = FIELD_WIDTH + 1;
  system("cls");
  // clrscr();
  for (unsigned int i = 0; i <= hor_border; i++) {
    for (unsigned int j = 0; j <= vert_border; j++) {
      /*if (i == 0 && j == 0) {
        printf(top_left_cor);
      } else if (i == 0 || i == hor_border) {
        print(hor_line);
      } else if (j == 0 || j == vert_border) {
        printf(vert_line);
      }*/
      char ch = 0;
      if (i == 0 || i == hor_border || j == 0 || j == vert_border) {
        if (i == 0 && j == 0) {
          ch = top_left;
        } else if (i == 0 && j == vert_border) {
          ch = top_right;
        } else if (i == hor_border && j == 0) {
          ch = bottom_left;
        } else if (i == hor_border && j == vert_border) {
          ch = bottom_right;
        } else if (i == 0 || i == hor_border) {
          ch = hor_line;
        } else/* if (j == 0 || j == vert_border)*/ {
          ch = vert_line;
        }
      } else {
        if (field[i-1][j-1] != 0) {
          //textcolor(2);
          // system("COLOR 2");
          //textcolor(2);
          ch = big_block;
        } else {
          ch = ' ';
        }
      }
      // printf(ch);
      // putchar(ch);
      putchar(ch);
      // system("COLOR 7");
      //textcolor(7);
    }
    // printf('\n');
    putchar('\n');
    //textcolor(15);
  }
}