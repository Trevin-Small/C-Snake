/* Trevin Small, May 2022
 * ----------------------------------------------------------------
 * C Snake Game
 */

#include "main.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ABS(x) (x) < 0 ? -(x) : (x)

bool is_alive = true;
snake_t *snake;
apple_t *apple;


/* Snake functions
 * ----------------------------------------------------------------
 */

void append_to_snake(snake_t *new_snake) {
  snake_node *node_ptr = malloc(sizeof(snake_node));

  node_ptr->prev = new_snake->tail;
  node_ptr->next = NULL;

  new_snake->tail->next = node_ptr;

  new_snake->tail = node_ptr;
}

snake_t *initialize_snake() {
  snake_t *new_snake = malloc(sizeof(snake_t));

  snake_node *head = malloc(sizeof(snake_node));

  new_snake->head = head;
  new_snake->tail = head;

  head->prev = NULL;
  head->next = NULL;
  head->x = GRID_SIZE / 2 + 1;
  head->y = GRID_SIZE / 2 + 1;

  snake_node *node = head;
  for (int i = 1; i < START_SIZE; i++) {
    append_to_snake(new_snake);
    node = node->next;
    node->x = node->prev->x;
    node->y = node->prev->y + 1;
  }

  new_snake->direction_x = 0;
  new_snake->direction_y = -1;

  return new_snake;
}

void free_snake() {
  snake_node *snake_current = snake->head;
  snake_node *snake_next = snake->head->next;

  while (snake_next != NULL) {
    free(snake_current);
    snake_current = snake_next;
    snake_next = snake_next->next;
  }

  free(snake_current);
  free(snake);
  snake = NULL;
}

bool snake_occupies(int x, int y, bool exclude_head) {
  snake_node *snake_ptr;

  if (exclude_head) {
    snake_ptr = snake->head;
  } else {
    snake_ptr = snake->head->next;
  }

  while (snake_ptr != NULL) {

    if (snake_ptr->x == x && snake_ptr->y == y) {
      return true;
    }

    snake_ptr = snake_ptr->next;
  }

  return false;
}

int size_of_snake() {
  snake_node *snake_ptr = snake->head;
  int counter = 0;

  while (snake_ptr != NULL) {
    counter++;
    snake_ptr = snake_ptr->next;
  }

  return counter;
}

/* Apple functions
 * ----------------------------------------------------------------
 */

apple_t *initialize_apple() {
  apple_t *new_apple = malloc(sizeof(apple_t));
  srandom(time(NULL));

  do {
    new_apple->x = random() % GRID_SIZE + 1;
  } while (new_apple->x == GRID_SIZE / 2 + 1);

  new_apple->y = random() % GRID_SIZE + 1;

  return new_apple;
}

void move_apple() {
  int new_x;
  int new_y;

  //srandom(time(NULL));

  do {

    new_x = random() % GRID_SIZE + 1;
    new_y = random() % GRID_SIZE + 1;

  } while (snake_occupies(new_x, new_y, true));

  apple->x = new_x;
  apple->y = new_y;

}

bool apple_occupies(int x, int y) {
  if (apple->x == x && apple->y == y) {
    return true;
  }
  return false;
}

/* Game loop functions
 * ----------------------------------------------------------------
 */

void take_input() {
  int c = getch();

  switch(c) {
    case KEY_UP:
      if (snake->direction_y == 0) {
        snake->direction_y = -1;
        snake->direction_x = 0;
      }
      break;
    case KEY_DOWN:
      if (snake->direction_y == 0) {
        snake->direction_y = 1;
        snake->direction_x = 0;
      }
      break;
    case KEY_RIGHT:
      if (snake->direction_x == 0) {
        snake->direction_x = 1;
        snake->direction_y = 0;
      }
      break;
    case KEY_LEFT:
      if (snake->direction_x == 0) {
        snake->direction_x = -1;
        snake->direction_y = 0;
      }
      break;
    default:
      refresh();
      break;
  }

}

void move_snake() {
  if ((ABS(snake->direction_x) > 0) || (ABS(snake->direction_y) > 0)) {

    if (!apple_occupies(snake->head->x + snake->direction_x, snake->head->y + snake->direction_y)) {

      snake->tail->x = snake->head->x + snake->direction_x;
      snake->tail->y = snake->head->y + snake->direction_y;

      snake->tail->next = snake->head;
      snake->head->prev = snake->tail;

      snake->tail = snake->tail->prev;
      snake->tail->next = NULL;
      snake->head->prev->prev = NULL;

      snake->head = snake->head->prev;

    } else {
      move_apple();
      snake_node *new_head = malloc(sizeof(snake_node));
      new_head->x = snake->head->x + snake->direction_x;
      new_head->y = snake->head->y + snake->direction_y;
      new_head->prev = NULL;
      new_head->next = snake->head;
      snake->head->prev = new_head;
      snake->head = new_head;

    }

    if (snake_occupies(snake->head->x, snake->head->y, false)) {
      is_alive = false;
    }

    if ((snake->head->x == 0) || (snake->head->x == GRID_SIZE + 1)) {
      is_alive = false;
    }

    if ((snake->head->y == 0) || (snake->head->y == GRID_SIZE + 1)) {
      is_alive = false;
    }

  }

}

void draw(char board[GRID_SIZE + 3]) {
  mvprintw(0, 0, "%s", board);

  for (int i = 0; i < GRID_SIZE; i++) {
    mvaddch(i + 1, 0, GRID_CHAR);
    mvaddch(i + 1, GRID_SIZE + 1, GRID_CHAR);

    for (int j = 0; j < GRID_SIZE; j++) {
      mvaddch(i + 1, j + 1, ' ');
    }
  }

  snake_node *snake_ptr = snake->head;

  mvaddch(snake_ptr->y, snake_ptr->x, SNAKE_HEAD);
  snake_ptr = snake_ptr->next;

  while(snake_ptr != NULL) {
    mvaddch(snake_ptr->y, snake_ptr->x, SNAKE_CHAR);
    snake_ptr = snake_ptr->next;
  }

  mvaddch(apple->y, apple->x, APPLE_CHAR);

  mvprintw(GRID_SIZE + 1, 0, "%s", board);
  mvprintw(GRID_SIZE + 3, 0, "Score: %d", size_of_snake() - START_SIZE);

}

int main() {

  /* Variable for tracking frame times */
  time_t prev_time = time(NULL);

  /* Initialize the snake doubly-linked list */
  snake = initialize_snake();

  /* Initialize the apple */
  apple = initialize_apple();

  /* Array to store string of '#' with length of board size */
  char board_edge[GRID_SIZE + 3];

  /* Populate string array */
  for (int i = 0; i < GRID_SIZE + 2; i++) {
    board_edge[i] = GRID_CHAR;
  }

  /* Ensure it is null terminated */
  board_edge[GRID_SIZE + 2] = '\0';

  /* Initialize window settings with ncurses */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(50);

  while (is_alive) {

    /* Take arrow key inputs */
    take_input();

    /* If one second has elapsed, update */
    if ((time(NULL) - prev_time) > 0) {

      /* Update the snake position */
      move_snake();

      /* Redraw the frame */
      draw(board_edge);

      /* Refresh the window */
      refresh();

      /* Update prev frame time */
      prev_time = time(NULL);
    }
  }

  int score = size_of_snake();

  /* Free memory allocated by snake */
  free_snake();

  /* Free memory allocated by apple */
  free(apple);

  /* End ncurses window */
  endwin();

  printf("Game over. Score: %d", score - START_SIZE);

  return 0;
}