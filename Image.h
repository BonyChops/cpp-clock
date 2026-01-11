#ifndef _IMAGE_H_
#define _IMAGE_H_

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <cstdlib>
#else
#include <GL/glpng.h>
#endif

#include <GL/glut.h>
#include <math.h>

#include <iostream>
#include <string>

using namespace std;

#ifdef __EMSCRIPTEN__
typedef struct {
    int Width;
    int Height;
} pngInfo;
#endif

class Image {
   public:
    GLuint img;
    pngInfo info;
    Image(string bfilename) {
        string filename = bfilename;
#ifdef __EMSCRIPTEN__
        filename = "/" + filename;
        img = 0;
        info.Width = 0;
        info.Height = 0;
        int w = 0;
        int h = 0;
        unsigned char *data = (unsigned char *)emscripten_get_preloaded_image_data(filename.c_str(), &w, &h);
        if (!data) {
            cerr << "[!]FAILED TO LOAD RESOURCE: " + filename << endl;
            return;
        }
        info.Width = w;
        info.Height = h;
        glGenTextures(1, &img);
        glBindTexture(GL_TEXTURE_2D, img);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        free(data);
#else
        ////cout << filename << endl;
        img = pngBind(filename.c_str(), PNG_NOMIPMAP, PNG_ALPHA, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
        ////cout << "IMG NUM is: " << img << endl;
#endif
    }

    void putSprite(int x, int y) {
        putSprite(x, y, 1.0);
    }
    void putSprite(int x, int y, double scale) {
        putSprite(x, y, scale, 0.0);
    }
    void putSprite(int x, int y, double scale, double angle) {
        putSprite(x, y, scale, angle, 255);
    }

    void putSprite(int posX, int posY, double scale, double angle, int alpha) {
        m_x = posX;
        m_y = posY;
        int w = info.Width * scale;
        int h = info.Height * scale;

        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, img);
        glColor4ub(255, 255, 255, alpha);

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

#endif _IMAGE_H_