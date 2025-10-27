#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MODE_LINE_BASIC 1
#define MODE_LINE_BOLD 2
#define MODE_CIRCLE_SET 3
#define MODE_LIANG_CLIP 4

int activeMode = MODE_LINE_BASIC;
int thicknessLevel = 3;

// Task 1 variables - renamed
int startX, startY, endX, endY;

// Task 3 variables (Liang-Barsky)
#define LINE_STORAGE 10
typedef struct {
    int fromX, fromY, toX, toY;
} Segment;
Segment segmentArray[LINE_STORAGE];
int segmentCount = 0;
int clipXmin = 100, clipYmin = 100, clipXmax = 400, clipYmax = 400;

void initializeGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

void render_pixel(int px, int py) {
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();
}

// Task 1a: Standard Bresenham's Line Drawing (renamed variables)
void render_line(int x_start, int y_start, int x_end, int y_end) {
    int deltaX, deltaY, step, error;
    int stepX, stepY, errInc1, errInc2;
    int currX, currY;

    deltaX = x_end - x_start;
    deltaY = y_end - y_start;

    if (deltaX < 0) deltaX = -deltaX;
    if (deltaY < 0) deltaY = -deltaY;
    stepX = 1;
    if (x_end < x_start) stepX = -1;
    stepY = 1;
    if (y_end < y_start) stepY = -1;
    currX = x_start;
    currY = y_start;

    if (deltaX > deltaY) {
        render_pixel(currX, currY);
        error = 2 * deltaY - deltaX;
        errInc1 = 2 * (deltaY - deltaX);
        errInc2 = 2 * deltaY;
        for (step = 0; step < deltaX; step++) {
            if (error >= 0) {
                currY += stepY;
                error += errInc1;
            } else {
                error += errInc2;
            }
            currX += stepX;
            render_pixel(currX, currY);
        }
    } else {
        render_pixel(currX, currY);
        error = 2 * deltaX - deltaY;
        errInc1 = 2 * (deltaX - deltaY);
        errInc2 = 2 * deltaX;
        for (step = 0; step < deltaY; step++) {
            if (error >= 0) {
                currX += stepX;
                error += errInc1;
            } else {
                error += errInc2;
            }
            currY += stepY;
            render_pixel(currX, currY);
        }
    }
}

// Task 1b: Thick Line Drawing using 8-way symmetry (renamed)
void render_bold_line(int x_start, int y_start, int x_end, int y_end, int boldness) {
    int deltaX = x_end - x_start;
    int deltaY = y_end - y_start;
    double len = sqrt((double)(deltaX * deltaX + deltaY * deltaY));

    if (len == 0) {
        // Draw a bold point
        for (int i = -boldness/2; i <= boldness/2; i++) {
            for (int j = -boldness/2; j <= boldness/2; j++) {
                render_pixel(x_start + i, y_start + j);
            }
        }
        return;
    }

    // Calculate perpendicular offset
    double offsetX = -deltaY / len;
    double offsetY = deltaX / len;

    // Draw multiple parallel lines for thickness
    for (int t = -boldness/2; t <= boldness/2; t++) {
        int shiftedX = (int)(offsetX * t);
        int shiftedY = (int)(offsetY * t);
        render_line(x_start + shiftedX, y_start + shiftedY, x_end + shiftedX, y_end + shiftedY);
    }
}

// Task 2: Midpoint Circle Drawing Algorithm (renamed)
void render_circle_points(int centerX, int centerY, int offsetX, int offsetY) {
    render_pixel(centerX + offsetX, centerY + offsetY);
    render_pixel(centerX - offsetX, centerY + offsetY);
    render_pixel(centerX + offsetX, centerY - offsetY);
    render_pixel(centerX - offsetX, centerY - offsetY);
    render_pixel(centerX + offsetY, centerY + offsetX);
    render_pixel(centerX - offsetY, centerY + offsetX);
    render_pixel(centerX + offsetY, centerY - offsetX);
    render_pixel(centerX - offsetY, centerY - offsetX);
}

void render_circle(int centerX, int centerY, int rad) {
    int circleX = 0;
    int circleY = rad;
    int p = 1 - rad;

    render_circle_points(centerX, centerY, circleX, circleY);

    while (circleX < circleY) {
        circleX++;
        if (p < 0) {
            p = p + 2 * circleX + 1;
        } else {
            circleY--;
            p = p + 2 * (circleX - circleY) + 1;
        }
        render_circle_points(centerX, centerY, circleX, circleY);
    }
}

void render_bold_circle(int centerX, int centerY, int rad, int boldness) {
    for (int t = 0; t < boldness; t++) {
        render_circle(centerX, centerY, rad + t - boldness/2);
    }
}

// Task 2: Concentric circles with gradient (renamed)
void render_concentric_set() {
    int cx = 250;
    int cy = 250;
    int totalCircles = 10;
    int biggestRadius = 200;

    for (int i = 0; i < totalCircles; i++) {
        int rad = (i + 1) * biggestRadius / totalCircles;
        int boldness = 2 + i / 2;

        // Gradient color from red to blue
        float grad = (float)i / (totalCircles - 1);
        glColor3f(1.0f - grad, 0.2f, grad);

        render_bold_circle(cx, cy, rad, boldness);
    }
}

// Task 3: Liang-Barsky Line Clipping Algorithm (renamed)
int clip_check(float param, float quant, float *minT, float *maxT) {
    float ratio;
    int isValid = 1;

    if (param < 0.0f) {
        ratio = quant / param;
        if (ratio > *maxT) {
            isValid = 0;
        } else if (ratio > *minT) {
            *minT = ratio;
        }
    } else if (param > 0.0f) {
        ratio = quant / param;
        if (ratio < *minT) {
            isValid = 0;
        } else if (ratio < *maxT) {
            *maxT = ratio;
        }
    } else {
        if (quant < 0.0f) {
            isValid = 0;
        }
    }
    return isValid;
}

