#ifndef _CLOCKGUI_H_
#define _CLOCKGUI_H_
// #include <GL/glpng.h>
#include <GL/glut.h>
#include <math.h>

#include <ctime>
#include <iostream>
#include <string>

#include "Animator.h"
#include "Block.h"
#include "Image.h"
#include "WindowManager.h"

#define CIRCLE_ACCURATE 1000
#define LINE_ACCURATE 1000
#define BLOCKS 10

using namespace std;

class ClockGUI {
   public:
    double size = 200.0;
    double x = 0.0;
    double y = 0.0;
    Animator* backAni;
    bool generateBlocks = false;
    Block* blocks[BLOCKS * 3];

    ClockGUI(WindowManager* wm) {
        m_wm = wm;
    }

    virtual void init() {
        updateTime();
        int mode = A_END_V0;
        backAni = new Animator(0, size, 1000 / FRAME);
        backAni->mode = mode;
        minutesHandAni = new Animator(0, size, 1000 / FRAME);
        minutesHandAni->mode = mode;
        secondsHandAni = new Animator(0, size, 1000 / FRAME);
        secondsHandAni->mode = mode;
        hoursHandAni = new Animator(0, size, 1000 / FRAME);
        hoursHandAni->mode = mode;
        handsRoundAni = new Animator(M_PI * 4, 0, 2000 / FRAME);
        handsRoundAni->mode = mode;
        secondsHandBendAni = new Animator(2 * M_PI / 60.0, 0, 3);
        secondsHandBendAni->mode = A_BEGIN_V0;
        minutesHandBendAni = new Animator(2 * M_PI / 60.0, 0, 6);
        minutesHandBendAni->mode = A_BEGIN_V0;
        hoursHandBendAni = new Animator(2 * M_PI / 12.0, 0, 30);
        hoursHandBendAni->mode = A_BEGIN_V0;
        backColorAni = new Animator(200.0, 255.0, 100);
        scaleAni = new Animator(0.0, 25.0, 100);
        for (int i = 0; i < 12; i++) {
            numPics[i] = new Image(("images/clock/" + to_string(i + 1) + ".png"));
            numsRotateAni[i] = new Animator((M_PI / 2.0) * -1.0, (M_PI * 2 * (i + 1) / 12) - (M_PI / 2), 2000 / FRAME);
            numsRotateAni[i]->mode = mode;
        }
    }
    virtual void draw() {
        m_wm->drawMode = CENTER;
        updateTime();
        drawBackCircle();
        if (backAni->played) {
            drawScale();
        }
        if (backSize > size * 0.6) {
            roundAniVal = handsRoundAni->play();
            drawNums();
            drawMinutesHand();
            drawSecondsHand();
            drawHoursHand();
        }

        /* if (numsRotateAni[0]->played && !numRounded) {
            numRounded = true;
            for (int i = 0; i < 12; i++) {
                delete numsRotateAni[i];
                numsRotateAni[i] = new Animator()
            }
        } */
        generateBlocks = false;
        blockIndex = 0;
        //glBegin(GL_POINTS);
        //m_wm->drawLine(m_wm->px(0.0), m_wm->py(0.0), m_wm->px(100.0 * cos(testRound)), m_wm->py(100.0 * sin(testRound)));
        ////cout << testRound << endl;
        //glEnd();
        testRound += 0.01;
        if (testRound > 2 * M_PI) {
            testRound = 0.0;
        }
    }

   protected:
    double backSize = 0.0;
    WindowManager* m_wm;
    time_t tt;
    struct tm* ts;
    Animator* minutesHandAni;
    Animator* secondsHandAni;
    Animator* hoursHandAni;
    Animator* handsRoundAni;
    Animator* minutesHandBendAni;
    Animator* hoursHandBendAni;
    Animator* secondsHandBendAni;
    Animator* backColorAni;
    Animator* scaleAni;
    Animator* numsRotateAni[12];
    double roundAniVal = 0.0;
    double secondsBuf = 0.0;
    double hoursBuf = 0.0;
    double minutesBuf = 0.0;
    Image* numPics[12];
    bool numRounded = false;
    double testRound = 0.0;
    int blockIndex = 0;

