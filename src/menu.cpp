#include "menu.h"

Menu *Instance;

static void down(GtkWidget *widget, gpointer data)
{
    Instance->GoDown();
}

static void changeViewType(GtkWidget *widget, gpointer data)
{
    Instance->ChangeViewType();
}

static void rotateZ(GtkWidget *widget, gpointer data)
{
    Instance->RotateZ();
}

static void right(GtkWidget *widget, gpointer data)
{
    Instance->GoRight();
}

static void left(GtkWidget *widget, gpointer data)
{
    Instance->GoLeft();
}

static void redraw(GtkWidget *widget, gpointer data)
{
    Instance->Redraw();
}

static void up(GtkWidget *widget, gpointer data)
{
    Instance->GoUp();
}

static void zoomIn(GtkWidget *widget, gpointer data)
{
    Instance->ZoomIn();
}

static void zoomOut(GtkWidget *widget, gpointer data)
{
    Instance->ZoomOut();
}

static void zoomPlus(GtkWidget *widget, gpointer data)
{
    Instance->ZoomPlus();
}

static void zoomMinus(GtkWidget *widget, gpointer data)
{
    Instance->ZoomMinus();
}

static void createPoint(GtkWidget *widget, gpointer data)
{
    Instance->CreatePoint();
}

static void createLine(GtkWidget *widget, gpointer data)
{
    Instance->CreateLine();
}

static void createPolygon(GtkWidget *widget, gpointer data)
{
    Instance->CreatePolygon();
}

static void createCurve(GtkWidget *widget, gpointer data)
{
    Instance->CreateCurve();
}


Menu::Menu()
{
    Instance = (Menu * )
    this;
}

void Menu::DrawLine()
{
    currentCanvas->DrawLine(10, 10, 100, 100);
}

void Menu::SetCanvas(Canvas *c)
{
    currentCanvas = c;
    //currentCanvas->DrawLine(0,0,0,0);
}

void Menu::ChangeViewType()
{
    currentCanvas->ChangeViewType();
}

void Menu::RotateZ()
{
    currentCanvas->RotateZ();
}

void Menu::CreatePoint()
{
    currentCanvas->CreatePoint();
}

void Menu::CreateLine()
{
    currentCanvas->CreateLine();
}

void Menu::CreatePolygon()
{
    currentCanvas->CreatePolygon();
}

void Menu::CreateCurve()
{
    currentCanvas->CreateCurve();
}

void Menu::Redraw()
{
    currentCanvas->Redraw();
}

void Menu::GoRight()
{
    currentCanvas->GoRight();
}

void Menu::GoLeft()
{
    currentCanvas->GoLeft();
}

void Menu::GoUp()
{
    currentCanvas->GoUp();
}

void Menu::GoDown()
{
    currentCanvas->GoDown();
}

void Menu::ZoomIn()
{
    currentCanvas->ZoomIn();
}

void Menu::ZoomOut()
{
    currentCanvas->ZoomOut();
}

void Menu::ZoomPlus()
{
    currentCanvas->ZoomPlus();
}

void Menu::ZoomMinus()
{
    currentCanvas->ZoomMinus();
}

void Menu::SetMenu(GtkWidget *movementGrid)
{

    GtkWidget *pointImg = gtk_image_new_from_file("point.jpg");
    GtkWidget *lineImg = gtk_image_new_from_file("line.jpg");
    GtkWidget *poliImg = gtk_image_new_from_file("poli.jpg");
    GtkWidget *curveImg = gtk_image_new_from_file("curve.jpg");

    buttonZoomIn = gtk_button_new_with_label("Rotate Right");
    buttonZoomOut = gtk_button_new_with_label("Rotate Left");
    buttonRotateInY = gtk_button_new_with_label("RotateZ");
    buttonRight = gtk_button_new_with_label("Right");
    buttonLeft = gtk_button_new_with_label("Left");
    buttonUp = gtk_button_new_with_label("Up");
    buttonDown = gtk_button_new_with_label("Down");
    buttonDraw = gtk_button_new_with_label("Draw");
    buttonTypeView = gtk_button_new_with_label("ViewType");
    buttonZoomPlus = gtk_button_new_with_label("Zoom In");
    buttonZoomMinus = gtk_button_new_with_label("Zoom Out");


    buttonPoint = gtk_button_new();
    buttonLine = gtk_button_new();
    buttonPoli = gtk_button_new();
    buttonCurve = gtk_button_new();

    gtk_button_set_image(GTK_BUTTON(buttonPoint), pointImg);
    gtk_button_set_image(GTK_BUTTON(buttonLine), lineImg);
    gtk_button_set_image(GTK_BUTTON(buttonPoli), poliImg);
    gtk_button_set_image(GTK_BUTTON(buttonCurve), curveImg);


    g_signal_connect(buttonZoomPlus, "clicked", G_CALLBACK(zoomPlus), NULL);
    g_signal_connect(buttonZoomMinus, "clicked", G_CALLBACK(zoomMinus), NULL);
    g_signal_connect(buttonDraw, "clicked", G_CALLBACK(redraw), NULL);
    g_signal_connect(buttonZoomIn, "clicked", G_CALLBACK(zoomIn), NULL);
    g_signal_connect(buttonRotateInY, "clicked", G_CALLBACK(rotateZ), NULL);
    g_signal_connect(buttonZoomOut, "clicked", G_CALLBACK(zoomOut), NULL);
    g_signal_connect(buttonRight, "clicked", G_CALLBACK(right), NULL);
    g_signal_connect(buttonLeft, "clicked", G_CALLBACK(left), NULL);
    g_signal_connect(buttonUp, "clicked", G_CALLBACK(up), NULL);
    g_signal_connect(buttonDown, "clicked", G_CALLBACK(down), NULL);
    g_signal_connect(buttonTypeView, "clicked", G_CALLBACK(changeViewType), NULL);

    g_signal_connect(buttonPoint, "clicked", G_CALLBACK(createPoint), NULL);
    g_signal_connect(buttonLine, "clicked", G_CALLBACK(createLine), NULL);
    g_signal_connect(buttonPoli, "clicked", G_CALLBACK(createPolygon), NULL);
    g_signal_connect(buttonCurve, "clicked", G_CALLBACK(createCurve), NULL);

    gtk_grid_attach(GTK_GRID(movementGrid), buttonPoint, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonLine, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonPoli, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonCurve, 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonZoomPlus, 8, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonZoomMinus, 8, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonTypeView, 7, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonDraw, 7, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonRotateInY, 6, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonZoomIn, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonZoomOut, 4, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonRight, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonLeft, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonUp, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(movementGrid), buttonDown, 2, 2, 1, 1);
}
