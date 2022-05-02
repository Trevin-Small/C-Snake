#ifndef MAIN_H
#define MAIN_H

#define GRID_SIZE 13
#define START_SIZE 4

#define GRID_CHAR  '#'
#define SNAKE_HEAD '0'
#define SNAKE_CHAR '*'
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

#endif //MAIN_H