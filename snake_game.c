#include <ncurses.h> // For the curses-based user interface
#include <stdlib.h> // For rand() and srand()
#include <unistd.h> // For usleep()
#include <time.h> // For time()

// Define the game area dimensions
#define WIDTH 40
#define HEIGHT 20

// Function prototypes
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int tailLength;
int dir; // Direction:1=left, 2=right, 3=up, 4=down
int gameOver;
int level;
int fruitsEaten;
int delay;

/**
 * Initializes the game state and settings.
 * Sets up the screen, game variables and initial direction.
 * Also sets up the initial fruit position and score.
 */
void Setup() {
    initscr();          // Start curses mode
    clear();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Non-blocking input

    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    gameOver = 0;
    dir = 0; // Initial direction: 0=stop
    tailLength = 0;
    level = 1;
    fruitsEaten = 0;
    delay = 300; // Initial delay in milliseconds (initial speed)
}

/**
 * Draws the current game state on the screen.
 * This includes the top wall, the game area, the sides and the bottom wall.
 * The game area is drawn with the snake head, the fruit and the tail.
 * The score and level are also displayed.
 */
void Draw() {
    int i, j, k;
    clear();

    // Draw top wall
    for (i = 0; i < WIDTH + 2; i++)
        mvprintw(0, i, "#");

    // Draw sides and content
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (j == 0)
                mvprintw(i + 1, j, "#");

            if (i == y && j == x)
                mvprintw(i + 1, j + 1, "O"); // Snake head
            else if (i == fruitY && j == fruitX)
                mvprintw(i + 1, j + 1, "F"); // Fruit
            else {
                int printed = 0;
                for (k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        mvprintw(i + 1, j + 1, "o"); // Tail
                        printed = 1;
                    }
                }
                if (!printed)
                    mvprintw(i + 1, j + 1, " ");
            }

            if (j == WIDTH - 1)
                mvprintw(i + 1, j + 2, "#");
        }
    }

    // Draw bottom wall
    for (i = 0; i < WIDTH + 2; i++)
        mvprintw(HEIGHT + 1, i, "#");

    // Display score and level
    mvprintw(HEIGHT + 3, 0, "Score: %d", score);
    mvprintw(HEIGHT + 4, 0, "Level: %d", level);
    refresh();
}

/**
 * Reads the user input and changes the direction of the snake accordingly.
 * The available commands are:
 *  - KEY_LEFT or 'a' to move left
 *  - KEY_RIGHT or 'd' to move right
 *  - KEY_UP or 'w' to move up
 *  - KEY_DOWN or 's' to move down
 *  - 'x' to exit the game
 */
void Input() {
    int ch = getch(); // Get user input
    switch (ch) {
        case KEY_LEFT:
        case 'a':
            if (dir != 2)
                dir = 1;
            break;
        case KEY_RIGHT:
        case 'd':
            if (dir != 1)
                dir = 2;
            break;
        case KEY_UP:
        case 'w':
            if (dir != 4)
                dir = 3;
            break;
        case KEY_DOWN:
        case 's':
            if (dir != 3)
                dir = 4;
            break;
        case 'x':
            gameOver = 1;
            break;
    }
}

/**
 * Updates the game state according to the current direction and user input.
 * Moves the snake head according to the current direction, moves the tail
 * accordingly, and checks for wall and self collisions. If the snake head
 * collides with the fruit, the score is increased and the fruit is moved to
 * a new position. If the snake head collides with the wall or itself, the
 * game over flag is set to 1.
 * The game level and speed are also updated according to the number of fruits
 * eaten.
 */
void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    int i;

    tailX[0] = x;
    tailY[0] = y;
    for (i = 1; i < tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];

        tailX[i] = prevX;
        tailY[i] = prevY;

        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case 1:
            x--;
            break;
        case 2:
            x++;
            break;
        case 3:
            y--;
            break;
        case 4:
            y++;
            break;
        default:
            break;
    }

    // Wall collision (uncomment below to enable wall-less gameplay)
    if (x < 0) x = WIDTH - 1; else if (x >= WIDTH) x = 0;
    if (y < 0) y = HEIGHT - 1; else if (y >= HEIGHT) y = 0;

    // Self-collision
    for (i = 0; i < tailLength; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = 1;
            break;
        }
    }

    // Fruit consumption
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        tailLength++;
        fruitsEaten++;

        if (fruitsEaten % 2 == 0) {
            level++;
            if (delay > 50)
                delay -= 50; // Increase speed
        }
    }
}

/**
 * Ends the game and displays a Game Over message.
 * Waits for the user to press any key, then exits the curses mode and
 * terminates the program.
 */
void EndGame() {
    nodelay(stdscr, FALSE);
    mvprintw(HEIGHT / 2, (WIDTH - 10) / 2, "Game Over!");
    mvprintw(HEIGHT / 2 + 1, (WIDTH - 19) / 2, "Press any key to exit");
    getch();
    endwin();
}

/**
 * Main function to run the Snake game.
 * Initializes the random number generator and game setup.
 * Enters the main game loop, which continues until the game is over.
 * Within the loop, the game is drawn, user input is processed, and game logic is executed.
 * The game speed is controlled through a delay.
 * Ends the game and cleans up before exiting the program.
 */

int main() {
    srand(time(0)); // Initialize random number generator
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(delay * 1000); // Control speed
    }

    EndGame();
    return 0;
}