    virtual void updateTime() {
        time(&tt);
        ts = localtime(&tt);
    }

    virtual void drawNums() {
        for (int i = 0; i < 12; i++) {
            double ani = numsRotateAni[i]->play();
            numPics[i]->putSprite(m_wm->px(size * 0.8 * cos(ani)), m_wm->py(size * 0.8 * sin(ani)), 0.8, 0.0 /* ani + M_PI / 2 */);
        }
    }

    virtual void drawScale() {
        drawScale(0, 0, 0);
    }

    void drawBorder(int r, int g, int b, double startPoint, double progress) {
        const int N = (int)CIRCLE_ACCURATE;

        glColor3ub((GLubyte)r, (GLubyte)g, (GLubyte)b);
        glPointSize(2.0f); // ※WebGLでは効かない/上限がある場合あり

        std::vector<float> v;
        v.reserve(N * 2);

        for (int i = 0; i < N; ++i) {
            double p = (double)i * 100.0 / (double)N;
            if (p < startPoint) continue;
            if (p > progress) break;

            double ang = 2.0 * M_PI * (double)i / (double)N - M_PI / 2.0;

            // 元コードの int キャストはやめて、そのまま px/py へ（安定）
            double x = (size - 0.2) * std::cos(ang);
            double y = (size)       * std::sin(ang);

            v.push_back((float)m_wm->px(x));
            v.push_back((float)m_wm->py(y));
        }

        if (v.empty()) return;

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, v.data());
        glDrawArrays(GL_POINTS, 0, (GLsizei)(v.size() / 2));
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void drawScale(int r, int g, int b) {
        const int N = 12;
        m_wm->drawMode = CENTER;

        double ani = scaleAni->play();

        // 線分 N 本 → 頂点は 2N 個 → (x,y) で 4N 要素
        std::vector<float> v;
        v.reserve(N * 4);

        for (int i = 0; i < N; ++i) {
            double ang = M_PI / 2.0 + (2.0 * M_PI * (double)i / (double)N);

            double x0 = size * std::cos(ang);
            double y0 = size * std::sin(ang);
            double x1 = (size - ani) * std::cos(ang);
            double y1 = (size - ani) * std::sin(ang);

            // ここは int キャストをやめて、px/py に double をそのまま渡すのが安定
            v.push_back((float)m_wm->px(x0));
            v.push_back((float)m_wm->py(y0));
            v.push_back((float)m_wm->px(x1));
            v.push_back((float)m_wm->py(y1));
        }

        glColor3ub(r, g, b);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, v.data());

        // GL_LINES: 2頂点で1本
        glDrawArrays(GL_LINES, 0, (GLsizei)(v.size() / 2));

        glDisableClientState(GL_VERTEX_ARRAY);
        glPointSize(1.0);
        glBegin(GL_POINTS);
        for (int i = 0; i < 12 * 5; i++) {
            m_wm->drawLine(
                (double)m_wm->px((int)(size * cos(M_PI / 2.0 + (M_PI * 2.0 * (double)i / (12.0 * 5.0))))),
                (double)m_wm->py((int)(size * sin(M_PI / 2.0 + (M_PI * 2.0 * (double)i / (12.0 * 5.0))))),
                (double)m_wm->px((int)((size - (ani / 2)) * cos(M_PI / 2.0 + (M_PI * 2.0 * (double)i / (12.0 * 5.0))))),
                (double)m_wm->py((int)((size - (ani / 2)) * sin(M_PI / 2.0 + (M_PI * 2.0 * (double)i / (12.0 * 5.0))))));
        }
        
