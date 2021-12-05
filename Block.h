#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <GL/glut.h>

#include "Animator.h"

class Block {
   public:
    int m_r = 0;
    int m_g = 0;
    int m_b = 0;
    double size = 15.0;

    Block(int x, int y, int r, int g, int b) {
        m_x = x;
        m_y = y;
        m_r = r;
        m_g = g;
        m_b = b;
        sizeAni = new Animator(0.0, size, 20);
        sizeAni->mode = A_BEGIN_V0;
    }

    void setAlive() {
        delete sizeAni;
        sizeAni = new Animator(0.0, size, 20);
        sizeAni->mode = A_BEGIN_V0;
        alive = true;
    }

    void setDead() {
        delete sizeAni;
        sizeAni = new Animator(size, 0.0, 20);
        sizeAni->mode = A_BEGIN_V0;
        alive = false;
    }

    bool getAlive() {
        return alive;
    }

    void draw() {
        glBegin(GL_QUADS);
        glColor3ub(m_r, m_g, m_b);
        double size = sizeAni->play();
        glVertex2i(m_x - size / 2, m_y - size / 2);
        glVertex2i(m_x + size / 2, m_y - size / 2);
        glVertex2i(m_x + size / 2, m_y + size / 2);
        glVertex2i(m_x - size / 2, m_y + size / 2);
        glEnd();
    }

    bool check(int x, int y) {
        return x > (m_x - size / 2) && x < (m_x + size / 2) && y > (m_y - size / 2) && y < (m_y + size / 2);
    }

   private:
    bool alive = true;
    int m_x = 0;
    int m_y = 0;
    Animator* sizeAni;
};

#endif  //_BLOCK_H_