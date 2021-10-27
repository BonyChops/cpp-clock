#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#define A_LINER 0
#define A_BEGIN_V0 1
#define A_END_V0 2
#define A_BOUND_V0 3

#include <iostream>

using namespace std;

class Animator {
   public:
    int mode = 0;
    bool played = false;

    Animator(double init, double end, int frame) {
        m_init = init;
        m_state = init;
        m_end = end;
        m_maxFrame = frame;
    }

    double play() {
        if (frame > m_maxFrame) {
            played = true;
            return m_end;
        }
        double t = ((double)frame / (double)m_maxFrame);
        frame += 1;
        switch (mode) {
            case A_LINER:
                return liner(t);
            case A_BEGIN_V0:
                return beginV0(t);
            case A_END_V0:
                return endV0(t);
            case A_BOUND_V0:
                return CubicBoundaryV0(t);
        }
        return 0.0;
    }
    void reset(){
        frame = 0;
    }

    double liner(double t) {
        return ((m_end - m_init) * t) + m_init;
    }

    double beginV0(double t) {
        double a = m_end - m_init;
        return (a * t * t) + m_init;
    }

    double endV0(double t) {
        double a = m_init - m_end;
        double b = -2.0 * a;
        double ret = a;
        ret *= t;
        ret += b;
        ret *= t;
        ret += m_init;
        return ret;
    }

    double CubicBoundaryV0(double t) {
        double a = 2.0 * (m_init - m_end);
        double b = 3.0 * (m_end - m_init);
        double ret = a;
        ret *= t;
        ret += b;
        ret *= t;
        ret *= t;
        ret += m_init;
        return ret;
    }

   private:
    double m_init;
    double m_end;
    double m_state = 0.0;
    int m_maxFrame = 0;
    int frame = 0;
};

#endif  //_ANIMATOR_H_