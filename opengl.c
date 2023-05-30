#include "header.h"
#include "functions.h"
#include "constants.h"


float dotX = -0.6; // Initial X position of the moving dot
float dotY = 0.0; // Initial Y position of the moving dot

void drawSquare(float x, float y, float size, float r, float g, float b) {
    glBegin(GL_QUADS);
    glColor3f(r, g, b);                   // Set color
    glVertex2f(x - size / 2, y + size / 2);  // Top-left vertex
    glVertex2f(x - size / 2, y - size / 2);  // Bottom-left vertex
    glVertex2f(x + size / 2, y - size / 2);  // Bottom-right vertex
    glVertex2f(x + size / 2, y + size / 2);  // Top-right vertex
    glEnd();
}

void drawCircle(float cx, float cy, float radius, int numSegments) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0, 0.0, 0.0); // Set color to black
    glVertex2f(cx, cy); // Center of the circle
    
    float angle;
    for (int i = 0; i <= numSegments; i++) {
        angle = i * 2.0 * M_PI / numSegments;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    
    glEnd();
}

void drawDottedLine(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glPushAttrib(GL_ENABLE_BIT); // Save current state
    
    glEnable(GL_LINE_STIPPLE); // Enable line stipple
    glLineStipple(1, 0xAAAA); // Set stipple pattern (dotted line)
    
    glLineWidth(3.0); // Set line width to make it bolder
    
    glBegin(GL_LINES);
    glColor3f(r, g, b); // Set color (white)
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    
    glPopAttrib(); // Restore previous state
}

void drawDot(float x, float y, float size, float r, float g, float b) {
    glBegin(GL_QUADS);
    glColor3f(r, g, b);                   // Set color
    glVertex2f(x - size / 2, y + size / 2);  // Top-left vertex
    glVertex2f(x - size / 2, y - size / 2);  // Bottom-left vertex
    glVertex2f(x + size / 2, y - size / 2);  // Bottom-right vertex
    glVertex2f(x + size / 2, y + size / 2);  // Top-right vertex
    glEnd();
}

void display() {
    glClearColor(0.5, 0.5, 0.5, 1.0);  // Set window background to gray
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw the four squares with different colors
    drawSquare(0.0, 0.6, 0.2, 1.0, 0.0, 0.0);   // Top square (red)
    drawSquare(0.6, 0.0, 0.2, 0.0, 1.0, 0.0);   // Right square (green)
    drawSquare(0.0, -0.6, 0.2, 0.0, 0.0, 1.0);  // Bottom square (blue)
    drawSquare(-0.6, 0.0, 0.2, 1.0, 1.0, 0.0);  // Left square (yellow)
    
    // Draw the black circle in the middle
    drawCircle(0.0, 0.0, 0.2, 100);
    
    // Draw the dotted lines
    drawDottedLine(-0.6, 0.0, 0.0, 0.6, 1.0, 1.0, 1.0); // Yellow to red
    drawDottedLine(-0.6, 0.0, 0.0, -0.6, 1.0, 1.0, 1.0); // Yellow to blue
    drawDottedLine(0.0, 0.6, 0.6, 0.0, 1.0, 1.0, 1.0); // Red to green
    drawDottedLine(0.0, -0.6, 0.6, 0.0, 1.0, 1.0, 1.0); // Blue to green
    
    // Draw the moving dot
    drawDot(dotX, dotY, 0.05, 0.0, 0.0, 0.0); // Dot color is black
    
    // Set text color to white
    glColor3f(1.0, 1.0, 1.0);
    
    // Set the text font
    void* font = GLUT_BITMAP_9_BY_15;
    
    // Get the width of the text
    const unsigned char* text = (const unsigned char*)"shared memory";
    int textWidth = glutBitmapLength(font, text);
    
    // Set text position at the center of the circle
    float textX = -textWidth / 2.0 / 1000.0;
    float textY = -0.01;
    
    // Render the text "shared memory" inside the circle
    glRasterPos2f(textX, textY);
    glutBitmapString(font, text);

    glFlush();
}

void update(int value) {
    // Update the position of the dot along the dotted line
    dotX += 0.001; // Adjust the speed of the dot movement
    dotY += 0.001; // Adjust the speed of the dot movement
    
    // Check if the dot has reached the end of the dotted line
    if (dotX >= 0.0 && dotY >= 0.6) {
        dotX = 0.0; // Reset the dot position to the starting point
        dotY = 0.6; // Reset the dot position to the starting point
    }
    
    
    glutPostRedisplay(); // Notify GLUT that the display needs to be updated
    glutTimerFunc(10, update, 0); // Call update function after 10 milliseconds
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL Squares and Circle");
    glutInitWindowSize(800, 800);
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0); // Call update function immediately
    glutMainLoop();
    return 0;
}