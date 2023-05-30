#include "header.h"

// Window dimensions
const int windowWidth = 1000;
const int windowHeight = 400;
const float alignY = 0;
const float alignX = 0;

float centerX = -0.03 + alignX; // X-coordinate of the center of the circle
float centerY = 0.18 + alignY;  // Y-coordinate of the center of the circle
float radius = 0.03;            // Radius of the circle
int numSegments = 100;
float movementRange = -0.65; // Range of movement for the circle
float movementSpeed = 0.01;  // Speed of the movement
bool moveDown = true;

float spyX = -0.15 + alignX; // X-coordinate of the spy of the circle
float spyY = -0.19 + alignY;
float spyRange = 0.1; // Range of movement for the circle
bool spyMove = true;
float spySpeed = 0.003; // Speed of the movement

float falseX = -0.6 + alignX;
float falseY = 0.15 + alignY;

float trueX = -0.65 + alignX;
float trueY = 0.05 + alignY;

void drawFilledCircle()
{
    // Set the color to blue
    glColor3f(0.1, 0.7, 1.0);
    // Draw the filled circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++)
    {
        float theta = 2.0 * M_PI * i / numSegments;
        float x = centerX + radius * cos(theta);
        float y = centerY + radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}
    void drawTrue()
{
    glColor3f(0.0, 1.0, 0.0); // RGB values for green
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(trueX + 0,trueY +  0);
    glVertex2f(trueX + 0.05,trueY +  0.1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(trueX + 0, trueY -0.003);
    glVertex2f(trueX + -0.01, trueY + 0.05);
    glEnd();
}

void drawFalse()
{
    glColor3f(1.0, 0.0, 0.0); // RGB values for green
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(falseX + 0, falseY + 0);
    glVertex2f(falseX + -0.03,falseY -0.1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(falseX + -0.03, falseY + 0);
    glVertex2f(falseX + 0, falseY - 0.10);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Set the color to black
    glColor3f(0.0, 0.0, 0.0);

    // Draw a rectangle that covers the entire window
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, -1.0);
    glEnd();

    // Set the color to white for the text
    glColor3f(1.0, 1.0, 1.0);

    // Set the position for the text
    glRasterPos2f(-1, 0.9);

    // The text to be displayed
    char text[] = "Project#2: IPC";

    // Loop through each character of the text and display it
    int length = sizeof(text);
    for (int i = 0; i < length; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }

    // Set the color to white for the text
    glColor3f(1.0, 1.0, 1.0);

    // Set the position for the text
    glRasterPos2f(-0.1 + alignX, 0.8 + alignY);

    // The text to be displayed
    char text1[] = "Parent";

    // Loop through each character of the text1 and display it
    for (int i = 0; i < sizeof(text1); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
    }
    

    // Line
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.03 + alignX, 0.47 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.03 + alignX, 0.7 + alignY);  // Ending point (x, y) at the top
    glEnd();

    // Sender Text start

    glRasterPos2f(-0.1 + alignX, 0.3 + alignY);

    // The text to be displayed
    char text2[] = "Sender";

    // Loop through each character of the text2 and display it
    for (int i = 0; i < sizeof(text2); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2[i]);
    }

    glRasterPos2f(-0.11 + alignX, -0.8 + alignY);

    // The text to be displayed
    char text3[] = "Receiver";

    // Loop through each character of the text3 and display it
    for (int i = 0; i < sizeof(text3); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text3[i]);
    }

    glRasterPos2f(-0.4 + alignX, -0.2 + alignY);

    // The text to be displayed
    char text4[] = "Spy Master";

    // Loop through each character of the text4 and display it
    for (int i = 0; i < sizeof(text4); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text4[i]);
    }

    glColor3f(1.0, 0.0, 0.0);

    glRasterPos2f(spyX + alignX, spyY + alignY);
    // The text to be displayed
    char text5[] = "SPY";

    // Loop through each character of the text5 and display it
    for (int i = 0; i < sizeof(text5); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text5[i]);
    }

    // Set the color to green
    drawFalse();

    drawFilledCircle();

    // Lines
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.15 + alignX, -0.76 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.55 + alignX, -0.76 + alignY); // Ending point (x, y) at the top
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.55 + alignX, -0.76 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.55 + alignX, 0.85 + alignY);  // Ending point (x, y) at the top
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.55 + alignX, 0.85 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.15 + alignX, 0.85 + alignY); // Ending point (x, y) at the top
    glEnd();

    glFlush();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void update(int value)
{
    // Update the vertical position of the circle
    if (moveDown)
    {
        centerY -= movementSpeed;
        if (centerY <= movementRange)
            moveDown = false;
    }
    else
    {
        centerY = 0.18 + alignY;
        moveDown = true;
    }

    if (spyMove)
    {
        spyX += spySpeed;
        if (spyX >= spyRange)
            spyMove = false;
    }
    else
    {
        spyX = -0.15 + alignY;
        spyMove = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Update at 60 frames per second
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("OpenGL Black Screen");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();

    return 0;
}
