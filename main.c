/* Trevin Small, May 2022
 * ----------------------------------------------
 * C Snake Game
 */

#include "main.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#define ABS(x) (x) < 0 ? -(x) : (x)

bool is_alive = true;
snake_t *snake;
apple_t *apple;

snake_t *initialize_snake() {
  snake_t *new_snake = malloc(sizeof(snake_t));

  snake_node *head = malloc(sizeof(snake_node));
  snake_node *tail = malloc(sizeof(snake_node));

  head->prev = NULL;
  head->next = tail;
  head->x = GRID_SIZE / 2;
  head->y = GRID_SIZE / 2;

  tail->prev = head;
  tail->next = NULL;
  tail->x = GRID_SIZE / 2;
  tail->y = GRID_SIZE / 2 + 1;

  new_snake->head = head;
  new_snake->tail = tail;
  new_snake->direction_x = 0;
  new_snake->direction_y = 1;

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

apple_t *initialize_apple() {
  apple_t *new_apple = malloc(sizeof(apple_t));
  srandom(time(NULL));
  new_apple->x = random() % GRID_SIZE + 1;
  new_apple->y = random() % GRID_SIZE + 1;
  return new_apple;
}

bool is_touching_apple() {
  if ((snake->head->x == apple->x) && (snake->head->y == apple->y)) {
    srandom(time(NULL));
    apple->x = random() % GRID_SIZE + 1;
    apple->y = random() % GRID_SIZE + 1;
    return true;
  }

  return false;
}

bool snake_occupies(int x, int y) {
  snake_node *snake_ptr = snake->head;

  while (snake_ptr != NULL) {

    if (snake_ptr->x == x && snake_ptr->y == y) {
      return true;
    }

    snake_ptr = snake_ptr->next;
  }

  return false;
}

bool apple_occupies(int x, int y) {
  if (apple->x == x && apple->y == y) {
    return true;
  }
  return false;
}

int size_of_snake() {
  snake_node *snake_ptr = snake->head;
  int counter = 0;

  while (snake_ptr != NULL) {
    snake_ptr = snake_ptr->next;
    counter++;
  }

  return counter;
}

void get_input() {
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

void move_snake(bool add_segment) {
  if ((ABS(snake->direction_x) > 0) || (ABS(snake->direction_y) > 0)) {
    if (!add_segment) {

      snake->tail->x = snake->head->x + snake->direction_x;
      snake->tail->y = snake->head->y + snake->direction_y;

      snake->tail->next = snake->head;
      snake->head->prev = snake->tail;

      snake->tail = snake->tail->prev;
      snake->tail->next = NULL;
      snake->head->prev->prev = NULL;

      snake->head = snake->head->prev;

    } else {

      snake_node *new_head = malloc(sizeof(snake_node));
      new_head->x = snake->head->x + snake->direction_x;
      new_head->y = snake->head->y + snake->direction_y;
      new_head->prev = NULL;
      new_head->next = snake->head;
      snake->head = new_head;

    }

    if ((snake->head->x == 0) || (snake->head->x == GRID_SIZE)) {
      is_alive = false;
    }

    if ((snake->head->y == 0) || (snake->head->y == GRID_SIZE)) {
      is_alive = false;
    }

  }

}

void draw(char *board) {

  char line[GRID_SIZE + 3];

  line[0] = '#';
  line[GRID_SIZE + 1] = '#';
  line[GRID_SIZE + 2] = '\0';
  printf("\n%s\n", board);

  for (int i = 0; i < GRID_SIZE; i++) {

    for (int j = 1; j < GRID_SIZE + 1; j++) {
      if (snake_occupies(j, i)) {
        line[j] = SNAKE_CHAR;
      } else if (apple_occupies(j, i)) {
        line[j] = APPLE_CHAR;
      } else {
        line[j] = ' ';
      }
    }

    printf("%s\n", line);
  }

  printf("%s\n", board);
}

int main() {

  time_t prev_time = time(NULL);
  snake = initialize_snake();
  apple = initialize_apple();

  char board_edge[GRID_SIZE + 1];

  for (int i = 0; i < GRID_SIZE + 2; i++) {
    board_edge[i] = '#';
  }

  while (is_alive) {
    //take_input();
    if ((time(NULL) - prev_time) > 0) {
      move_snake(is_touching_apple());
      draw(board_edge);
      prev_time = time(NULL);
    }
  }

  //free_snake();
  free(apple);

  printf("\n\nGame Over. Your Score: %d\n\n", size_of_snake() - 2);

  return 0;
}