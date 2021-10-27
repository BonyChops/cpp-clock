#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_
#include <GL/glpng.h>
#include <GL/glut.h>
#include <math.h>

#include "Animator.h"

#define NORMAL 0
#define CENTER 1
using namespace std;

class WindowManager {
   public:
    WindowManager(int argc, char** argv) {
        //InitDisplay(argc, argv);
    }
    static void ReshapeHandler(int w, int h);
    static WindowManager* bufWindowManager;
    //static int bufWindowManager;
    static int sint;
    static void setBufWindowManager(WindowManager* windowManager);
    static WindowManager* getBufWindowManager();
    int drawMode = 0;
    void InitDisplay(int argc, char** argv);
    void setWindowTitle(const char* windowTitle);
    void setWindowSize(int w, int h);
    void Start();
    int px(int x);
    int py(int y);
    int toReadPosX(int x);
    int toReadPosY(int y);
    void clearWithColor(int r, int g, int b) {
        glBegin(GL_QUADS);
        glColor3ub(r, g, b);
        glVertex2i(0, 0);
        glVertex2i(m_windowW, 0);
        glVertex2i(m_windowW, m_windowH);
        glVertex2i(0, m_windowH);
        glEnd();
    }
    void drawLine(double x1, double y1, double x2, double y2) {
        int length = (int)sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        Animator xLiner(x1, x2, length);
        Animator yLiner(y1, y2, length);
        for (int i = 0; i < length; i++) {
            glVertex2i((int)xLiner.play(), (int)yLiner.play());
        }
    }

   private:
    int pos(char mode, int p);
    int toReadPos(char mode, int p);
    const char* m_windowTitle;
    bool m_windowInitialized = false;
    int m_windowW;
    int m_windowH;
};

#endif  //_WINDOW_MANAGER_H