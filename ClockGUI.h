#ifndef _CLOCKGUI_H_
#define _CLOCKGUI_H_
#include <GL/glpng.h>
#include <GL/glut.h>
#include <math.h>

#include <ctime>
#include <iostream>
#include <string>

#include "Animator.h"
#include "Image.h"
#include "WindowManager.h"

#define CIRCLE_ACCURATE 1000
#define LINE_ACCURATE 1000

using namespace std;

class ClockGUI {
   public:
    double size = 200.0;
    double x = 0.0;
    double y = 0.0;
    ClockGUI(WindowManager* wm) {
        m_wm = wm;
    }

    void init() {
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
        secondsHandBendAni = new Animator(2 * M_PI / 60.0, 0, 15);
        secondsHandBendAni->mode = A_BEGIN_V0;
        minutesHandBendAni = new Animator(2 * M_PI / 60.0, 0, 30);
        minutesHandBendAni->mode = A_BEGIN_V0;
        hoursHandBendAni = new Animator(2 * M_PI / 12.0, 0, 30);
        hoursHandBendAni->mode = A_BEGIN_V0;
        backColorAni = new Animator(200.0, 255.0, 100);
        scaleAni = new Animator(0.0, 25.0, 100);
        for (int i = 0; i < 12; i++) {
            numPics[i] = new Image(("images/" + to_string(i + 1) + ".png"));
            numsRotateAni[i] = new Animator((M_PI / 2.0) * -1.0, (M_PI * 2 * (i + 1) / 12) - (M_PI / 2), 2000 / FRAME);
            numsRotateAni[i]->mode = mode;
        }
    }
    void draw() {
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
    }

   private:
    double backSize = 0.0;
    WindowManager* m_wm;
    time_t tt;
    struct tm* ts;
    Animator* backAni;
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

    void updateTime() {
        time(&tt);
        ts = localtime(&tt);
    }

    void drawNums() {
        for (int i = 0; i < 12; i++) {
            double ani = numsRotateAni[i]->play();
            numPics[i]->putSprite(m_wm->px(size * 0.8 * cos(ani)), m_wm->py(size * 0.8 * sin(ani)), 0.8, ani + M_PI / 2);
        }
    }

    void drawScale() {
        glPointSize(2.0);
        glBegin(GL_POINTS);
        glColor3ub(0.0, 0.0, 0.0);
        double ani = scaleAni->play();
        for (int i = 0; i < 12; i++) {
            m_wm->drawLine(
                (double)m_wm->px((int)(size * cos(M_PI / 2.0 + (M_PI * 2.0 * (double)i / 12.0)))),
                (double)m_wm->py((int)(size * sin(M_PI / 2.0 + (M_PI * 2.0 * (double)i / 12.0)))),
                (double)m_wm->px((int)((size - ani) * cos(M_PI / 2.0 + (M_PI * 2.0 * (double)i / 12.0)))),
                (double)m_wm->py((int)((size - ani) * sin(M_PI / 2.0 + (M_PI * 2.0 * (double)i / 12.0)))));
        }
        glEnd();
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

    void drawBackCircle() {
        glBegin(GL_POLYGON);
        glColor3ub(/* (int)backColorAni->play() */ 255, 255, 255);
        m_wm->drawMode = CENTER;
        double ani = backAni->play();
        for (int i = 0; i < CIRCLE_ACCURATE; i++) {
            glVertex2i(m_wm->px(0 + ani * cos(2 * M_PI * i / CIRCLE_ACCURATE)), m_wm->py(0 + ani * sin(2 * M_PI * i / CIRCLE_ACCURATE)));
        }
        backSize = ani;
        glEnd();
    }

    void drawMinutesHand() {
        if (minutesBuf != (double)ts->tm_min) {
            minutesHandBendAni->reset();
            minutesBuf = (double)ts->tm_min;
        }
        drawHand(100.0, 0, 0, 0, minutesHandAni, 0.75, (double)ts->tm_min /* + ((double)ts->tm_sec / 60.0) */, 60.0, minutesHandBendAni);
    }

    void drawSecondsHand() {
        double timePer = 60.0;
        if (secondsBuf != (double)ts->tm_sec) {
            secondsHandBendAni->reset();
            secondsBuf = (double)ts->tm_sec;
            backColorAni->reset();
        }
        drawHand(50.0, 200, 200, 0, secondsHandAni, 0.8, (double)ts->tm_sec, timePer, secondsHandBendAni);
    }

    void drawHoursHand() {
        if (hoursBuf != (double)ts->tm_hour) {
            hoursHandBendAni->reset();
            hoursBuf = (double)ts->tm_hour;
        }
        drawHand(200.0, 0, 0, 0, hoursHandAni, 0.4, (double)ts->tm_hour /*  + (((double)ts->tm_min + ((double)ts->tm_sec / 60.0)) / 60.0) */, 12.0, hoursHandBendAni);
    }

    void drawHand(double width, int r, int g, int b, Animator* an, double per, double time, double timePer) {
        drawHand(width, r, g, b, an, per, time, timePer, nullptr);
    }

    void drawHand(double width, int r, int g, int b, Animator* an, double per, double time, double timePer, Animator* bendAni) {
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
        glEnd();
    }
};

#endif  //_CLOCKGUI_H_