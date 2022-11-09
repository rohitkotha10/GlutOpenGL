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
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    if (x1 == x2) {
        if (y1 > y2) swap(y1, y2);
        for (int i = y1; i <= y2; i++) draw_pixel(x1, i);
        return;
    } else if (y1 == y2) {
        for (int i = x1; i <= x2; i++) draw_pixel(i, y1);
        return;
    }
    int option = 0;

    if (y1 < y2) {  // positive slope
        if ((y2 - y1) <= (x2 - x1))
            option = 0;
        else if ((y2 - y1) > (x2 - x1)) {
            option = 1;
            swap(x1, y1);
            swap(x2, y2);
        }
    }

    else if (y2 < y1) {  // negative slopes
        y1 = -y1;
        y2 = -y2;
        if ((y2 - y1) > (x2 - x1)) {
            option = 2;
            swap(x1, y1);
            swap(x2, y2);
        } else if ((y2 - y1) <= (x2 - x1)) {
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
        // cout << option << ' ' << x << ' ' << y << endl;
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

void draw_pillar() {
    // pillar lines
    draw_line(700, 360, 700, 720);
    draw_line(715, 360, 715, 720);
    draw_line(730, 360, 730, 720);
    draw_line(760, 360, 760, 720);
    draw_line(790, 360, 790, 720);
    draw_line(830, 360, 830, 720);
    draw_line(870, 360, 870, 720);
    draw_line(885, 360, 885, 720);
    draw_line(900, 360, 900, 720);

    // pillar bases
    draw_line(680, 360, 920, 360);
    draw_line(680, 360, 680, 350);
    draw_line(920, 360, 920, 350);
    draw_line(680, 350, 920, 350);

    draw_line(670, 350, 930, 350);
    draw_line(670, 350, 670, 340);
    draw_line(930, 350, 930, 340);
    draw_line(670, 340, 930, 340);

    draw_line(665, 340, 935, 340);
    draw_line(665, 340, 665, 300);
    draw_line(935, 340, 935, 300);
    draw_line(665, 300, 935, 300);
}

void draw_spear() {
    draw_line(300, 720, 755, 595);
    draw_line(280, 720, 750, 590);
    draw_line(740, 580, 770, 610);

    draw_line(740, 580, 800, 580);
    draw_line(770, 610, 800, 580);
}
void myDisplay() {
    // draw_pillar();
    draw_spear();
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
