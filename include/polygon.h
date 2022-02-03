#ifndef __POLYGON_H_INCLUDED__
#define __POLYGON_H_INCLUDED__

#include <gtk/gtk.h>
#include <vector>
#include <list>
#include "geometricObject.h"
#include "point.h"
#include "line.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

class Polygon : public GeometricObject
{

public:
    bool clipped;
    bool conectFirstAndLast;

    Polygon(std::vector<Point *> points);

    void Draw(cairo_t *cr);

    void Translate(float x, float y, float z);

    void Scale(float scale);

    void ScaleWorld(float scale);

    void Clip();

    void InsertBetweenWindow(Point *p);

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

    void SetWindow(float _xWinMin, float _yWinMin, float _xWinMax,
                   float _yWinMax, float _dop);

    void ApplyTransform();

    void clipLeft(Point *p);

    void clipRight(Point *p);

    void clipUp(Point *p);

    void clipDown(Point *p);

    void clipLeftUp(Point *p);

    void clipLeftDown(Point *p);

    void clipRightUp(Point *p);

    void clipRightDown(Point *p);

    void insidePoints();

    //private:
    std::vector<Point *> _points;
    std::vector<Point *> _newPoints;
    std::vector<Line *> _Lines;

    float TransformToVpX(float x);

    float TransformToVpY(float y);

};

#endif
