
#include "WindowManager.h"

#include <iostream>

#include "GL/glut.h"

using namespace std;

WindowManager* WindowManager::bufWindowManager;

void WindowManager::InitDisplay(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(m_windowW, m_windowH);
    glutCreateWindow(m_windowTitle);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
    //glutReshapeFunc(Reshape);
    glutReshapeFunc(ReshapeHandler);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void WindowManager::Start() {
    cout << "Let's go" << endl;
    glutMainLoop();
}

void WindowManager::ReshapeHandler(int w, int h) {
    cout << "ウィンドウの幅と高さ w:" << w << " h:" << h << endl;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
    bufWindowManager->m_windowH = h;
    bufWindowManager->m_windowW = w;
}

void WindowManager::setWindowTitle(const char* windowTitle) {
    m_windowTitle = windowTitle;
}

void WindowManager::setWindowSize(int w, int h) {
    m_windowW = w;
    m_windowH = h;
    if (m_windowInitialized) {
        glutReshapeWindow(w, h);
    }
}

void WindowManager::setBufWindowManager(WindowManager* windowManager) {
    bufWindowManager = windowManager;
}

WindowManager* WindowManager::getBufWindowManager() {
    return bufWindowManager;
}

int WindowManager::px(int x) {
    return pos('x', x);
}

int WindowManager::py(int y) {
    return pos('y', y);
}

int WindowManager::pos(char mode, int p) {
    int buf = p;
    switch (drawMode) {
        case CENTER:
            buf = ((mode == 'x' ? m_windowW : m_windowH) / 2) + p;
    }
    return toReadPos(mode, buf);
}

int WindowManager::toReadPos(char mode, int p) {
    //TODO: exchange to read pos
    return p;
}