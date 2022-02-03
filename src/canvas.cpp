#include "canvas.h"

Canvas *CanvasInstance;
int state = 0;
/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
GtkWidget *da2;

static void clear_surface(void)
{
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_destroy(cr);
}

static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if (surface)
    {
        cairo_surface_destroy(surface);
    }

    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
                                                CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width(widget),
                                                gtk_widget_get_allocated_height(widget));

    clear_surface();

    return TRUE;
}

static void draw_brush(GtkWidget *widget)
{
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_line_width(cr, 0.9);
    cairo_move_to(cr, 10, 10);
    cairo_line_to(cr, (int) (CanvasInstance->viewportX - 10), 10);
    cairo_line_to(cr, (int) (CanvasInstance->viewportX - 10), CanvasInstance->viewportY - 10);
    cairo_line_to(cr, (int) (10), CanvasInstance->viewportY - 10);
    cairo_line_to(cr, (int) (10), 10);
    cairo_set_source_rgb(cr, 1, 0, 0);

    std::list<GeometricObject *>::iterator i;

    int j = 0;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->Draw(cr);
        j++;
    }
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke(cr);
    cairo_destroy(cr);
}

bool hasPoint = false;
Point *firstLinePoint;
std::vector<Point *> _mousePoints;

static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event,
                                      gpointer data)
{

    if (surface == NULL)
    {
        return false;
    }

    if (event->button == GDK_BUTTON_PRIMARY)
    {
        if (state == 0)
        {
            CanvasInstance->AddPoint(event->x, event->y);
        }
        if (state == 1)
        {
            if (!hasPoint)
            {
                hasPoint = true;
                firstLinePoint = new Point(event->x, event->y, 0);
            }
            else
            {
                hasPoint = false;
                CanvasInstance->AddLine(firstLinePoint->position[0],
                                        firstLinePoint->position[1], event->x, event->y);
            }
        }
        if (state == 2)
        {
            CanvasInstance->AddPoint(event->x, event->y);
            _mousePoints.push_back(new Point(event->x, event->y, 0));
        }
        if (state == 3)
        {
            CanvasInstance->AddPoint(event->x, event->y);
            _mousePoints.push_back(new Point(event->x, event->y, 0));
            if (_mousePoints.size() >= 4)
            {
                CanvasInstance->AddCurve(_mousePoints);
                _mousePoints.erase(_mousePoints.begin());
            }
        }

    }
    else if (event->button == GDK_BUTTON_SECONDARY)
    {
        if (state == 2)
        {
            CanvasInstance->AddPolygon(_mousePoints);
            _mousePoints.clear();
        }
    }

    return true;
}

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{

    cairo_set_source_surface(cr, surface, 0, 0);

    cairo_paint(cr);

    draw_brush(widget);

    //g_print("drawSignal\n");

    return FALSE;
}

static void close_window(void)
{
    if (surface)
    {
        cairo_surface_destroy(surface);
    }

    gtk_main_quit();

}

