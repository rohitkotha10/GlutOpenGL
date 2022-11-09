#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

#include <GL/freeglut.h>
#include <iostream>
#include <vector>

using namespace std;

void draw_pixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void draw_line(int x1, int y1, int x2, int y2) {
    int option = 0;
    int quad = 0;
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    if (y1 < y2) {    // 1 and 3 quadrant
        if (x2 > y2)  // 1 and 5 octant
            option = 0;
        else if (y2 > x2) {  // 2 and 6 octant
            option = 1;
            swap(x1, y1);
            swap(x2, y2);
        }
    }

    if (y2 < y1) {  // 2 and 4 quadrant
        y1 = -y1;
        y2 = -y2;
        if (x1 > y1) {
            option = 2;  // 3 and 7 octant
            swap(x1, y1);
            swap(x2, y2);
        } else if (y1 > x1) {
            option = 3;
        }
    }
    vector<pair<int, int>> coords;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = 2 * dy - dx;
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);
    int x = x1;
    int y = y1;
    coords.emplace_back(x, y);

    while (x < x2) {
        if (d <= 0) {
            d = d + incrE;
        }

        else {
            d = d + incrNE;
            y++;
        }
        x++;
        coords.emplace_back(x, y);
    }
    for (auto i: coords) {
        if (option == 0)
            draw_pixel(i.first, i.second);
        else if (option == 1)
            draw_pixel(i.second, i.first);
        else if (option == 2)
            draw_pixel(i.second, -i.first);
        else if (option == 3)
            draw_pixel(i.first, -i.second);
    }
}

void draw_8way(int x, int y, vector<pair<int, int>>& coords) {
    coords.emplace_back(x, y);
    coords.emplace_back(-x, y);
    coords.emplace_back(x, -y);
    coords.emplace_back(-x, -y);
    if (x != y) {
        coords.emplace_back(y, x);
        coords.emplace_back(-y, x);
        coords.emplace_back(y, -x);
        coords.emplace_back(-y, -x);
    }
}

void draw_circle(int radius, int x1, int y1) {
    vector<pair<int, int>> coords;
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    int deltaE = 3;
    int deltaSE = -2 * radius + 5;
    draw_8way(x, y, coords);

    while (y > x) {
        if (d < 0) {
            d += deltaE;
            deltaE += 2;
            deltaSE += 2;
        } else {
            d += deltaSE;
            deltaE += 2;
            deltaSE += 4;
            y--;
        }
        x++;
        draw_8way(x, y, coords);
    }

    for (auto i: coords) { draw_pixel(i.first + x1, i.second + y1); }
}

void draw_4way(int x, int y, vector<pair<int, int>>& coords) {
    coords.emplace_back(x, y);
    coords.emplace_back(-x, y);
    coords.emplace_back(x, -y);
    coords.emplace_back(-x, -y);
}

void draw_ellipse(int a, int b, int x1, int y1) {
    vector<pair<int, int>> coords;
    double d2;
    int x = 0;
    int y = b;
    double dl = (b * b) - (a * a * b) + (0.25 * a * a);
    draw_4way(x, y, coords);
    while ((a * a) * (y - 0.5) > (b * b) * (x + 1)) {
        if (dl < 0)
            dl += b * b * (2 * x + 3);
        else {
            dl += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            y--;
        }
        x++;
        draw_4way(x, y, coords);
    }

    d2 = ((b * b) * (x + 0.5) * (x + 0.5)) + (a * a * (y - 1) * (y - 1)) - (a * a * b * b);
    while (y > 0) {
        if (d2 < 0) {
            d2 += (b * b * (2 * x + 2)) + ((a * a) * (-2 * y + 3));
            x++;
        } else
            d2 += ((a * a) * (-2 * y + 3));
        y--;
        draw_4way(x, y, coords);
    }

    for (auto i: coords) { draw_pixel(i.first + x1, i.second + y1); }
}

void myDisplay() {
    draw_line(0, 0, 600, 500);
    draw_line(400, 700, 300, 200);
    draw_circle(50, 640, 360);
    draw_ellipse(100, 50, 640, 360);
    glFlush();
}

int main(int argc, char** argv) {
    int scr_width = 1280;
    int scr_height = 720;
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(scr_width, scr_height);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Shot 6A");

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, scr_width, 0, scr_height);

    glutDisplayFunc(myDisplay);
    glutMainLoop();
}
