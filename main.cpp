#include <GL/glut.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

const int WIDTH = 640;
const int HEIGHT = 480;
const int TILE_SIZE = 20;
int SNAKE_SPEED = 100;

struct SnakePart
{
    int x, y;
};
struct Hurdles
{
    int x, y;
};
struct Food
{
    int x, y;
};

Hurdles hurdle[WIDTH * HEIGHT];
SnakePart snake[WIDTH * HEIGHT];
Food food;
int snakeLength, hurdleLength;
int direction; //0 - Down, 1 - Right, 2 - Up, 3 - Left
int score;
bool gameOver;

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);

    snakeLength = 3;
    direction = 1;
    score = 0;
    gameOver = false;

    hurdleLength = 28;
    hurdle[0] = {0, 0};
    hurdle[1] = {TILE_SIZE, 0};
    hurdle[2] = {0, TILE_SIZE};
    hurdle[3] = {WIDTH - TILE_SIZE, 0};
    hurdle[4] = {WIDTH - TILE_SIZE, TILE_SIZE};
    hurdle[5] = {WIDTH - TILE_SIZE - TILE_SIZE, 0};
    hurdle[6] = {WIDTH - TILE_SIZE, HEIGHT - TILE_SIZE};
    hurdle[7] = {WIDTH - TILE_SIZE, HEIGHT - TILE_SIZE - TILE_SIZE};
    hurdle[8] = {WIDTH - TILE_SIZE - TILE_SIZE, HEIGHT - TILE_SIZE};
    hurdle[9] = {0, HEIGHT - TILE_SIZE};
    hurdle[10] = {TILE_SIZE, HEIGHT - TILE_SIZE};
    hurdle[11] = {0, HEIGHT - TILE_SIZE - TILE_SIZE};
    hurdle[12] = {0, 2 * TILE_SIZE};
    hurdle[13] = {0, 3 * TILE_SIZE};
    hurdle[14] = {2 * TILE_SIZE, 0};
    hurdle[15] = {3 * TILE_SIZE, 0};
    hurdle[16] = {WIDTH - 3 * TILE_SIZE, 0};
    hurdle[17] = {WIDTH - 4 * TILE_SIZE, 0};
    hurdle[18] = {WIDTH - TILE_SIZE, 2 * TILE_SIZE};
    hurdle[19] = {WIDTH - TILE_SIZE, 3 * TILE_SIZE};
    hurdle[20] = {WIDTH - 3 * TILE_SIZE, HEIGHT - TILE_SIZE};
    hurdle[21] = {WIDTH - TILE_SIZE, HEIGHT - 3 * TILE_SIZE};
    hurdle[22] = {WIDTH - 4 * TILE_SIZE, HEIGHT - TILE_SIZE};
    hurdle[23] = {WIDTH - TILE_SIZE, HEIGHT - 4 * TILE_SIZE};
    hurdle[24] = {0, HEIGHT - 3 * TILE_SIZE};
    hurdle[25] = {0, HEIGHT - 4 * TILE_SIZE};
    hurdle[26] = {2 * TILE_SIZE, HEIGHT - TILE_SIZE};
    hurdle[27] = {3 * TILE_SIZE, HEIGHT - TILE_SIZE};
    for (int i = 0; i < 30; i++)
    {
        hurdle[hurdleLength + i] = {WIDTH / 4 + i * TILE_SIZE, HEIGHT / 4};
        hurdleLength++;
    }
    for (int i = 0; i < 30; i++)
    {
        hurdle[hurdleLength + i] = {WIDTH / 4 + i * TILE_SIZE, 3 * HEIGHT / 4};
        hurdleLength++;
    }

    snake[0] = {WIDTH / 2, HEIGHT / 2};
    snake[1] = {WIDTH / 2 - TILE_SIZE, HEIGHT / 2};
    snake[2] = {WIDTH / 2 - 2 * TILE_SIZE, HEIGHT / 2};
    food = {rand() % (WIDTH / TILE_SIZE) * TILE_SIZE, rand() % (HEIGHT / TILE_SIZE) * TILE_SIZE};
    
    // ensures that food is not present on any of the hurdles
    bool f = false;
    while (1)
    {
        for (int i = 0; i < hurdleLength; i++)
        {
            if (food.x == hurdle[i].x && food.y == hurdle[i].y)
            {
                food = {rand() % (WIDTH / TILE_SIZE) * TILE_SIZE, rand() % (HEIGHT / TILE_SIZE) * TILE_SIZE};
                f = true;
                break;
            }
        }
        if (f == false)
        {
            break;
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);                          // set color to white
    glRasterPos2f(WIDTH / 2 - 50, HEIGHT - 450);       // set the position to the top center of the screen
    std::string s = "Score: " + std::to_string(score); // create a string with the score
    
    for (int i = 0; i < s.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]); // display the score
    }

    if (gameOver)
    {
        std::cout << "Game Over! Final Score: " << score << std::endl;
        exit(0);
    }

    // Draw snake
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 0; i < snakeLength; i++)
    {
        glRecti(snake[i].x, snake[i].y, snake[i].x + TILE_SIZE, snake[i].y + TILE_SIZE);
    }
    glColor3f(0.5, 1.0, 0.3);
    for (int i = 0; i < hurdleLength; i++)
    {
        glRecti(hurdle[i].x, hurdle[i].y, hurdle[i].x + TILE_SIZE, hurdle[i].y + TILE_SIZE);
    }
    // Draw food
    glColor3f(1.0, 0.0, 0.0);
    glRecti(food.x, food.y, food.x + TILE_SIZE, food.y + TILE_SIZE);
    glutSwapBuffers(); //to prevent flickering when updating the screen content
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
    if (SNAKE_SPEED > 50)
    {
        SNAKE_SPEED -= 0.00001;
    }

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
        bool f = false;
        while (1)
        {
            for (int i = 0; i < hurdleLength; i++)
            {
                if (food.x == hurdle[i].x && food.y == hurdle[i].y)
                {
                    food = {rand() % (WIDTH / TILE_SIZE) * TILE_SIZE, rand() % (HEIGHT / TILE_SIZE) * TILE_SIZE};
                    f = true;
                    break;
                }
            }
            if (f == false)
            {
                break;
            }
        }
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
    
    // hurdle collision
    for (int i = 0; i < hurdleLength; i++)
    {
        if (snake[0].x == hurdle[i].x && snake[0].y == hurdle[i].y)
        {
            gameOver = true;
            break;
        }
    }
    
    // Wall collision
    if (snake[0].x < 0)
    {
        snake[0].x = WIDTH;
    }
    else if (snake[0].x >= WIDTH)
    {
        snake[0].x = 0;
    }
    else if (snake[0].y < 0)
    {
        snake[0].y = HEIGHT;
    }
    else if (snake[0].y >= HEIGHT)
    {
        snake[0].y = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(SNAKE_SPEED, update, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Snake Game");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp); // Handle key release events
    glutTimerFunc(SNAKE_SPEED, update, 0);
    glutMainLoop();
    return 0;
}