int liang_clip(float x0, float y0, float x1, float y1,
                float *clippedX0, float *clippedY0, float *clippedX1, float *clippedY1) {
    float deltax = x1 - x0;
    float deltay = y1 - y0;
    float tmin = 0.0f;
    float tmax = 1.0f;

    if (clip_check(-deltax, x0 - clipXmin, &tmin, &tmax)) {
        if (clip_check(deltax, clipXmax - x0, &tmin, &tmax)) {
            if (clip_check(-deltay, y0 - clipYmin, &tmin, &tmax)) {
                if (clip_check(deltay, clipYmax - y0, &tmin, &tmax)) {
                    if (tmax < 1.0f) {
                        *clippedX1 = x0 + tmax * deltax;
                        *clippedY1 = y0 + tmax * deltay;
                    } else {
                        *clippedX1 = x1;
                        *clippedY1 = y1;
                    }

                    if (tmin > 0.0f) {
                        *clippedX0 = x0 + tmin * deltax;
                        *clippedY0 = y0 + tmin * deltay;
                    } else {
                        *clippedX0 = x0;
                        *clippedY0 = y0;
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

void show_clipping_box() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(clipXmin, clipYmin);
    glVertex2i(clipXmax, clipYmin);
    glVertex2i(clipXmax, clipYmax);
    glVertex2i(clipXmin, clipYmax);
    glEnd();
}

void show_clipped_segments() {
    show_clipping_box();

    for (int i = 0; i < segmentCount; i++) {
        float clippedX0, clippedY0, clippedX1, clippedY1;

        // Draw original segment in red
        glColor3f(1.0f, 0.0f, 0.0f);
        render_line(segmentArray[i].fromX, segmentArray[i].fromY, segmentArray[i].toX, segmentArray[i].toY);

        // Clip and draw clipped portion in green
        if (liang_clip((float)segmentArray[i].fromX, (float)segmentArray[i].fromY,
                       (float)segmentArray[i].toX, (float)segmentArray[i].toY,
                       &clippedX0, &clippedY0, &clippedX1, &clippedY1)) {
            glColor3f(0.0f, 1.0f, 0.0f);
            render_bold_line((int)clippedX0, (int)clippedY0, (int)clippedX1, (int)clippedY1, 3);
        }
    }
}

void displayMain() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch(activeMode) {
        case MODE_LINE_BASIC:
            glColor3f(1.0f, 1.0f, 1.0f);
            render_line(startX, startY, endX, endY);
            break;

        case MODE_LINE_BOLD:
            glColor3f(1.0f, 1.0f, 0.0f);
            render_bold_line(startX, startY, endX, endY, thicknessLevel);
            break;

        case MODE_CIRCLE_SET:
            render_concentric_set();
            break;

        case MODE_LIANG_CLIP:
            show_clipped_segments();
            break;
    }

    glFlush();
}

void menuCallback(int selected) {
    activeMode = selected;
    glutPostRedisplay();
}

void keyboardCallback(unsigned char key, int x, int y) {
    switch(key) {
        case '+':
            if (activeMode == MODE_LINE_BOLD) {
                thicknessLevel++;
                glutPostRedisplay();
            }
            break;
        case '-':
            if (activeMode == MODE_LINE_BOLD && thicknessLevel > 1) {
                thicknessLevel--;
                glutPostRedisplay();
            }
            break;
        case 'q':
        case 'Q':
            exit(0);
            break;
    }
}

void setupInitialData() {
    // Default line for Task 1
    startX = 50; startY = 50; endX = 450; endY = 400;

    // Sample lines for Task 3
    segmentCount = 5;
    segmentArray[0].fromX = 50;  segmentArray[0].fromY = 150;  segmentArray[0].toX = 450; segmentArray[0].toY = 350;
    segmentArray[1].fromX = 150; segmentArray[1].fromY = 50;   segmentArray[1].toX = 350; segmentArray[1].toY = 450;
    segmentArray[2].fromX = 200; segmentArray[2].fromY = 200;  segmentArray[2].toX = 300; segmentArray[2].toY = 300;
    segmentArray[3].fromX = 50;  segmentArray[3].fromY = 450;  segmentArray[3].toX = 450; segmentArray[3].toY = 50;
    segmentArray[4].fromX = 10;  segmentArray[4].fromY = 10;   segmentArray[4].toX = 80;  segmentArray[4].toY = 80;
}

int main(int argc, char **argv) {
    printf("Lab Assignment\n");
    printf("================================\n");
    printf("Controls:\n");
    printf("  Right-click for menu to switch tasks\n");
    printf("  +/- keys to adjust line thickness (Task 1b)\n");
    printf("  Q to quit\n\n");

    setupInitialData();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CSE 426");

    initializeGL();

    glutCreateMenu(menuCallback);
    glutAddMenuEntry("Task 1a: Standard Line", MODE_LINE_BASIC);
    glutAddMenuEntry("Task 1b: Thick Line", MODE_LINE_BOLD);
    glutAddMenuEntry("Task 2: Concentric Circles", MODE_CIRCLE_SET);
    glutAddMenuEntry("Task 3: Liang-Barsky Clipping", MODE_LIANG_CLIP);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(displayMain);
    glutKeyboardFunc(keyboardCallback);
    glutMainLoop();

    return 0;
}
