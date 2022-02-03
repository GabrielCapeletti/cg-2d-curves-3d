#ifndef __SUPERSPLINE_H_INCLUDED__
#define __SUPERSPLINE_H_INCLUDED__

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

class superSpline : public GeometricObject
{

public:
    bool clipped;

    superSpline(Point *Gp[4][4]);

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

    Polygon *
    drawForwardDiffSup(float n, float x, float dx, float dx2, float dx3, float y, float dy, float dy2, float dy3,
                       float z, float dz, float dz2, float dz3);

    void UpdateForwardDiffMatrices();

    void defineGxGyGz(Point *G[4][4]);

    float TransformToVpX(float x);

    float TransformToVpY(float y);

    float **multMatrix4x4(float **m1, float **m2);

    void transpose(float **mat);

    void createDeltaMatrices(float s, float t);

    void createMBMatrix();

    void calculateCoefficients();

    void createForwardDiffMatrices();

    //+++++++SPLINE++++++++++++++++++++++++++++++++++++
    Point *G[4][4];
    float **matrixMBS;
    float **Es;
    float **Et;
    float **forwardDiffX;
    float **forwardDiffY;
    float **forwardDiffZ;
    float **Gx;
    float **Gy;
    float **Gz;
    float oldX, oldY, oldZ;
    float **Cx;
    float **Cy;
    float **Cz;
    float delta_s;
    float delta_t;
    std::vector<Polygon *> polygons;
    //+++++++SPLINE++++++++++++++++++++++++++++++++++++
    std::vector<Line *> _lines;
    std::vector<Point *> _newPoints;

};

#endif
