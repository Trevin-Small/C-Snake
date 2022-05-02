# C Snake Game
### A doubly-linked list based snake clone written in C.
- The snake is represented as a double-linked list.
- Moving forwards involves popping the tail node from the list, and prepending it to the head. Its coordinates are then modified in respect to the direction the snake is moving.
- When an apple is touched, its new position is randomized (ensuring that the apple is not spawned on the snake)
- Adding to the snake allocates the memory for a new node, and prepends it to the snake, becoming the new head.
- The game is written using ncurses for keyboard input and improved screen printing to terminal

# Example
![Example Snake Image](https://github.com/Trevin-Small/C-Snake/blob/main/example.png)
- The game board size can be set in the header file
- The game runs at 1fps (lol)
- Touching the walls or running into your tail ends the game.