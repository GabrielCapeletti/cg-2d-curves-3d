#ifndef __WINDOW_H_INCLUDED__
#define __WINDOW_H_INCLUDED__

#include <gtk/gtk.h>
#include <math.h>
#include "point.h"

class Window
{
public:
    float minX;
    float minY;
    float maxX;
    float maxY;
    float scale;
    float angX;
    float angY;
    float dop;
    Point *p0;
    Point *cop;
    Point *vpn;
    Point *v;
    Point *u;

    Window(Point *initialPos, float rotX, float rotY, float _scale, float w, float h);

    void Translate(float x, float y, int z);

    void Scale();

private:

};

#endif
