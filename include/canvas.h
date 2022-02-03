#ifndef __CANVAS_H_INCLUDED__
#define __CANVAS_H_INCLUDED__

#include <gtk/gtk.h>
#include <list>
#include <vector>
#include "point.h"
#include "line.h"
#include "spline.h"
#include "polygon.h"
#include "curve.h"
#include "window.h"
#include "superBezier.h"
#include "superSpline.h"
#include "geometricObject.h"
#include "OBJLoader.h"

class Canvas
{
public:
    GtkWidget *frame;
    GtkWidget *da;
    Window *win;
    int viewportX;
    int viewportY;
    int windowMinX;
    int windowMaxX;
    int windowMinY;
    bool inPerpective;
    int windowMaxY;
    std::list<GeometricObject *> lg;
    Line *axisX;
    Line *axisY;
    int listSize;
    OBJLoader obj;
    float scale;

    Canvas();

    void SetCanvas(GtkWidget *movementGrid);

    void DrawLine(int xi, int yi, int xf, int yf);

    void GoRight();

    void GoLeft();

    void GoUp();

    void GoDown();

    void ZoomIn();

    void ZoomPlus();

    void ZoomMinus();

    void ZoomOut();

    void CreatePoint();

    void CreateLine();

    void CreatePolygon();

    void CreateCurve();

    void ChangeViewType();

    void RotateZ();

    void AddPoint(float mouseX, float mouseY);

    void AddLine(float x1, float y1, float x2, float y2);

    void AddPolygon(std::vector<Point *> mousePoints);

    void AddCurve(std::vector<Point *> mousePoints);

    void Redraw();

private:
    void RedrawWindow();

    void UpdateObjects();

    void ClipObjects();

    void ProjectObjects();

    void UnprojectObjects();

    void TranslateWorld(float x, float y, float z);

    void RotateWorldInX(float ang);

    void RotateWorldInY(float ang);

    void RotateWorldInZ(float ang);

    void ScaleWorld(float scale);

};

#endif
