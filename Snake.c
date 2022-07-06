#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

/* CONSTANTS */
// Time constants
unsigned int TIMEPAUSE = 100, GAMEOVER_TIME = 5000;
// Field dimensions (preferably odd numbers)
const unsigned int FIELD_WIDTH = 21, FIELD_HEIGHT = 21;
// Field indicators
enum {EMPTY = 0, SNAKE = 1, FOOD = 2};
// Frame character numbers
int vert_line = 186, hor_line = 205,
  top_left = 201, top_right = 187,
  bottom_left = 200, bottom_right = 188;

// char vert_line = '║', hor_line = '═',
//   top_left = '╔', top_right = '╗',
//   bottom_left = '╚', bottom_right = '╝';

// Block instances character numbers
int SNAKE_BLOCK = 219, small_block = 254;

/* STRUCTURES */
typedef enum Direction Direction;
typedef struct Node Node;
typedef struct List List;

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



/* FUNCTIONS PROTOTYPES */
// Used for displaying
void hidecursor();
void textcolor(int color);
void field_put(int **field, Node *node, unsigned int value);
void field_clear(int **field);
void display(int **field);
// Mechanics functions
Node * create_Node(unsigned int x, unsigned int y);
void move(List *list, Direction direction);
unsigned int head_intersects_list(List *list);
// Random generators
int random(int min, int max);
Node * generate_food(List *list);



int main() {
  /* VARIABLES */
  // Keyboard input character
  char ch = 0;
  // Field 2D array
  int **field;
  // Default direction
  Direction dir = UP;
  // Snake list
  List *snake;
  // Food node
  Node *food;


  // Initialization of random number generator
  srand(time(NULL));
  // Hiding cursor
  hidecursor();
  // Creating array with dynamically allocated memory
  field = (int **) malloc(sizeof(int *) * FIELD_HEIGHT);
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    field[i] = (int *) malloc(sizeof(int) * FIELD_WIDTH);
  }
  // Initialising array;
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    for (unsigned int j = 0; j < FIELD_WIDTH; j++) {
      field[i][j] = 0;
    }
  }
  
  // Clearing the console
  system("cls");
  // Printing game field frame
  int hor_border = FIELD_HEIGHT + 1, vert_border = FIELD_WIDTH + 1;
  for (unsigned int i = 0; i <= hor_border; i++) {
    for (unsigned int j = 0; j <= vert_border; j++) {
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

  // Returning cursor to the beginning of the field
  printf("\033[22A"); // Cursor move up on 22 positions
  printf("\033[1C"); // Cursor move right on 1 position

  // Creating linked list for snake
  snake = (List *) malloc(sizeof(List));
  // Creating head of the snake/list in the middle of the field
  snake->head = snake->end = create_Node(FIELD_WIDTH/2, FIELD_HEIGHT/2);
  // Create snake body sections below the tail
  snake->end->node = create_Node(snake->end->x, snake->end->y);
  snake->end = snake->end->node;
  snake->end->node = create_Node(snake->end->x, snake->end->y);
  snake->end = snake->end->node;

  // Food coordinates generation
  food = generate_food(snake);

  while (1) {
    // Move the snake
    move(snake, dir);

    // If snake intersects itself - terminate
    if (head_intersects_list(snake) == 1) {
      break;
    }
    
    // If a key was pressed
    if (kbhit()) {
      ch = getch();
      
      // If it's an arrow key
      if (ch == 0 || ch == 0xE0) {
       ch = getch();
      }
      
      if ((ch == 'w' || ch == 72) && dir != DOWN) {
        dir = UP;
      } else if ((ch == 'd' || ch == 77) && dir != LEFT) {
        dir = RIGHT;
      } else if ((ch == 's' || ch == 80) && dir != UP) {
        dir = DOWN;
      } else if ((ch == 'a' || ch == 75) && dir != RIGHT) {
        dir = LEFT;
      }

      // If ESC key was pressed/Termination condition
      if (ch == 27) {
        break;
      }
    }

    // If food was taken
    if (snake->head->x == food->x && snake->head->y == food->y) {
      food = generate_food(snake);
      snake->end->node = create_Node(snake->end->x, snake->end->y);
      snake->end = snake->end->node;
    }

    // Put snake and food on the field
    field_put(field, snake->head, SNAKE);
    field_put(field, food, FOOD);

    // Display and clearthe field
    display(field);
    field_clear(field);

    // Pause
    Sleep(TIMEPAUSE);
  }
  // Gameover pause
  Sleep(GAMEOVER_TIME);
  return 0;
}



void hidecursor() {
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void textcolor(int color) {
    static int __BACKGROUND;

    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    GetConsoleScreenBufferInfo(h, &csbiInfo);

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
      color + (__BACKGROUND << 4));
}

void field_put(int **field, Node *node, unsigned int value) {
  for (Node *i = node; i != NULL; i = i->node) {
    field[i->y][i->x] = value;
  }
}

void field_clear(int **field) {
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    for (unsigned int j = 0; j < FIELD_WIDTH; j++) {
      field[i][j] = EMPTY;
    }
  }
}

void display(int **field){
  char ch;
  for (unsigned int i = 0; i < FIELD_HEIGHT; i++) {
    for (unsigned int j = 0; j < FIELD_WIDTH; j++) {
      if (field[i][j] == EMPTY) {
        ch = ' ';
      } else if (field[i][j] == SNAKE){
        textcolor(2);
        ch = SNAKE_BLOCK;
      } else if (field[i][j] == FOOD) {
        textcolor(4);
        ch = small_block;
      }
      putchar(ch);
      textcolor(7);
    }
    printf("\033[1C"); // Cursor move right on 1 position
    putchar('\n'); // Line break. Start from new line
    printf("\033[1C"); // Cursor move right on 1 position
  }
  printf("\033[21A"); // Cursor move up on 21 position - to the beginning of the frame;
}

Node * create_Node(unsigned int x, unsigned int y) {
  if (x >= FIELD_WIDTH || y >= FIELD_HEIGHT) {
    return NULL;
  }
  Node *node = (Node *) malloc(sizeof(Node));
  node->x = x;
  node->y = y;
  node->node = NULL;
  return node;
}

void move(List *list, Direction direction) {
  if (list == NULL) {
    return ;
  }
  if (list->head == NULL) {
    return ;
  }

  Node *node = list->head;
  int temp_x = node->x, temp_y = node->y;
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

unsigned int head_intersects_list(List *list) {
  if (list == NULL) {
    return 2;
  }
  if (list->head == NULL) {
    return 2;
  }
  if (list->head->node == NULL) {
    return 0;
  }
  Node *head = list->head;
  for (Node *i = head->node; i != NULL; i = i->node) {
    if (head->x == i->x && head->y == i->y) {
      return 1;
    }
  }
  return 0;
}

int random(int min, int max) {
  return rand() % max + min;
}

Node *generate_food(List *list) {
  unsigned int x, y;
  unsigned int condition = 1;
  while (condition) {
    x = random(0, FIELD_WIDTH);
    y = random(0, FIELD_WIDTH);
    condition = 0;
    for (Node *i = list->head; i != NULL; i = i->node) {
      if (x == i->x && y == i->y) {
        condition = 1;
        break;
      }
    }
  }
  Node *node = create_Node(x, y);
  return node;
}



/* CURSOR COMMENTS */

// printf("\033[XA"); // Move up X lines;
// printf("\033[XB"); // Move down X lines;
// printf("\033[XC"); // Move right X column;
// printf("\033[XD"); // Move left X column;
// printf("\033[2J"); // Clear screen