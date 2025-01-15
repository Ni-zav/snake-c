# snake-c

This is a simple implementation of the classic snake game using the ncurses library in C.

## Setup

To set up and run the game, follow these steps:

1. Clone the repository:
   ```
   git clone https://github.com/your-username/snake-game.git
   ```

2. Make sure you have the ncurses library installed on your system. On most Linux distributions, you can install it using the package manager. For example, on Ubuntu, you can run:
   ```
   sudo apt-get install libncurses5-dev libncursesw5-dev
   ```
   
On Windows, replace ncurses with [PDCurses](https://pdcurses.org/). Download PDCurses, include its headers and link against its libraries in your project.

3. Open the `Makefile` in a text editor and make sure the `CC` variable is set to your C compiler. For example, if you're using GCC, it should look like this:
   ```
   CC = gcc
   ```

4. Build the game by running the following command in the terminal:
   ```
   make
   ```

5. Run the game by executing the generated `snake` binary:
   ```
   ./snake
   ```

## How to Play

The objective of the game is to eat the fruit (represented by `F`) without colliding with the walls or your own body.

- Use the WASD keys to control the snake's direction.
- Eat the fruit to grow the snake and increase your score.
- Avoid colliding with your own body (walls is passable). If you do, the game ends.
- The game becomes progressively harder as you score higher. The snake moves faster and the fruit appears less frequently.

## Controls

- WASD keys: Change the direction of the snake.
- X key: Quit the game.

## Acknowledgments

- The game was inspired by the classic snake game.
- The ncurses library was used for handling the terminal interface.
