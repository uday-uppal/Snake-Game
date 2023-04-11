#include <GL/glut.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

const int WIDTH = 640;
const int HEIGHT = 480;
const int TILE_SIZE = 20;
const int SNAKE_SPEED = 100;

struct SnakePart
{
    int x, y;
};

struct Food
{
    int x, y;
};

SnakePart snake[WIDTH * HEIGHT];
int snakeLength;
int direction;
int score;
Food food;
bool gameOver;

void init()
{
    snakeLength = 3;
    direction = 1;
    score = 0;
    gameOver = false;
    snake[0] = {WIDTH / 2, HEIGHT / 2};
    snake[1] = {WIDTH / 2 - TILE_SIZE, HEIGHT / 2};
    snake[2] = {WIDTH / 2 - 2 * TILE_SIZE, HEIGHT / 2};

    food = {rand() % (WIDTH / TILE_SIZE) * TILE_SIZE, rand() % (HEIGHT / TILE_SIZE) * TILE_SIZE};
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver)
    {
        std::cout << "Game Over! Final Score: " << score << std::endl;
        exit(0);
    }

    // Draw snake
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < snakeLength; i++)
    {
        glRecti(snake[i].x, snake[i].y, snake[i].x + TILE_SIZE, snake[i].y + TILE_SIZE);
    }

    // Draw food
    glColor3f(1.0, 0.0, 0.0);
    glRecti(food.x, food.y, food.x + TILE_SIZE, food.y + TILE_SIZE);

    glutSwapBuffers();
}
bool keyStates[256] = {false};

void keyboard(unsigned char key, int x, int y)
{
    keyStates[key] = true;
}

void keyboardUp(unsigned char key, int x, int y)
{
    keyStates[key] = false;
}

void updateDirection()
{
    if (keyStates['s'] && direction != 2)
        direction = 0;
    if (keyStates['d'] && direction != 3)
        direction = 1;
    if (keyStates['w'] && direction != 0)
        direction = 2;
    if (keyStates['a'] && direction != 1)
        direction = 3;
}

void update(int value)
{
    updateDirection();

    for (int i = snakeLength - 1; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }

    if (direction == 0)
        snake[0].y += TILE_SIZE;
    if (direction == 1)
        snake[0].x += TILE_SIZE;
    if (direction == 2)
        snake[0].y -= TILE_SIZE;
    if (direction == 3)
        snake[0].x -= TILE_SIZE;

    // Collision with food
    if (snake[0].x == food.x && snake[0].y == food.y)
    {
        snakeLength++;
        score++;
        food = {rand() % (WIDTH / TILE_SIZE) * TILE_SIZE, rand() % (HEIGHT / TILE_SIZE) * TILE_SIZE};
    }

    // Self-collision
    for (int i = 1; i < snakeLength; i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            gameOver = true;
            break;
        }
    }

    // Wall collision
    if (snake[0].x < 0 || snake[0].x >= WIDTH || snake[0].y < 0 || snake[0].y >= HEIGHT)
    {
        gameOver = true;
    }

    glutPostRedisplay();
    glutTimerFunc(SNAKE_SPEED, update, 0);
}

void display()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Snake Game");

    init();

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp); // Handle key release events
    glutTimerFunc(SNAKE_SPEED, update, 0);

    display();
    glutMainLoop();

    return 0;
}