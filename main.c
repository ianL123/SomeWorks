#include <stdio.h>
#include "graphics.h"

#define numMark 3
#define numBlock 3
#define movePeriod 100

struct position
{
    int x;
    int y;
    int direction; // 1=right,2=down,3=left,4=up. I'll give blocks with no direction any number like 0.
};
struct position robot = {2, 3, 1};
struct position home = {2, 3, 0};
int grid[9][9]; // 1=mark, 2=block
struct position marks[numMark] = {{5, 8, 0}, {1, 3, 0}, {6, 1, 0}};
int markleft = numMark;
struct position block[numBlock] = {{4, 3, 0}, {3, 5, 0}, {6, 7, 0}};
int movementRecord[100]; // 1=right(left to reverse),2=left(right to reverse),3=forward
int recordIndex = 0;
int isCarryingMarker = 0;

void homeEqualRobotCheck()
{
    home.x = robot.x;
    home.y = robot.y;
}

void initializedTheGrid()
{
    for (int i = 0; i < 80; i++)
    {
        grid[i / 9][i % 9] = 0;
    }
    for (int i = 0; i < numMark; i++)
    {
        grid[marks[i].x][marks[i].y] = 1;
    }
    for (int i = 0; i < numBlock; i++)
    {
        grid[block[i].x][block[i].y] = 2;
    }
}

void paintBackground()
{
    background();
    paintGrid();
    drawTheHome();
    drawTheMarker();
    drawTheBlock();
}

void paintGrid() // 8*8 grid from 50 to 450 px
{
    background();
    drawRect(50, 50, 400, 400);
    drawLine(50, 100, 450, 100);
    drawLine(50, 150, 450, 150);
    drawLine(50, 200, 450, 200);
    drawLine(50, 250, 450, 250);
    drawLine(50, 300, 450, 300);
    drawLine(50, 350, 450, 350);
    drawLine(50, 400, 450, 400);
    drawLine(100, 50, 100, 450);
    drawLine(150, 50, 150, 450);
    drawLine(200, 50, 200, 450);
    drawLine(250, 50, 250, 450);
    drawLine(300, 50, 300, 450);
    drawLine(350, 50, 350, 450);
    drawLine(400, 50, 400, 450);
}

void drawTheHome()
{
    background();
    setColour(blue);
    fillRect(50 * robot.x, 50 * robot.y, 50, 50);
}

void drawTheMarker()
{
    background();
    setColour(gray);
    for (int i = 0; i < numMark; i++)
    {
        fillRect(50 * marks[i].x, 50 * marks[i].y, 50, 50);
    }
}

void drawTheBlock()
{
    background();
    setColour(black);
    for (int i = 0; i < numBlock; i++)
    {
        fillRect(50 * block[i].x, 50 * block[i].y, 50, 50);
    }
}

void findingTheMarkers()
{
    while (markleft != 0)
    {
        while (canMoveForward() && markleft != 0)
        {
            sleep(movePeriod);
            fTM_Forward(); // Specially used for this time. It also check if robot is stepping onto a marker.
        }
        while (!canMoveForward())
        {
            right();
            movementRecord[recordIndex] = 1;
            recordIndex++;
        }
    }
}

void fTM_Forward()
{
    if (isCarryingMarker == 1)
    {
        takeAwayTheMarker();
    }
    forward();
    movementRecord[recordIndex] = 3;
    recordIndex++;
    drawTheRobot();
    if (isCarryingMarker == 1)
    {
        carryingTheMarker();
    }
    if (atMarker())
    {
        isCarryingMarker = 1;
        markleft--;
    }
}

void goBackToHome()
{
    right();
    right();
    for (int i = recordIndex - 1; i > -1; i--)
    {
        if (movementRecord[i] == 1)
        {
            left();
        }
        else if (movementRecord[i] == 3)
        {
            takeAwayTheMarker();
            forward();
            drawTheRobot();
            carryingTheMarker();
            sleep(movePeriod);
        }
        else if (movementRecord[i] == 2)
        {
            right();
        }
    }
}

void drawTheRobot()
{
    foreground();
    clear();
    drawOval(50 * robot.x + 5, 50 * robot.y + 5, 40, 40);
}

void forward()
{

    if (robot.direction == 1)
    {
        robot.x += 1;
    }
    else if (robot.direction == 2)
    {
        robot.y += 1;
    }
    else if (robot.direction == 3)
    {
        robot.x -= 1;
    }
    else
    {
        robot.y -= 1;
    }
}

void left()
{
    if (robot.direction != 1)
    {
        robot.direction -= 1;
    }
    else
    {
        robot.direction = 4;
    }
}

void right()
{
    if (robot.direction != 4)
    {
        robot.direction += 1;
    }
    else
    {
        robot.direction = 1;
    }
}

int atMarker()
{
    return grid[robot.x][robot.y] == 1;
}

void takeAwayTheMarker()
{
    background();
    setColour(white);
    fillRect(50 * robot.x, 50 * robot.y, 50, 50);
    setColour(black);
    drawRect(50 * robot.x, 50 * robot.y, 50, 50);
    grid[robot.x][robot.y] = 0;
}

void carryingTheMarker()
{
    background();
    setColour(gray);
    fillRect(50 * robot.x, 50 * robot.y, 50, 50);
}

int canMoveForward()
{
    return notEdge() && !blockforward();
}

int notEdge()
{
    if (robot.direction == 1 && robot.x < 8)
    {
        return 1;
    }
    else if (robot.direction == 2 && robot.y < 8)
    {
        return 1;
    }
    else if (robot.direction == 3 && robot.x > 1)
    {
        return 1;
    }
    else if (robot.direction == 4 && robot.y > 1)
    {
        return 1;
    }
    return 0;
}

int blockforward()
{
    if (robot.direction == 1 && grid[robot.x + 1][robot.y] == 2)
    {
        return 1;
    }
    else if (robot.direction == 2 && grid[robot.x][robot.y + 1] == 2)
    {
        return 1;
    }
    else if (robot.direction == 3 && grid[robot.x - 1][robot.y] == 2)
    {
        return 1;
    }
    else if (robot.direction == 4 && grid[robot.x][robot.y - 1] == 2)
    {
        return 1;
    }
    return 0;
}

void main()
{
    homeEqualRobotCheck();
    setWindowSize(500, 500);
    initializedTheGrid();
    paintBackground();
    sleep(300);
    drawTheRobot();
    findingTheMarkers();
    sleep(300);
    goBackToHome();
}