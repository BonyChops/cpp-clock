#include <GL/glut.h>
#include <string.h>

#define FRAME 5

#include "Animator.h"
#include "ClockGUI.h"
#include "DayCycle.h"
#include "DigitalClockGUI.h"
#include "Game.h"
#include "WindowManager.h"

using namespace std;

WindowManager* windowManager;
ClockGUI* clockgui;
DigitalClockGUI* digitalClockGUI;
Game* game;

DayCycle* dayCycle;

Animator* backR;
Animator* backG;
Animator* backB;

Block** blocks;

bool playGame = false;

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
    clockgui = new ClockGUI(windowManager);
    digitalClockGUI = new DigitalClockGUI(windowManager);
    clockgui->size = 200.0;
    clockgui->init();
    dayCycle = new DayCycle();
    backR = new Animator(0, dayCycle->getR(), 500);
    backG = new Animator(0, dayCycle->getG(), 500);
    backB = new Animator(0, dayCycle->getB(), 500);
    DayCycle* a = new DayCycle();
    windowManager->Start();

    //glutMainLoop();    return 0;
}
void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    //glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'g') {
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
    cout << key << endl;
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
    }
}

void Loop(int value) {
    windowManager->clearWithColor(
        !backR->played ? backR->play() : dayCycle->getR(),
        !backG->played ? backG->play() : dayCycle->getG(),
        !backB->played ? backB->play() : dayCycle->getB());
    clockgui->draw();
    if (playGame) {
        game->draw();
    }
    if (clockgui->backAni->played) {
        digitalClockGUI->draw();
    }

    glFlush();
    glutTimerFunc(FRAME, Loop, 0);
}