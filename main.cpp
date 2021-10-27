#include <GL/glut.h>
#include <string.h>

#define FRAME 5

#include "ClockGUI.h"
#include "WindowManager.h"
#include "Animator.h"
using namespace std;

WindowManager* windowManager;
ClockGUI* clockgui;
Animator* backR;
Animator* backG;
Animator* backB;

//WindowManager* bufWindowManager;

void Display(void);
void Loop(int value);

int main(int argc, char** argv) {
    windowManager = new WindowManager(argc, argv);
    //windowManager->bufWindowManager = windowManager;
    WindowManager::setBufWindowManager(windowManager);
    windowManager->setWindowTitle("Clock");
    windowManager->setWindowSize(640, 480);
    windowManager->InitDisplay(argc, argv);
    glutDisplayFunc(Display);
    glutTimerFunc(FRAME, Loop, 0);
    clockgui = new ClockGUI(windowManager);
    clockgui->size = 200.0;
    clockgui->init();
    backR = new Animator(0, 00, 500);
    backG = new Animator(0, 100, 500);
    backB = new Animator(0, 100, 500);
    windowManager->Start();
    //glutMainLoop();

    return 0;
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    //glFlush();
}

void Loop(int value) {
    windowManager->clearWithColor(backR->play(), backG->play(), backB->play());
    clockgui->draw();
    glFlush();
    glutTimerFunc(FRAME, Loop, 0);
}