Canvas::Canvas()
{
    // g_print("CAnvas init\n");
    //if(!CanvasInstance){
    //g_print("CAnvas init\n");
    CanvasInstance = this;
    //}
    inPerpective = false;
    //(Point *initialPos, float rot, float _scale,int _minX,int _minY,int _maxX,int _maxY)
    win = new Window(new Point(0, 0, 0), 0, 0, 1, 600, 600);

    //worldCoordinates = new Point(win->p0->position[0],win->p0->position[1],win->p0->position[2]+1);
    scale = 1;

    viewportX = 600;
    viewportY = 600;

    windowMinX = -200;
    windowMaxX = 400;
    windowMinY = -300;
    windowMaxY = 300;

    Point *GPoints[4][4];    //pontos para bezier
    GPoints[0][0] = new Point(100, 300, 1);
    GPoints[1][0] = new Point(100, 300, 2);
    GPoints[2][0] = new Point(100, 300, 3);
    GPoints[3][0] = new Point(100, 300, 4);
    GPoints[0][1] = new Point(200, 300, 1);
    GPoints[1][1] = new Point(200, -200, 2);
    GPoints[2][1] = new Point(200, -200, 3);
    GPoints[3][1] = new Point(200, 300, 4);
    GPoints[0][2] = new Point(300, 300, 1);
    GPoints[1][2] = new Point(300, -200, 2);
    GPoints[2][2] = new Point(300, -200, 3);
    GPoints[3][2] = new Point(300, 300, 4);
    GPoints[0][3] = new Point(400, 300, 1);
    GPoints[1][3] = new Point(400, 300, 2);
    GPoints[2][3] = new Point(400, 300, 3);
    GPoints[3][3] = new Point(400, 300, 4);

    Point *GP[4][4];    //pontos para spline
    GP[0][0] = new Point(-1, 3, -1);
    GP[1][0] = new Point(-1, 3, -2);
    GP[2][0] = new Point(-1, 3, -3);
    GP[3][0] = new Point(-1, 3, -4);
    GP[0][1] = new Point(0, 3, 1);
    GP[1][1] = new Point(0, -2, 2);
    GP[2][1] = new Point(0, -2, 3);
    GP[3][1] = new Point(0, 3, 4);
    GP[0][2] = new Point(1, 3, -1);
    GP[1][2] = new Point(1, -2, -2);
    GP[2][2] = new Point(1, -2, -3);
    GP[3][2] = new Point(1, 3, -4);
    GP[0][3] = new Point(2, 3, 1);
    GP[1][3] = new Point(2, 3, 2);
    GP[2][3] = new Point(2, 3, 3);
    GP[3][3] = new Point(2, 3, 4);
    superSpline *sSpline = new superSpline(GP);
    sSpline->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    sSpline->SetViewPort(viewportX, viewportY);
    //lg.push_back(sSpline);

    superBezier *sB = new superBezier(GPoints);
    sB->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    sB->SetViewPort(viewportX, viewportY);
    //lg.push_back(sB);

    std::vector < Point * > cPoints(4);
    cPoints[0] = new Point(-300, -100, 2);
    cPoints[1] = new Point(-300, 200, 2);
    cPoints[2] = new Point(-100, 200, 2);
    cPoints[3] = new Point(-100, 600, 2);

    Curve *curve = new Curve(cPoints);
    curve->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    curve->SetViewPort(viewportX, viewportY);

    Spline *spline = new Spline(cPoints);
    spline->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    spline->SetViewPort(viewportX, viewportY);
    // curve->Translate(50,-100);
//    curve->Scale(1);
//    curve->Rotate(90);
//	lg.push_back(spline);

    cPoints[0] = cPoints[1];
    cPoints[1] = cPoints[2];
    cPoints[2] = cPoints[3];
    cPoints[3] = new Point(0, 800, 2);
    spline = new Spline(cPoints);
    spline->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    spline->SetViewPort(viewportX, viewportY);
    //lg.push_back(spline);
    //lg.push_back(curve);

//     
    axisX = new Line(new Point(-1000, 0, 1), new Point(1000, 0, 1));
    axisX->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    axisX->SetViewPort(viewportX, viewportY);
//	lg.push_back(axisX);

    axisY = new Line(new Point(0, -1000, 1), new Point(0, 1000, 1));
    axisY->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    axisY->SetViewPort(viewportX, viewportY);
//	lg.push_back(axisY);
//     /**/    
    Point *p = new Point(-150, 100, 1);
    p->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    p->SetViewPort(viewportX, viewportY);
//	lg.push_back(p);
//     
//     for(int i = 0; i < 360; i+=30){
// 	p = new Point(-100,0);
// 	p->SetWindow(win->minX,win->minY,win->maxX,win->maxY);
// 	p->SetViewPort(viewportX,viewportY);	
// 	p->Rotate(i);
// 	lg.push_back(p);
//     }
//     
//     for(int i = 0; i < 360; i+=30){
// 	p = new Point(50,100);
// 	p->SetWindow(win->minX,win->minY,win->maxX,win->maxY);
// 	p->SetViewPort(viewportX,viewportY);	
// 	p->RotateAroundPoint(i,100,150);
// 	lg.push_back(p);
//     }
//     
//     for(int i = 0; i < 360; i+=30){  
//       l = new Line(new Point(-100,0),new Point(-100,100));
//       l->SetWindow(win->minX,win->minY,win->maxX,win->maxY);
//       l->SetViewPort(viewportX,viewportY);
//       l->Rotate(i);
//       l->Scale(i/100);
//       lg.push_back(l);      
//     }
//     
//     for(int i = 0; i < 360; i+=30){  
//       l = new Line(new Point(-100,0),new Point(-100,100));
//       l->SetWindow(win->minX,win->minY,win->maxX,win->maxY);
//       l->SetViewPort(viewportX,viewportY);
//       l->RotateAroundWorld(i);
//       l->Scale(0.5);
//       lg.push_back(l);      
//     }
//       
//     
//    // l->Translate(-100,0);
//    // l->ApplyTransform();
//     lg.push_back(l);
//     
    //obj.GetPoints();

    Line *l = new Line(new Point(-100, 100, 1), new Point(700, -100, 1));
    l->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    l->SetViewPort(viewportX, viewportY);
    //lg.push_back(l);

    l = new Line(new Point(10, 10, 1), new Point(100, 10, 1));
    l->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    l->SetViewPort(viewportX, viewportY);
    //lg.push_back(l);

    l = new Line(new Point(-700, -400, 1), new Point(700, 400, 1));
    l->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    l->SetViewPort(viewportX, viewportY);
//	lg.push_back(l);

    //+++++++++++++++++Criando poligono em 3D
    std::vector < Point * > points(4);

    points[0] = new Point(-70, 70, 1);
    points[0] = new Point(-400, 130, 1);
    points[1] = new Point(-400, 400, 1);
    points[2] = new Point(30, 400, 1);
    points[3] = new Point(30, 130, 1);

    Polygon *pol = new Polygon(points);
    pol->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    pol->SetViewPort(viewportX, viewportY);

    lg.push_back(pol);

    std::vector < Point * > points2(4);
    points2[0] = new Point(-70, 70, 4);
    points2[0] = new Point(-400, 130, 4);
    points2[1] = new Point(-400, 400, 4);
    points2[2] = new Point(30, 400, 4);
    points2[3] = new Point(30, 130, 4);

    Polygon *pol2 = new Polygon(points2);
    pol2->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    pol2->SetViewPort(viewportX, viewportY);

    lg.push_back(pol2);

    for (int i = 0; i < points.size(); i++)
    {
        Line *l = new Line(
                new Point(points[i]->position[0], points[i]->position[1],
                          points[i]->position[2]),
                new Point(points2[i]->position[0], points2[i]->position[1],
                          points2[i]->position[2]));
        l->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
        l->SetViewPort(viewportX, viewportY);
        lg.push_back(l);
    }

    //+++++++++++++++++FIM poligono em 3D
    Polygon *polOBJ = new Polygon(obj.GetPoints());
    polOBJ->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    polOBJ->SetViewPort(viewportX, viewportY);

    //lg.push_back(polOBJ);

    //Pegando a distancia do centro de projeção a origem
    float diffX = win->cop->position[0];
    float diffY = win->cop->position[1];
    float diffZ = win->cop->position[2];

    //Transalado a window
    win->Translate(-diffX, -diffY, -diffZ);
    //Atualizo Objetos
    UpdateObjects();
    //Tranlado o Mundo
    TranslateWorld(-diffX, -diffY, -diffZ);
    RotateWorldInX(-win->angX);
    RotateWorldInY(-win->angY);
    win->Scale();
    ScaleWorld(win->scale);
    //	Polygon *polOBJ = new Polygon(obj.GetPoints());
    //	polOBJ->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    //	polOBJ->SetViewPort(viewportX, viewportY);
    //
    //	lg.push_back(polOBJ);
    //ProjectObjects();
    UpdateObjects();
    ClipObjects();
}

