#include <GL/glut.h>
#include <string.h>

#define FRAME 20

#include "Animator.h"
#include "ClockGUI.h"
#include "DayCycle.h"
#include "DigitalClockGUI.h"
#include "FutureGUI.h"
#include "Game.h"
#include "WindowManager.h"

using namespace std;

WindowManager* windowManager;
ClockGUI* clockgui;
FutureGUI* futuregui;
DigitalClockGUI* digitalClockGUI;
Game* game;

DayCycle* dayCycle;

Animator* backR;
Animator* backG;
Animator* backB;

bool isDigitalShowed = false;

Block** blocks;

bool playGame = false;

int mode = 0;
int clocks = 2;

//MORNING(7:00)
//rgb(0, 146, 255)

//NOON(12:00)
//rgb(0, 255, 255)

//EVENING(17:00)
//rgb(253, 143, 0)

//NIGHT(24:00)
//rgb(0, 0, 100)

//WindowManager* bufWindowManager;

void Display(void);
void Loop(int value);
void Keyboard(unsigned char, int, int);
void Special(int, int, int);

int main(int argc, char** argv) {
    //windowManager->bufWindowManager = windowManager;
    windowManager = new WindowManager(argc, argv);
    windowManager->setWindowTitle("Clock");
    WindowManager::setBufWindowManager(windowManager);
    windowManager->setWindowSize(640, 480);
    windowManager->InitDisplay(argc, argv);
    glutDisplayFunc(Display);
    glutTimerFunc(FRAME, Loop, 0);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);
    digitalClockGUI = new DigitalClockGUI(windowManager);
    switch (mode) {
        case 0:
            clockgui = new ClockGUI(windowManager);
            clockgui->size = 200.0;
            clockgui->init();
            break;
        case 1:
            futuregui = new FutureGUI(windowManager);
            futuregui->size = 200.0;
            futuregui->init();
            break;
    }
    dayCycle = new DayCycle();
    backR = new Animator(0, dayCycle->getR(), 100);
    backG = new Animator(0, dayCycle->getG(), 100);
    backB = new Animator(0, dayCycle->getB(), 100);
    DayCycle* a = new DayCycle();
    char *GL_version = (char *)glGetString(GL_VERSION);
    char *GL_vendor = (char *)glGetString(GL_VENDOR);
    char *GL_renderer = (char *)glGetString(GL_RENDERER);
    cout << GL_version << endl;
    cout << GL_vendor << endl;
    cout << GL_renderer << endl;
    windowManager->Start();

    //glutMainLoop();    return 0;
}
void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    //glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'g' && mode == 0) {
        if (!playGame) {
            delete game;
            game = new Game(windowManager);
            clockgui->generateBlocks = true;
            clockgui->draw();
            blocks = clockgui->blocks;
            game->blocks = blocks;
            game->start = true;
            playGame = true;
        } else {
            playGame = false;
        }
    } else if (key == ' ') {
        if (playGame) {
            game->space();
        }
    }
    //cout << key << endl;
}

void Special(int key, int x, int y) {
    if (playGame) {
        switch (key) {
            case GLUT_KEY_LEFT:
                game->leftArrow();
                break;
            case GLUT_KEY_RIGHT:
                game->rightArrow();
        }
    } else {
        switch (key) {
            case GLUT_KEY_LEFT:
                mode -= 1;
                if (mode < 0) {
                    mode = clocks - 1;
                }
                break;
            case GLUT_KEY_RIGHT:
                mode += 1;
                if (mode >= clocks) {
                    mode = 0;
                }
                break;
            default:
                return;
        }
        switch (mode) {
            case 0:
                delete clockgui;
                clockgui = new ClockGUI(windowManager);
                clockgui->size = 200.0;
                clockgui->init();
                break;
            case 1:
                delete futuregui;
                futuregui = new FutureGUI(windowManager);
                futuregui->size = 200.0;
                futuregui->init();
                break;
        }
    }
}

void Loop(int value) {
    windowManager->clearWithColor(
        !backR->played ? backR->play() : dayCycle->getR(),
        !backG->played ? backG->play() : dayCycle->getG(),
        !backB->played ? backB->play() : dayCycle->getB());
    switch (mode) {
        case 0:
            clockgui->draw();
            break;
        case 1:
            futuregui->draw();
            break;
    }
    if (playGame) {
        game->draw();
    }
    if ((mode == 0 ? clockgui : futuregui)->backAni->played || isDigitalShowed) {
        digitalClockGUI->draw();
        isDigitalShowed = true;
    }
    /* if (clockgui->backAni->played) {
        digitalClockGUI->draw();
    } */

    glFlush();
    glutSwapBuffers();
    glutTimerFunc(FRAME, Loop, 0);
}