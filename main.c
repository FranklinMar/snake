#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

// #define FIELD_WIDTH 20;
// #define FIELD_HEIGHT 20;
//#define SNAKE 1;
//#define APPLE 2;
unsigned int TIMEPAUSE = 100;
// unsigned int EMPTY = 0, SNAKE = 1, APPLE = 2;

enum {EMPTY = 0, SNAKE = 1, APPLE = 2};

const unsigned int FIELD_WIDTH = 21, FIELD_HEIGHT = 21;
// char vert_line = '║', hor_line = '═',
//   top_left = '╔', top_right = '╗',
//   bottom_left = '╚', bottom_right = '╝';
int vert_line = 186, hor_line = 205,
  top_left = 201, top_right = 187,
  bottom_left = 200, bottom_right = 188;
int big_block = 219, small_block = 254; //small_block = snake block

typedef struct Node Node;
typedef struct List List;
typedef enum Direction Direction;

enum Direction {UP, RIGHT, DOWN, LEFT};

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
void hidecursor();
void move(List *list, Direction direction);



int main() {
  // Hiding cursor
  hidecursor();
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
  
  // Printing game field frame
  system("cls");
  // clrscr();
  int hor_border = FIELD_HEIGHT + 1, vert_border = FIELD_WIDTH + 1;
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
      } else if (j == 0 || j == vert_border) {
        ch = vert_line;
      } else {
        ch = ' ';
      }
      putchar(ch);
    }
    putchar('\n');
  }

  // Returning cursor to the beginning of field
  printf("\033[22A"); // Cursor move up on 22 positions
  printf("\033[1C"); // Cursir move right on 1 position

  // Creating linked list for snake
  List *snake = (List *) malloc(sizeof(List));
  // Creating head of the snake/list in the middle of the field
  snake->head = snake->end = create_Node(FIELD_WIDTH/2, FIELD_HEIGHT/2);//(Node *) malloc(sizeof(Node));
  // Place the snake in the middle of the field
  //snake->head->x = FIELD_WIDTH/2;
  //snake->head->y = FIELD_HEIGHT/2;

  // Put snake on the field
  char ch = 0;
  unsigned int loop = 1;
  Direction dir = UP;
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
      if ((ch == 'w' || ch == 72) && dir != DOWN) {
        dir = UP;
      } else if ((ch == 'd' || ch == 77) && dir != LEFT) {
        dir = RIGHT;
      } else if ((ch == 's' || ch == 80) && dir != UP) {
        dir = DOWN;
      } else if ((ch == 'a' || ch == 75) && dir != RIGHT) {
        dir = LEFT;
      }

      if (ch == 27) {
        break;
      }
      // Move the snake

      /*switch (ch) {
        case 27:
          loop = 0;
          break;
        case 'w':
        case 72 :
          dir = UP;
          break;
        case 'd':
        case 77 :
          dir = RIGHT;
          if (snake->head->x == FIELD_WIDTH-1) {
            snake->head->x = 0;
          } else {
            snake->head->x++;
          }
          break;
        case 's':
        case 80 :
          dir = DOWN;
          if (snake->head->y == FIELD_HEIGHT-1) {
            snake->head->y = 0;
          } else {
            snake->head->y++;
          }
          break;
        case 'a':
        case 75 :
          dir = LEFT;
          if (snake->head->x == 0) {
            snake->head->x = FIELD_WIDTH-1;
          } else {
            snake->head->x--;
          }
          break;
      }*/
      //move(snake->head, dir);
    }
    
    move(snake, dir);
    field_put(field, snake, SNAKE);
    display(field);
    Sleep(TIMEPAUSE);
  }
  //snake->head->node = NULL;
    //field_put(field, snake, SNAKE);
    //display(field);
    //Sleep(TIMEPAUSE);
    // gotoxy(2, 2);
    // system("cls");
    // printf("\033[1D");
    //printf("\033[2B");
    //printf("HELLO!");
    //display(field);


  //field [0][0] = field [0][1] = field [1][0] = field[0][2] = 1;
  // display(field);
  return 0;
}



void move(List *list, Direction direction) {
  // Termination condition
  if (list->head == NULL) {
    return ;
  }
  Node *node = list->head;
  int temp_x = node->x, temp_y = node->y;//,
  //temp_x2, temp_y2;
  if (direction == UP) {
    if (node->y == 0) {
      node->y = FIELD_HEIGHT-1;
    } else {
      node->y--;
    }
  } else if (direction == RIGHT) {
    if (node->x == FIELD_WIDTH-1) {
      node->x = 0;
    } else {
      node->x++;
    }
  } else if (direction == DOWN) {
    if (node->y == FIELD_HEIGHT-1) {
      node->y = 0;
    } else {
      node->y++;
    }
  } else if (direction == LEFT) {
    if (node->x == 0) {
      node->x = FIELD_WIDTH-1;
    } else {
      node->x--;
    }
  }

  // Updating the rest of the list
  for (Node *i = node->node; i != NULL; i = i->node) {
    // Swapping the X value with Bitwise XOR
    // i->x = i->x ^ temp_x;
    // temp_x = i->x ^ temp_x;
    // i->x = i->x ^ temp_x;

    // Swapping the Y value with Bitwise XOR
    // i->y = i->y ^ temp_y;
    // temp_y = i->y ^ temp_y;
    // i->y = i->y ^ temp_y;

    // Swapping the X value
    i->x = i->x + temp_x;
    temp_x = i->x - temp_x;
    i->x = i->x - temp_x;

    // Swapping the Y value
    i->y = i->y + temp_y;
    temp_y = i->y - temp_y;
    i->y = i->y - temp_y;
  }
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

void hidecursor() {
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void display(int **field){
  /*int vert_line = 186, hor_line = 205,
    top_left = 201, top_right = 187,
    bottom_left = 200, bottom_right = 188;
  int big_block = 219, small_block = 254; //small_block = snake block
  int hor_border = FIELD_HEIGHT + 1, vert_border = FIELD_WIDTH + 1;
  system("cls");
  for (unsigned int i = 0; i <= hor_border; i++) {
    for (unsigned int j = 0; j <= vert_border; j++) {
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
        } else{
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

  }*/
  char ch;
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    for (unsigned int j = 0; j < FIELD_WIDTH; j++) {
      if (field[i][j] == EMPTY) {
        ch = ' ';
      } else {
        ch = big_block;
      }
      putchar(ch);
    }
    printf("\033[1C"); // Cursor move right on 1 position
    putchar('\n'); // Line break. Start from new line
    printf("\033[1C"); // Cursor move right on 1 position
  }
  printf("\033[21A"); // Cursor move up on 21 position - to the beginning of the frame
  // printf("HELLO!");
}

// printf("\033[XA"); // Move up X lines;
// printf("\033[XB"); // Move down X lines;
// printf("\033[XC"); // Move right X column;
// printf("\033[XD"); // Move left X column;
// printf("\033[2J"); // Clear screen