void Canvas::AddPoint(float mouseX, float mouseY)
{
    //g_print("x %i y %i \%i",);
    //	Point *p = new Point(
    //			win->p0->position[0] + mouseX * (win->scale)
    //					- (viewportX / 2) * (win->scale),
    //			win->p0->position[1] - mouseY * (win->scale)
    //					+ (viewportY / 2) * (win->scale), win->p0->position[2]-1);
    //	p->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    //	p->SetViewPort(viewportX, viewportY);
    //	//p->Scale()
    //	lg.push_back(p);
    Point *p = new Point(win->p0->position[0] + mouseX - (viewportX / 2),
                         win->p0->position[1] - mouseY + (viewportY / 2),
                         win->p0->position[2] + 1);

    p->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    p->SetViewPort(viewportX, viewportY);
    p->ScaleWorld(win->scale);
    //p->Scale()
    lg.push_back(p);
    Redraw();
}

void Canvas::AddLine(float x1, float y1, float x2, float y2)
{
    Line *line = new Line(
            new Point(win->p0->position[0] + x1 - (viewportX / 2),
                      win->p0->position[1] - y1 + (viewportY / 2),
                      win->p0->position[2] + 1),
            new Point(win->p0->position[0] + x2 - (viewportX / 2),
                      win->p0->position[1] - y2 + (viewportY / 2),
                      win->p0->position[2] + 1));

    line->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    line->SetViewPort(viewportX, viewportY);
    line->ScaleWorld(win->scale);
    lg.push_back(line);
    Redraw();
}

