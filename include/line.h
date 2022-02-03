#ifndef __LINE_H_INCLUDED__
#define __LINE_H_INCLUDED__

#include <gtk/gtk.h>
#include <stdlib.h>
#include "geometricObject.h"
#include "point.h"
#include "line.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <list>

using std::vector;
using std::endl;
using std::cout;

class Line : public GeometricObject
{

public:
    Line *clippedLine;
    bool clipped;
    int RC[4];
    int RCP1;
    int RCP2;
    Point *_pi;
    Point *_pf;

    Line(Point *pi, Point *pf);

    void Clip();

    void CohenSuthernClipping();

    void Draw(cairo_t *cr);

    void Project();

    void Unproject();

    vector<float> calculateP();

    vector<float> calculateQ();

    vector<float> calculateR(vector<float> q, vector<float> r);

    Line *clippingLiang();

    Line *newLine;
    float dx;
    float dy;
    using GeometricObject::Translate;

    void Translate(float x, float y, float z);

    void ApplyTransform();

    void Scale(float scale);

    void ScaleWorld(float scale);

    bool IsBitOne(int number, int index, int mask);

    void RotateInX(float ang);

    void RotateAroundWorldInX(float ang);

    void RotateAroundPointInX(float ang, float x, float y, float z);

    void RotateInY(float ang);

    void RotateAroundWorldInY(float ang);

    void RotateAroundPointInY(float ang, float x, float y, float z);

    void RotateInZ(float ang);

    void RotateAroundWorldInZ(float ang);

    void RotateAroundPointInZ(float ang, float x, float y, float z);

private:


};

#endif
