// $Id: hello-world.cpp,v 1.26 2019-05-15 17:13:01-07 - - $

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include <GL/freeglut.h>
//#include <libgen.h>
#include "getopt.h"
#include "graphics.h"
#include "util.h"
#include "interp.h"

// Characteristics of the window.
struct window window;

template <typename item_t>
struct cycle_iterator {
    const vector<item_t> items;
    size_t curr_item{ 0 };
    cycle_iterator(initializer_list<item_t> list) :
        items(vector<item_t>(list)) {}
    const item_t& operator*() { return items[curr_item]; }
    const item_t& operator[] (size_t index) {
        return items[(curr_item + index) % items.size()];
    }
    cycle_iterator& operator++() {
        curr_item = (curr_item + 1) % items.size();
        return *this;
    }
};

cycle_iterator<string> greetings{
   "Hello, World!",
   "Hodie natus est radici frater.",
   "Goodbye, World!",
};

GLubyte RED[] = { 0xFF, 0x00, 0x00 };
GLubyte GREEN[] = { 0x00, 0xFF, 0x00 };
GLubyte BLUE[] = { 0x00, 0x00, 0xFF };
cycle_iterator<GLubyte*> colors{ RED, GREEN, BLUE };

cycle_iterator<void*> glut_fonts{
   GLUT_BITMAP_TIMES_ROMAN_24,
   GLUT_BITMAP_HELVETICA_18,
   GLUT_BITMAP_9_BY_15,
};


//void draw_rectangle(const GLubyte* color) {
//    glBegin(GL_POLYGON);
//    glColor3ubv(color);
//    window.setheight(0);
//    window.setwidth(0);
//    window.main();
//    //glVertex2f(0, 0);
//    //glVertex2f(window.width, 0);
//    //glVertex2f(window.width, window.height);
//    //glVertex2f(0, window.height);
//    glEnd();
//}

void draw_greeting(const string& greeting, const GLubyte* color) {
    const GLubyte* glgreeting =
        reinterpret_cast<const GLubyte*> (greeting.c_str());

    void* font = *glut_fonts;
    int greeting_width = glutBitmapLength(font, glgreeting);
    int greeting_height = glutBitmapHeight(font);

    glColor3ubv(color);
    float width = 100;
    float height = 100;
    float xpos = width / 2.0 - greeting_width / 2.0;
    float ypos = height / 2.0 - greeting_height / 4.0;
    //glRasterPos2f(xpos, ypos);
    window.setwidth(xpos);
    window.setheight(ypos);

    glutBitmapString(font, glgreeting);
}

void display() {
    glClearColor(0.15, 0.15, 0.15, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //draw_rectangle(colors[0]);
    draw_greeting(*greetings, colors[2]);
    text t(GLUT_BITMAP_HELVETICA_10, "Testing");

    glutSwapBuffers();
}


void invert_colors() {
    for (size_t color = 0; color < 3; ++color) {
        for (size_t rgb = 0; rgb < 3; ++rgb) {
            colors[color][rgb] ^= 0xFF;
        }
    }
}

void keyboard(GLubyte key, int, int) {
    enum { BS = 8, LF = 10, CR = 13, ESC = 27, DEL = 127 };
    switch (key) {
    case ' ': case BS: case CR: case LF: case DEL:
        invert_colors();
        break;
    case 'c': case 'C':
        ++colors;
        break;
    case 'f': case 'F':
        ++glut_fonts;
        break;
    case 'g': case 'G':
        ++greetings;
        break;
    case 'q': case 'Q': case ESC:
        exit(EXIT_SUCCESS);
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int, int) {
    if (state == GLUT_DOWN) {
        switch (button) {
        case GLUT_LEFT_BUTTON:
            ++glut_fonts;
            break;
        case GLUT_MIDDLE_BUTTON:
            ++greetings;
            break;
        case GLUT_RIGHT_BUTTON:
            ++colors;
            break;
        }
    }
    glutPostRedisplay();
}


//void reshape(int width, int height) {
//    window.width = width;
//    window.height = height;
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(0, window.width, 0, window.height);
//    glMatrixMode(GL_MODELVIEW);
//    glViewport(0, 0, window.width, window.height);
//    glutPostRedisplay();
//}

void print_howto() {
    cout << R"(
To cycle the colors: right mouse button or key 'c' or 'C'.
To cycle the fonts: left mouse button or key 'f' or 'F'.
To cycle the greetings: middle mouse button or key 'g' or 'G'.
To invert the colors: key Space, Backspace, Return, or Delete.
To quit: key 'q' or 'Q' or ESC.
)";
}

int main(int argc, char** argv) {
    print_howto();
    sys_info::execname(argv[0]);
    interpreter::shape_map shapemap;
    interpreter interp;
    string line = "define r1 equilateral 50";
    interpreter::parameters words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "draw red r1 90 90";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);
    window.main();
    return 0;
}

//TEST// mkpspdf hello-world.cpp.ps hello-world.cpp*
