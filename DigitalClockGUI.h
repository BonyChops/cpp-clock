#ifndef _DIGITAL_CLOCK_GUI_H_
#define _DIGITAL_CLOCK_GUI_H_

#include <stdio.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#include "Image.h"
#include "WindowManager.h"

using namespace std;

class DigitalClockGUI {
   public:
    DigitalClockGUI(WindowManager* wm) {
        m_wm = wm;
        for (int i = 0; i < 10; i++) {
            numPics[i] = new Image(("images/clock/" + to_string(i) + ".png"));
        }
        colon = new Image("images/clock/colon.png");
        board = new Image("images/clock/board.png");
        updateTime();
        hoursAni = new Animator(ts->tm_hour > 12 ? 0.0 : 24.0, (double)ts->tm_hour, 300);
        hoursAni->mode = A_END_V0;
        minutesAni = new Animator(ts->tm_min > 30 ? 0.0 : 60.0, (double)ts->tm_min, 300);
        minutesAni->mode = A_END_V0;
        yAni = new Animator(-50.0, m_y, 300);
        yAni->mode = A_END_V0;
        blinkAni = new Animator(0.0, 255, 100);
    }

    virtual void draw() {
        m_y = (int)yAni->play();
        m_wm->drawMode = NORMAL;
        updateTime();
        board->putSprite(px(55), py(22), 1.2);
        drawNums();
    }

   private:
    WindowManager* m_wm;
    Image* numPics[10];
    Image* colon;
    Image* board;
    time_t tt;
    Animator* hoursAni;
    Animator* minutesAni;
    Animator* yAni;
    Animator* blinkAni;
    struct tm* ts;
    int m_x = 25;
    int m_y = 50;
    int ms = 0;
    bool blinked = false;

    void updateTime() {
        time(&tt);
        ts = localtime(&tt);
        ms = (int)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000);

        if ((ms < 500) != blinked) {
            blinked = ms < 500;
            delete blinkAni;
            blinkAni = new Animator(blinked ? 0.0 : 255.0, !blinked ? 0.0 : 255.0, 10);
        }
    }

    void drawNums() {
        int sumW = 0;
        ostringstream oss;
        oss << setfill('0') << right << std::setw(2) << (hoursAni->played ? ts->tm_hour : (int)hoursAni->play());
        oss << ":";
        oss << setfill('0') << right << std::setw(2) << (minutesAni->played ? ts->tm_min : (int)minutesAni->play());
        oss << flush;
        string hour = oss.str();
        putsString(hour, px(0), px(0));
        //snprintf(hour, sizeof(hour), "%02d", ts->tm_hour);
    }

    void putsString(string str, int x, int y) {
        int sumW = x;
        for (int i = 0; i < str.length(); i++) {
            sumW += putChar(str[i], sumW, y);
        }
    }

    int putChar(char c, int x, int y) {
        switch (c) {
            case ':':
                colon->putSprite(px(x) - 4, py(y - 3), 1.0, 0.0, (int)blinkAni->play());
                return colon->info.Width;
                break;
        }
        if ('0' <= c && c <= '9') {
            numPics[c - '0']->putSprite(px(x), py(y));
            return numPics[c - '0']->info.Width;
        }
        return 0;
    }

    int px(int x) {
        return x + m_x;
    }

    int py(int y) {
        return y + m_y;
    }
};

#endif  //_DIGITAL_CLOCK_GUI_H_