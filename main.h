#ifndef MAIN_H
#define MAIN_H

#define GRID_SIZE 9
#define SNAKE_CHAR '0'
#define APPLE_CHAR '@'

typedef struct node {
  int x, y;
  struct node *next;
  struct node *prev;
} snake_node;

typedef struct snake_t {
  struct node *head;
  struct node *tail;
  int direction_x, direction_y;
} snake_t;

typedef struct apple {
  int x, y;
} apple_t;

extern snake_t *initialize_snake();
extern apple_t *initialize_apple();

#endif //MAIN_H