void Canvas::AddPolygon(std::vector<Point *> mousePoints)
{
    std::vector < Point * > polyPoints;
    for (int i = 0; i < mousePoints.size(); i++)
    {
        polyPoints.push_back(
                new Point(
                        win->p0->position[0] + mousePoints[i]->position[0]
                        - (viewportX / 2),
                        win->p0->position[1] - mousePoints[i]->position[1]
                        + (viewportY / 2), win->p0->position[2] + 1));
    }
    Polygon *pol = new Polygon(polyPoints);
    pol->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    pol->SetViewPort(viewportX, viewportY);
    pol->ScaleWorld(win->scale);
    lg.push_back(pol);
    Redraw();
}

void Canvas::AddCurve(std::vector<Point *> mousePoints)
{
    std::vector < Point * > polyPoints;
    for (int i = 0; i < mousePoints.size(); i++)
    {
        polyPoints.push_back(
                new Point(
                        win->p0->position[0] + mousePoints[i]->position[0]
                        - (viewportX / 2),
                        win->p0->position[1] - mousePoints[i]->position[1]
                        + (viewportY / 2), win->p0->position[2] + 1));
    }
    Spline *spline = new Spline(polyPoints);
    spline->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    spline->SetViewPort(viewportX, viewportY);
    spline->ScaleWorld(win->scale);

    Curve *curve = new Curve(polyPoints);
    curve->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
    curve->SetViewPort(viewportX, viewportY);
    curve->ScaleWorld(win->scale);
    //lg.push_back(curve);
    lg.push_back(spline);
    Redraw();
}

void Canvas::CreatePoint()
{
    state = 0;
    _mousePoints.clear();
}

void Canvas::CreateLine()
{
    state = 1;
    _mousePoints.clear();
}

void Canvas::CreatePolygon()
{
    state = 2;
    _mousePoints.clear();
}

void Canvas::CreateCurve()
{
    state = 3;
    _mousePoints.clear();
}

void Canvas::ChangeViewType()
{
    inPerpective = !inPerpective;
    if (inPerpective)
    {
        g_print("\n\n Project \n\n");
        ProjectObjects();
    }
    else
    {
        UnprojectObjects();
    }
    Redraw();
}

void Canvas::ProjectObjects()
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->Project();
    }
}

void Canvas::UnprojectObjects()
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->Unproject();
    }
}

void Canvas::ClipObjects()
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->Clip();
    }
}

void Canvas::ScaleWorld(float scale)
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->ScaleWorld(scale);
        (*i)->ApplyTransform();
    }
    ClipObjects();
}

void Canvas::RotateWorldInX(float ang)
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->SetVUp(ang);
        (*i)->RotateAroundWorldInX(ang);
        (*i)->ApplyTransform();
    }
    ClipObjects();
}

void Canvas::RotateWorldInY(float ang)
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->SetVUp(ang);
        (*i)->RotateAroundWorldInY(ang);
        (*i)->ApplyTransform();
    }
    ClipObjects();
}

void Canvas::RotateWorldInZ(float ang)
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->SetVUp(ang);
        (*i)->RotateAroundWorldInZ(ang);
        (*i)->ApplyTransform();
    }
    ClipObjects();
}

void Canvas::TranslateWorld(float x, float y, float z)
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        (*i)->Translate(x, y, z);
        (*i)->ApplyTransform();
    }
}

void Canvas::UpdateObjects()
{
    std::list<GeometricObject *>::iterator i;
    for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i)
    {
        //g_print("Min x %f max x %f", win->minX, win->maxX);
        (*i)->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
//     (*i)->SetWindow(-1,-1,1,1);
    }
    //ClipObjects();
}

void Canvas::SetCanvas(GtkWidget *movementGrid)
{

    frame = gtk_frame_new(NULL);

    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);

    gtk_grid_attach(GTK_GRID(movementGrid), frame, 1, 3, 14, 14);

    CanvasInstance->da = gtk_drawing_area_new();

    gtk_widget_set_size_request(CanvasInstance->da, viewportX, viewportY);

    gtk_container_add(GTK_CONTAINER(frame), CanvasInstance->da);

    g_signal_connect(CanvasInstance->da, "draw", G_CALLBACK(draw_cb), NULL);

    g_signal_connect(CanvasInstance->da, "button-press-event",
                     G_CALLBACK(button_press_event_cb), NULL);

    g_signal_connect(CanvasInstance->da, "configure-event",
                     G_CALLBACK(configure_event_cb), NULL);

    gtk_widget_set_events(CanvasInstance->da,
                          gtk_widget_get_events(CanvasInstance->da) | GDK_BUTTON_PRESS_MASK);

    gtk_widget_show(CanvasInstance->da);

    gtk_widget_queue_draw(CanvasInstance->da);

}

