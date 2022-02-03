#ifndef __MENU_H_INCLUDED__
#define __MENU_H_INCLUDED__

#include <gtk/gtk.h>
#include "canvas.h"

class Menu
{
public:
    Menu();

    void SetMenu(GtkWidget *movementGrid);

    void SetCanvas(Canvas *c);

    void DrawLine();

    void GoRight();

    void GoLeft();

    void GoUp();

    void GoDown();

    void ZoomIn();

    void ZoomOut();

    void ZoomPlus();

    void ZoomMinus();

    void RotateZ();

    void CreatePoint();

    void CreateLine();

    void CreatePolygon();

    void CreateCurve();

    void Redraw();

    void ChangeViewType();

private:
    Canvas *currentCanvas;

    GtkWidget *buttonPoint;
    GtkWidget *buttonLine;
    GtkWidget *buttonPoli;
    GtkWidget *buttonCurve;

    GtkWidget *buttonZoomPlus;
    GtkWidget *buttonZoomMinus;

    GtkWidget *buttonDraw;
    GtkWidget *buttonRight;
    GtkWidget *buttonLeft;
    GtkWidget *buttonUp;
    GtkWidget *buttonDown;
    GtkWidget *buttonRotateInY;
    GtkWidget *buttonTypeView;
    GtkWidget *buttonZoomIn;
    GtkWidget *buttonZoomOut;
};

#endif
