#ifndef __SPLINE_H_INCLUDED__
#define __SPLINE_H_INCLUDED__

#include <math.h>
#include <gtk/gtk.h>
#include <vector>
#include <list>
#include "geometricObject.h"
#include "point.h"
#include "polygon.h"
#include "line.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

class Spline : public GeometricObject
{

public:
    bool clipped;

    Spline(std::vector<Point *> points);

    void Draw(cairo_t *cr);

    void Translate(float x, float y, float z);

    void Scale(float scale);

    void ScaleWorld(float scale);

    void Clip();

    void RotateInX(float ang);

    void RotateAroundWorldInX(float ang);

    void RotateAroundPointInX(float ang, float x, float y, float z);

    void RotateInY(float ang);

    void RotateAroundWorldInY(float ang);

    void RotateAroundPointInY(float ang, float x, float y, float z);

    void RotateInZ(float ang);

    void RotateAroundWorldInZ(float ang);

    void RotateAroundPointInZ(float ang, float x, float y, float z);

    void Project();

    void Unproject();

    void SetViewPort(float _xVpMax, float _yVpMax);

    void SetWindow(float _xWinMin, float _yWinMin, float _xWinMax, float _yWinMax, float _dop);

    void ApplyTransform();

    float *MultiplyCoefMatrix(float m1[4][4], float m2[4]);

    void ForwardDiferences(int n, float x, float dx, float dx2, float dx3,
                           float y, float dy, float dy2, float dy3);

    std::vector<Point *> _pointsCurve;
    std::vector<Point *> _points; //inseridos pelo usuario
    float TransformToVpX(float x);

    float TransformToVpY(float y);

    float matrixM[4][4];

    //+++++++SPLINE++++++++++++++++++++++++++++++++++++
    float matrixMBS[4][4];
    float matrixE[4][4];
    float *forwardDiffX;
    float *forwardDiffY;
    float Gx[4];
    float Gy[4];
    float Gz[4];
    float oldX, oldY, oldZ;
    float *Cx;
    float *Cy;
    float *Cz;
    float delta;
    Polygon *poliClip;
    //+++++++SPLINE++++++++++++++++++++++++++++++++++++
    float _t;
    std::vector<Line *> _lines;
    std::vector<Point *> _newPoints;

};

#endif
