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
        backAni = new Animator(0.0, size, 20);
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
            //cout << angle << endl;
        }
    }

    void drawBack() {
        m_wm->drawMode = CENTER;

        glColor3ub(250, 250, 250);

        double ani = backAni->play();
        double routateAni = backRoutateAni->play(); // 今はコメントされてたので同じ挙動にするなら使わない
        (void)routateAni;

        // 角度（元コードと同じ：45度オフセット）
        // もし回転アニメを戻すなら + routateAni を足す
        const double base = M_PI / 4.0 /* + routateAni */;

        // 4頂点（px/py に double を渡して安定化）
        double x0 = ani * std::cos(2.0 * M_PI * 0.0 / 4.0 + base);
        double y0 = ani * std::sin(2.0 * M_PI * 0.0 / 4.0 + base);

        double x1 = ani * std::cos(2.0 * M_PI * 1.0 / 4.0 + base);
        double y1 = ani * std::sin(2.0 * M_PI * 1.0 / 4.0 + base);

        double x2 = ani * std::cos(2.0 * M_PI * 2.0 / 4.0 + base);
        double y2 = ani * std::sin(2.0 * M_PI * 2.0 / 4.0 + base);

        double x3 = ani * std::cos(2.0 * M_PI * 3.0 / 4.0 + base);
        double y3 = ani * std::sin(2.0 * M_PI * 3.0 / 4.0 + base);

        // 2三角形: (0,1,2) と (0,2,3)
        float v[12] = {
            (float)m_wm->px(x0), (float)m_wm->py(y0),
            (float)m_wm->px(x1), (float)m_wm->py(y1),
            (float)m_wm->px(x2), (float)m_wm->py(y2),

            (float)m_wm->px(x0), (float)m_wm->py(y0),
            (float)m_wm->px(x2), (float)m_wm->py(y2),
            (float)m_wm->px(x3), (float)m_wm->py(y3),
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, v);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableClientState(GL_VERTEX_ARRAY);

        // 元コードの計算をそのまま再現（※ i=4 は i=0 と同じ角度）
        squareActualSize = ani * std::cos(2.0 * M_PI * 4.0 / 4.0 + base);
    }

    void drawBar() {
        // 色（barColorAni->play() は 0–255 を返す前提）
        GLubyte c = (GLubyte)barColorAni->play();
        glColor3ub(c, c, 0);

        // 線の両端
        float v[4] = {
            (float)m_wm->px(bar_x - barWidth / 2.0),
            (float)m_wm->py(bar_y),

            (float)m_wm->px(bar_x + barWidth / 2.0),
            (float)m_wm->py(bar_y)
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, v);

        // 1本の線
        glDrawArrays(GL_LINES, 0, 2);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void drawBall() {
        if (gameStarted) {
            ball_x += std::cos(angle) * 5.0;
            ball_y += std::sin(angle) * 5.0;
        }

        // 色
        glColor3ub(255, 0, 0);

        // 中心座標（int に潰さずそのまま）
        double cx = m_wm->px(ball_x);
        double cy = m_wm->py(ball_y);

        const double half = 5.0; // pointSize 10 相当

        // 四角形（2三角形）
        float v[12] = {
            (float)(cx - half), (float)(cy - half),
            (float)(cx + half), (float)(cy - half),
            (float)(cx + half), (float)(cy + half),

            (float)(cx - half), (float)(cy - half),
            (float)(cx + half), (float)(cy + half),
            (float)(cx - half), (float)(cy + half),
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, v);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void checkWall() {
        if (ball_x > squareActualSize) {
            //cout << "a" << endl;
            ballTurnLeft();
            ball_x -= 10;
        }
        if (ball_x < squareActualSize * -1) {
            //cout << "a" << endl;
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
            //cout << "HIT" << endl;
            angle > M_PI / 2 && angle < (M_PI * 3 / 2) ? ballTurnPlus() : ballTurnMinus();
            ball_y -= 10;
        }
    }

    void leftArrow() {
        //cout << abs(bar_x) + (barWidth / 2) + G_MOVE_PER << endl;
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
            //cout << angle << endl;
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
        //cout << angle << endl;
    }
    void ballTurnMinus() {
        angle = (angle - ((double)(rand() % (int)(M_PI * 100)) / 100.0 / 4.0) - M_PI * 2.0 / 8.0);
        if (angle < 0) {
            angle += M_PI * 2;
        }
        //cout << angle << endl;
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