        glEnd();
        
    }

    virtual void drawBackCircle() {
        drawBackCircle(255, 255, 255);
    }

    virtual void drawBackCircle(int r, int g, int b) {
        const int N = (int)CIRCLE_ACCURATE;
        m_wm->drawMode = CENTER;

        double ani = backAni->play();
        backSize = ani;

        // 頂点 (x,y) を float で詰める（WebGL系は float が相性良い）
        std::vector<float> v;
        v.reserve((N + 2) * 2);

        // 中心
        v.push_back((float)m_wm->px(0));
        v.push_back((float)m_wm->py(0));

        // 外周（最後に始点を重ねて閉じる）
        for (int i = 0; i <= N; ++i) {
            double t = 2.0 * M_PI * (double)i / (double)N;
            double x = ani * std::cos(t);
            double y = ani * std::sin(t);

            v.push_back((float)m_wm->px(x));
            v.push_back((float)m_wm->py(y));
        }

        // ---- ここから描画（glBegin/glEnd を使わない）----
        glColor4ub((GLubyte)r, (GLubyte)g, (GLubyte)b, 255);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, v.data());

        const GLsizei count = (GLsizei)(v.size() / 2); // ★ここが必ず整数
        glDrawArrays(GL_TRIANGLE_FAN, 0, count);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    virtual void drawMinutesHand() {
        if (minutesBuf != (double)ts->tm_min) {
            minutesHandBendAni->reset();
            minutesBuf = (double)ts->tm_min;
        }
        drawHand(100.0, 0, 0, 0, minutesHandAni, 0.75, (double)ts->tm_min /* + ((double)ts->tm_sec / 60.0) */, 60.0, minutesHandBendAni);
    }

    virtual void drawSecondsHand() {
        double timePer = 60.0;
        if (secondsBuf != (double)ts->tm_sec) {
            secondsHandBendAni->reset();
            secondsBuf = (double)ts->tm_sec;
            backColorAni->reset();
        }
        drawHand(50.0, 200, 200, 0, secondsHandAni, 0.8, (double)ts->tm_sec, timePer, secondsHandBendAni);
    }

    virtual void drawHoursHand() {
        /* if (hoursBuf != (double)ts->tm_hour) {
            hoursHandBendAni->reset();
            hoursBuf = (double)ts->tm_hour;
        } */
        drawHand(200.0, 0, 0, 0, hoursHandAni, 0.4, (double)ts->tm_hour + (((double)ts->tm_min + ((double)ts->tm_sec / 60.0)) / 60.0), 12.0, hoursHandBendAni);
    }

    virtual void drawHand(double width, int r, int g, int b, Animator* an, double per, double time, double timePer) {
        drawHand(width, r, g, b, an, per, time, timePer, nullptr);
    }

    virtual void drawHand(double width, int r, int g, int b, Animator* an, double per, double time, double timePer, Animator* bendAni) {
        /* glLineWidth(width);
        glBegin(GL_LINES);
        glColor3ub(r, g, b);
        glVertex2i(m_wm->px(0), m_wm->py(0));
        //glVertex2i(m_wm->px(size * 0.8), m_wm->py(0));

        double ani = an->play();
        glVertex2i(m_wm->px(ani * per * cos(roundAniVal + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer))),
                   m_wm->py(ani * per * sin(roundAniVal + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer)) * -1.0));
        glEnd(); */

        glPointSize(width / 25.0);
        glBegin(GL_POINTS);
        glColor3ub(r, g, b);
        glVertex2i(m_wm->px(0), m_wm->py(0));
        double ani = an->play();
        Animator lineBending(0.0, bendAni == nullptr ? 0 : bendAni->play(), LINE_ACCURATE);
        //Animator* lineBending = new Animator(0.0, bendAni == nullptr ? 0 : bendAni->play(), LINE_ACCURATE);
        lineBending.mode = A_BEGIN_V0;
        for (int i = 0; i < LINE_ACCURATE; i++) {
            double bendVal = lineBending.play();
            glVertex2i(m_wm->px((ani * per * cos(bendVal + roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer))) * i / LINE_ACCURATE),
                       m_wm->py((ani * per * sin(bendVal + roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer)) * -1.0) * i / LINE_ACCURATE));
        }
        if (generateBlocks) {
            for (int i = 0; i < BLOCKS; i++) {
                blocks[blockIndex] = new Block(
                    m_wm->px((ani * per * cos(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer))) * i / BLOCKS),
                    m_wm->py((ani * per * sin(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer)) * -1.0) * i / BLOCKS),
                    r, g, b);
                blockIndex += 1;
            }
        }
        

        glEnd();
        
    }
};

#endif  //_CLOCKGUI_H_