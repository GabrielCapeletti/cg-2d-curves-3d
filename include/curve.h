#ifndef __CURVE_H_INCLUDED__
#define __CURVE_H_INCLUDED__

#include <math.h>
#include <gtk/gtk.h>
#include <vector>
#include <list>
#include "geometricObject.h"
#include "point.h"
#include "line.h"
#include "spline.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

class Curve : public GeometricObject
{

public:
    bool clipped;

    Curve(std::vector<Point *> points);

    void Draw(cairo_t *cr);

    void Translate(float x, float y, float z);

    void Scale(float scale);

    void ScaleWorld(float scale);

    void Clip();

    void Unproject();

    void Project();

    void RotateInX(float ang);

    void RotateAroundWorldInX(float ang);

    void RotateAroundPointInX(float ang, float x, float y, float z);

    void RotateInY(float ang);

    void RotateAroundWorldInY(float ang);

    void RotateAroundPointInY(float ang, float x, float y, float z);

    void RotateInZ(float ang);

    void RotateAroundWorldInZ(float ang);

    void RotateAroundPointInZ(float ang, float x, float y, float z);

    void SetWindow(float _xWinMin, float _yWinMin, float _xWinMax, float _yWinMax, float dop);

    void SetViewPort(float _xVpMax, float _yVpMax);

    void ApplyTransform();

    void bezier(vector<Point *> p);

    Point *multiplay(vector<float> T, float matrixM[4][4], vector<Point *> p);


    //private:
    std::vector<Point *> _pointsCurve;
    std::vector<Point *> _points;//inseridos pelo usuario
    float TransformToVpX(float x);

    float TransformToVpY(float y);

    float matrixM[4][4];
    float _t;
    Polygon *poliClip;


};

#endif