void Canvas::Redraw()
{
    clear_surface();

    ClipObjects();

    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::DrawLine(int xi, int yi, int xf, int yf)
{
    //   CanvasInstance->lg.push_back(new Line(new Point(100,300),new Point(800,200)));
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::GoDown()
{
    /*win->Translate(0, -0.5f, 0);

     clear_surface();

     std::list<GeometricObject*>::iterator i;
     for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i) {
     (*i)->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
     }
     */
    clear_surface();
    UnprojectObjects();
    TranslateWorld(0, -1, 0);
    ProjectObjects();
    UpdateObjects();
    ClipObjects();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::GoUp()
{

    /*win->Translate(0, 0.5f, 0);

     clear_surface();

     std::list<GeometricObject*>::iterator i;
     for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i) {
     (*i)->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
     }*/
    //ProjectObjects();
    clear_surface();
    UnprojectObjects();
    TranslateWorld(0, 1, 0);
    ProjectObjects();
    UpdateObjects();
    ClipObjects();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::GoLeft()
{

//   win->minX-=10;
//   win->maxX-=10;
//	win->Translate(-0.5f, 0, 0);
//	clear_surface();
//
//	std::list<GeometricObject*>::iterator i;
//	for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i) {
//		(*i)->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
//	}
//	//ProjectObjects();
//	ClipObjects();
    clear_surface();
    UnprojectObjects();
    TranslateWorld(-1, 0, 0);
    ProjectObjects();
    UpdateObjects();
    ClipObjects();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::GoRight()
{
    /*
     win->minX+=10;
     win->maxX+=10*/
//
//	win->Translate(0.5f, 0, 0);
//
//	clear_surface();
//
//	std::list<GeometricObject*>::iterator i;
//	for (i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i) {
//		(*i)->SetWindow(win->minX, win->minY, win->maxX, win->maxY, win->dop);
//	}
//	//ProjectObjects();
//	ClipObjects();
    clear_surface();
    UnprojectObjects();
    TranslateWorld(1, 0, 0);
    ProjectObjects();
    UpdateObjects();
    ClipObjects();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::RotateZ()
{
    float diffX = win->p0->position[0];
    float diffY = win->p0->position[1];
    float diffZ = win->p0->position[2];
    win->Translate(-win->p0->position[0], -win->p0->position[1],
                   -win->p0->position[2]);

    UpdateObjects();
    TranslateWorld(-diffX, -diffY, -diffZ);
    RotateWorldInY(-90);
    UpdateObjects();
    clear_surface();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::ZoomIn()
{
//   g_print("Translate in x %f \n",win->p0->position[0]);
    /*
     win->minX+=1;
     win->maxX-=1;
     win->minY+=1;
     win->maxY-=1;

     clear_surface();

     std::list<GeometricObject*>::iterator i;
     for(i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i){
     (*i)->SetWindow(win->minX,win->minY,win->maxX,win->maxY);
     }
     */
    float diffX = win->p0->position[0];
    float diffY = win->p0->position[1];
    float diffZ = win->p0->position[2];
    win->Translate(-win->p0->position[0], -win->p0->position[1],
                   -win->p0->position[2]);
    UpdateObjects();
    TranslateWorld(-diffX, -diffY, -diffZ);
    RotateWorldInZ(-90);
    //win->Scale();
    //ScaleWorld(win->scale);
    UpdateObjects();
    clear_surface();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::ZoomOut()
{
    /*win->minX-=1;
     win->maxX+=1;
     win->minY-=1;
     win->maxY+=1;

     clear_surface();

     std::list<GeometricObject*>::iterator i;
     for(i = CanvasInstance->lg.begin(); i != CanvasInstance->lg.end(); ++i){
     (*i)->SetWindow(win->minX,win->minY,win->maxX,win->maxY);
     }*/
    float diffX = win->p0->position[0];
    float diffY = win->p0->position[1];
    float diffZ = win->p0->position[2];
    win->Translate(-win->p0->position[0], -win->p0->position[1],
                   -win->p0->position[2]);
    UpdateObjects();
    TranslateWorld(-diffX, -diffY, -diffZ);
    RotateWorldInZ(90);
    //win->Scale();
    //ScaleWorld(win->scale);
    UpdateObjects();
    clear_surface();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::ZoomPlus()
{

    ScaleWorld(1.5f);
    UpdateObjects();
    clear_surface();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}

void Canvas::ZoomMinus()
{
    ScaleWorld(0.5f);
    UpdateObjects();
    clear_surface();
    draw_brush(CanvasInstance->da);
    gtk_widget_queue_draw(CanvasInstance->da);
}