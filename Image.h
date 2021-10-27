#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <GL/glpng.h>
#include <GL/glut.h>
#include <math.h>

#include <iostream>
#include <string>

using namespace std;

class Image {
   public:
    GLuint img;
    pngInfo info;
    Image(string filename) {
        img = pngBind(filename.c_str(), PNG_NOMIPMAP, PNG_ALPHA, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }

    void putSprite(int x, int y) {
        putSprite(x, y, 1.0);
    }
    void putSprite(int x, int y, double scale) {
        putSprite(x, y, scale, 0.0);
    }

    void putSprite(int posX, int posY, double scale, double angle) {
        m_x = posX;
        m_y = posY;
        int w = info.Width * scale;
        int h = info.Height * scale;

        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE, img);
        glColor4ub(255, 255, 255, 255);

        glBegin(GL_QUADS);

        glTexCoord2i(0, 0);
        double x, y;
        x = (double)w * -1.0 / 2.0;
        y = (double)h * -1.0 / 2.0;
        glVertex2i(px((int)(x * cos(angle) - y * sin(angle))), py((int)(x * sin(angle) + y * cos(angle))));

        glTexCoord2i(0, 1);
        x = (double)w * -1.0 / 2.0;
        y = (double)h * 1.0 / 2.0;
        glVertex2i(px((int)(x * cos(angle) - y * sin(angle))), py((int)(x * sin(angle) + y * cos(angle))));

        glTexCoord2i(1, 1);
        x = (double)w * 1.0 / 2.0;
        y = (double)h * 1.0 / 2.0;
        glVertex2i(px((int)(x * cos(angle) - y * sin(angle))), py((int)(x * sin(angle) + y * cos(angle))));

        glTexCoord2i(1, 0);
        x = (double)w * 1.0 / 2.0;
        y = (double)h * -1.0 / 2.0;
        glVertex2i(px((int)(x * cos(angle) - y * sin(angle))), py((int)(x * sin(angle) + y * cos(angle))));

        glEnd();

        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

   private:
    int m_x;
    int m_y;
    int px(int x) {
        return x + m_x;
    }

    int py(int y) {
        return y + m_y;
    }
};

#endif  //_IMAGE_H_