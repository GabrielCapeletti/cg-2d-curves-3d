#include "window.h"

Window::Window(Point *initialPos, float rotX, float rotY, float _scale, float w, float h)
{
    p0 = initialPos;
    float angInRadsX = rotX * (M_PI / 180);
    float angInRadsY = rotY * (M_PI / 180);
    //v = new Point(sin(angInRads), cos(angInRads), 0);
    //u = new Point(cos(angInRads), sin(angInRads), 0);

    angX = angInRadsX;
    angY = angInRadsY;

    dop = 1;

    vpn = new Point(0, 0, 1);

    cop = new Point(vpn->position[0], vpn->position[1], vpn->position[2] - dop);

    scale = 2 / w;

    minX = p0->position[0] - w / 2;
    maxX = p0->position[0] + w / 2;
    minY = p0->position[1] - h / 2;
    maxY = p0->position[1] + h / 2;
}

void Window::Scale()
{
    minX = -1;
    maxX = 1;
    minY = -1;
    maxY = 1;
    dop *= scale;
}

void Window::Translate(float x, float y, int z)
{
    p0->Translate(x, y, z);
    p0->ApplyTransform();
    minX += x;
    maxX += x;
    minY += y;
    maxY += y;
}
