#ifndef _GAME_H_
#define _GAME_H_

#include "Animator.h"
#include "Block.h"
#include "ClockGUI.h"
#include "WindowManager.h"

#define G_MOVE_PER 10

class Game {
   public:
    double size = 300.0;
    bool start = false;
    Block** blocks;

    Game(WindowManager* wm) {
        m_wm = wm;
        backAni = new Animator(0.0, size, 100);
        backAni->mode = A_END_V0;
        backRoutateAni = new Animator(0.0, M_PI * 2, 100);
        backRoutateAni->mode = A_END_V0;
        barColorAni = new Animator(200.0, 0.0, 100);
    }

    void init() {
        for (int i = 0; i < BLOCKS * 3; i++) {
            blocks[i]->setAlive();
        }
    }

    void draw() {
        m_wm->drawMode = CENTER;
        if (start) {
            drawBack();
            for (int i = 0; i < BLOCKS * 3; i++) {
                blocks[i]->draw();
            }
            if (backAni->played) {
                drawBar();
                drawBall();
            }
            if (gameStarted) {
                checkWall();
            }
            cout << angle << endl;
        }
    }

    void drawBack() {
        glBegin(GL_POLYGON);
        //rgb(250, 250, 0)
        glColor3ub(250, 250, 250);
        double ani = backAni->play();
        double routateAni = backRoutateAni->play();
        m_wm->drawMode = CENTER;
        for (int i = 0; i < 4; i++) {
            glVertex2i(m_wm->px(0 + ani * cos(2 * M_PI * i / 4 + M_PI / 4 /* + routateAni */)), m_wm->py(0 + ani * sin(2 * M_PI * i / 4 + M_PI / 4 /* + routateAni */)));
        }
        glEnd();
        squareActualSize = ani * cos(2 * M_PI * 4 / 4 + M_PI / 4 /* + routateAni */);
    }

    void drawBar() {
        glBegin(GL_POINTS);
        glColor3ub(barColorAni->play(), barColorAni->play(), 0.0);
        glPointSize(5.0);
        m_wm->drawLine(
            (double)m_wm->px(bar_x - barWidth / 2),
            m_wm->py(bar_y),
            (double)m_wm->px(bar_x + barWidth / 2),
            m_wm->py(bar_y));
        glEnd();
    }

    void drawBall() {
        if (gameStarted) {
            ball_x += cos(angle) * 2.0;
            ball_y += sin(angle) * 2.0;
        }
        glBegin(GL_POINTS);
        glColor3ub(255.0, 0.0, 0.0);
        glPointSize(10.0);
        glVertex2i(m_wm->px((int)ball_x), m_wm->py((int)ball_y));
        glEnd();
    }

    void checkWall() {
        if (ball_x > squareActualSize) {
            cout << "a" << endl;
            ballTurnLeft();
            ball_x -= 10;
        }
        if (ball_x < squareActualSize * -1) {
            cout << "a" << endl;
            ballTurnRight();
            ball_x += 10;
        }
        if (ball_y > squareActualSize) {
            /* angle > M_PI / 2 && angle < (M_PI * 3 / 2) ? ballTurnPlus() : ballTurnMinus();
            ball_y -= 10; */
            gameStarted = false;
            barWidth -= 10;
            ball_x = bar_x;
            ball_y = 195;
        }
        if (ball_y < squareActualSize * -1) {
            (angle > M_PI / 2 && angle < M_PI * 3 / 2) ? ballTurnMinus() : ballTurnPlus();
            ball_y += 10;
        }
        for (int i = 0; i < BLOCKS * 3; i++) {
            if (blocks[i]->getAlive()) {
                bool result = blocks[i]->check(m_wm->px(ball_x), m_wm->py(ball_y));
                if (result) {
                    blocks[i]->setDead();
                    rand() % 2 == 0 && angle < (M_PI * 3 / 2) ? ballTurnPlus() : ballTurnMinus();
                    if(blocks[i]->m_r > 1.0){
                        //高得点
                        barColorAni->reset();
                        barWidth += 50;
                    }
                }
            }
        }
        if(ball_y > bar_y - 5 && ball_y < bar_y + 5 && ball_x > bar_x - (barWidth / 2) && ball_x < bar_x + (barWidth / 2)){
            cout << "HIT" << endl;
            angle > M_PI / 2 && angle < (M_PI * 3 / 2) ? ballTurnPlus() : ballTurnMinus();
            ball_y -= 10;
        }
    }

    void leftArrow() {
        cout << abs(bar_x) + (barWidth / 2) + G_MOVE_PER << endl;
        if (bar_x - (barWidth / 2) - G_MOVE_PER <= (int)squareActualSize * -1) {
            return;
        }
        bar_x -= G_MOVE_PER;
        if (!gameStarted) {
            ball_x -= G_MOVE_PER;
        }
    }

    void rightArrow() {
        if (bar_x + (barWidth / 2) + G_MOVE_PER >= (int)squareActualSize) {
            return;
        }
        bar_x += G_MOVE_PER;
        if (!gameStarted) {
            ball_x += G_MOVE_PER;
        }
    }

    void space() {
        if (!gameStarted) {
            srand(time(NULL));
            angle = ((double)(rand() % (int)(M_PI * 10000)) / 2.0 / 10000.0) * -1.0 - (M_PI / 4);
            cout << angle << endl;
            gameStarted = true;
        }
    }

    void ballTurnRight() {
        angle > M_PI ? ballTurnPlus() : ballTurnMinus();
    }

    void ballTurnLeft() {
        angle < M_PI ? ballTurnPlus() : ballTurnMinus();
    }

    void ballTurnPlus() {
        angle = (angle + ((double)(rand() % (int)(M_PI * 100)) / 100.0 / 4.0) + M_PI * 2.0 / 8.0);
        if (angle > M_PI * 2) {
            angle -= M_PI * 2;
        }
        cout << angle << endl;
    }
    void ballTurnMinus() {
        angle = (angle - ((double)(rand() % (int)(M_PI * 100)) / 100.0 / 4.0) - M_PI * 2.0 / 8.0);
        if (angle < 0) {
            angle += M_PI * 2;
        }
        cout << angle << endl;
    }

   private:
    double angle = 0.0;
    WindowManager* m_wm;
    Animator* backAni;
    Animator* backRoutateAni;
    Animator* barColorAni;
    bool gameStarted = false;
    int bar_x = 0;
    int bar_y = 200;
    int barWidth = 100;
    double ball_x = 0.0;
    double ball_y = 195.0;
    int squareActualSize = 0;
};

#endif  //_GAME_H_