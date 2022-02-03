#ifndef __POINT_H_INCLUDED__
#define __POINT_H_INCLUDED__

#include <gtk/gtk.h>
#include <math.h>
#include "geometricObject.h"

class Point : public GeometricObject
{
public:
    int xVp;
    int yVp;
    float nonProjectedPosition[4];
    float position[4];

    Point(float x, float y, float z);

    void Clip();

    void Draw(cairo_t *cr);

    void Scale(float scale);

    void ScaleWorld(float scale);

    using GeometricObject::Translate;

    void Project();

    void Unproject();

    void Translate(float x, float y, float z);

    void RotateInX(float ang);

    void RotateAroundWorldInX(float ang);

    void RotateAroundPointInX(float ang, float x, float y, float z);

    void RotateInY(float ang);

    void RotateAroundWorldInY(float ang);

    void RotateAroundPointInY(float ang, float x, float y, float z);

    void RotateInZ(float ang);

    void RotateAroundWorldInZ(float ang);

    void RotateAroundPointInZ(float ang, float x, float y, float z);

    void ApplyTransform();
    /*int** Scale(int scale);
     int** Rotate(float ang);
     int** Rotate(float ang,int x, int y);
     int** RotateAroundWorld(float ang);
     */

};

#endif
