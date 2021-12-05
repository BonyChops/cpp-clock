#ifndef _FUTUREGUI_H_
#define _FUTUREGUI_H_

#include <chrono>
#include <sstream>

#include "ClockGUI.h"
#include "DayCycle.h"

class FutureGUI : public ClockGUI {
   public:
    using ClockGUI::ClockGUI;

    void init() override {
        ClockGUI::init();
        for (int i = 0; i <= 9; i++) {
            digitalNums[i] = new Image(("images/digital/" + to_string(i) + ".png"));
        }
    }

    void draw() override {
        ClockGUI::draw();
        if (backAni->played) {
            drawBorder(0, 200, 0, ts->tm_sec % 2 == 1 ? (int)(ms * 100.0 / 1000.0) : 0, ts->tm_sec % 2 == 0 ? (int)(ms * 100.0 / 1000.0) : 100);
        }
    }

   protected:
    int ms = 0;
    DayCycle dayCycle;
    Image* digitalNums[10];
    int frame[12] = {0};
    string bufferNum[12];

    int deToBi(int de) {
        int tmp = de;
        int res[5];
        int i;
        for (i = 0; i < 5; i++) {
            res[i] = tmp % 2;
            tmp /= 2;
            if (tmp < 1) {
                break;
            }
        }
        for (int j = i; j >= 0; j--) {
            if (j != i) {
                tmp *= 10;
            }
            tmp += res[j];
        }
        return tmp;
    }

    void putsString(string str, double size, double angle, double scale) {
        double sumW = 0;
        for (int i = 0; i < str.length(); i++) {
            sumW += putChar(str[i], size + sumW, angle, scale);
        }
    }

    double putChar(char c, double size, double angle, double scale) {
        if ('0' <= c && c <= '9') {
            digitalNums[c - '0']->putSprite(m_wm->px(size * cos(angle)), m_wm->py(size * sin(angle)), scale, angle);
            return digitalNums[c - '0']->info.Width * scale;
        }
        return 0;
    }

    void drawNums() override {
        for (int i = 0; i < 12; i++) {
            int bi = deToBi(i + 1);
            ostringstream oss;
            string biStr;
            if (!numsRotateAni[0]->played) {
                if (frame[i] < 7) {
                    frame[i] += 1;
                    biStr = bufferNum[i];
                } else {
                    frame[i] = 0;
                    for (int i = 0; i < 5; i++) {
                        oss << to_string(rand() % 10);
                    }

                    oss << flush;
                    biStr = oss.str();
                    bufferNum[i] = biStr;
                }

            } else {
                oss << setfill('0') << right << std::setw(5) << bi << flush;
                biStr = oss.str();
            }

            double ani = numsRotateAni[i]->play();
            putsString(biStr, size - 80, ani /* + M_PI / 2 */, 0.5);
            //numPics[i]->putSprite(m_wm->px(size * 0.8 * cos(ani)), m_wm->py(size * 0.8 * sin(ani)), 0.8, 0.0 /* ani + M_PI / 2 */);
        }
    }

    void drawScale() override {
        ClockGUI::drawScale(255, 255, 255);
    }

    void drawBackCircle() override {
        ClockGUI::drawBackCircle(50, 50, 50);
    }

    void updateTime() override {
        ClockGUI::updateTime();
        ms = (int)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000);
    }

    void drawMinutesHand() override {
        if (minutesBuf != (double)ts->tm_min) {
            minutesHandBendAni->reset();
            minutesBuf = (double)ts->tm_min;
        }
        drawHand(100.0, 0, 200, 0, minutesHandAni, 0.75, (double)ts->tm_min /* + ((double)ts->tm_sec / 60.0) */, 60.0);
    }

    void drawSecondsHand() override {
        double timePer = 60.0;
        if (secondsBuf != (double)ts->tm_sec) {
            secondsHandBendAni->reset();
            secondsBuf = (double)ts->tm_sec;
            backColorAni->reset();
        }
        drawHand(50.0, 0, 200, 0, secondsHandAni, 0.8, (double)ts->tm_sec, timePer, (double)ms * 100.0 / 1000.0);
    }

    void drawHoursHand() override {
        /* if (hoursBuf != (double)ts->tm_hour) {
            hoursHandBendAni->reset();
            hoursBuf = (double)ts->tm_hour;
        } */
        drawHand(200.0, 0, 200, 0, hoursHandAni, 0.4, (double)ts->tm_hour + (((double)ts->tm_min + ((double)ts->tm_sec / 60.0)) / 60.0), 12.0);
    }

    void drawHand(double width, int r, int g, int b, Animator* an, double per, double time, double timePer) {
        drawHand(width, r, g, b, an, per, time, timePer, 50);
    }

    void drawHand(double width, int r, int g, int b, Animator* an, double per, double time, double timePer, double progress) {
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
        ////cout << dayCycle.calcD(progress, 0.0, 100.0, 0, 50.0) << endl;
        double startPos = progress < 50.0 ? dayCycle.calcD(progress, 0.0, 100.0, 0.0, 50.0) : 100.0;
        double endPos = progress >= 50.0 ? dayCycle.calcD(progress, 0.0, 100.0, 50.0, 100.0) : 0.0;
        m_wm->drawLine(
            m_wm->px((ani * per * cos(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer))) * startPos / 100.0),
            m_wm->py((ani * per * sin(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer)) * -1.0) * startPos / 100.0),
            m_wm->px((ani * per * cos(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer))) * endPos / 100.0),
            m_wm->py((ani * per * sin(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer)) * -1.0) * endPos / 100.0));
        /* for (int i = 0; i < LINE_ACCURATE; i++) {
            double bendVal = lineBending.play();
            glVertex2i(m_wm->px((ani * per * cos(bendVal + roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer))) * i / LINE_ACCURATE),
                       m_wm->py((ani * per * sin(bendVal + roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer)) * -1.0) * i / LINE_ACCURATE));
        } */
        /* if (generateBlocks) {
            for (int i = 0; i < BLOCKS; i++) {
                blocks[blockIndex] = new Block(
                    m_wm->px((ani * per * cos(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer))) * i / BLOCKS),
                    m_wm->py((ani * per * sin(roundAniVal * (timePer == 12.0 ? 1 : -1) + (M_PI / 2.0) - (M_PI * 2.0 * time / timePer)) * -1.0) * i / BLOCKS),
                    r, g, b);
                blockIndex += 1;
            }
        } */
        glEnd();
    }
};

#endif  //_FUTUREGUI_H_