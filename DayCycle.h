#ifndef _DAY_CYCLE_H_
#define _DAY_CYCLE_H_

#include <ctime>
#include <iostream>

using namespace std;

class DayCycle {
   public:
    DayCycle() {
        updateTime();
        for (int i = 3600 * 12; i < 3600 * 16; i++) {
            cout << calc(i, 0, 253, 3600 * 12, 3600 * 17) << endl;
        }
    }

    void updateTime() {
        time(&tt);
        ts = localtime(&tt);
        ds = ts->tm_hour * 3600 + ts->tm_min * 60 + ts->tm_sec;
        /* ds += 10;
        if(ds >= 3600 * 24){
            ds = 0;
        } */
        //tt %= (60 * 60 * 24);
    }

    int calc(int x, int start, int end, int x1, int x2) {
        return (int)(((double)end - (double)start) * ((double)x - (double)x1) / ((double)x2 - (double)x1) + (double)start);
    }

    int getR() {
        updateTime();
        if (ds < (3600 * 12)) {
            return 0;
        } else if (ds < 3600 * 17) {
            return calc(ds, 0, 253, 3600 * 12, 3600 * 17);
        } else if(ds < 3600 * 20){
            return calc(ds, 253, 0, 3600 * 17, 3600 * 20);
        }else{
            return 0;
        }
        return 0;
    }

    int getG() {
        updateTime();
        if (ds < (3600 * 7)) {
            return calc(ds, 0, 146, 0, 3600 * 7);
        } else if (ds < (3600 * 12)) {
            return calc(ds, 146, 255, 3600 * 7, 3600 * 12);
        } else if (ds < (3600 * 17)) {
            return calc(ds, 255, 143, 3600 * 12, 3600 * 17);
        } else if(ds < 3600 * 20){
            return calc(ds, 143, 0, 3600 * 17, 3600 * 20);
        }
        return 0;
    }

    int getB() {
        updateTime();
        if (ds < (3600 * 7)) {
            return calc(ds, 100, 255, 0, 3600 * 7);
        } else if (ds < (3600 * 12)) {
            return 255;
        } else if (ds < (3600 * 17)) {
            return calc(ds, 255, 0, 3600 * 12, 3600 * 17);
        } else if(ds < 3600 * 20){
            return calc(ds, 0, 100, 3600 * 17, 3600 * 20);
        }
        return 100;
    }

   private:
    time_t tt;
    struct tm* ts;
    int ds = 0;
};

#endif  //_DAY_CYCLE_H_