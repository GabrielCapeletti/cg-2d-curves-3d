#include "line.h"

Line::Line(Point *pi, Point *pf)
{

    _pi = pi;
    _pf = pf;
    RC[0] = 0x08;
    RC[1] = 0x04;
    RC[2] = 0x02;
    RC[3] = 0x01;
    RCP1 = 0x00;
    RCP2 = 0x00;
    clipped = false;
}

void Line::RotateAroundPointInZ(float ang, float x, float y, float z)
{
    float averageX = (_pi->position[0] + _pf->position[0]) / 2;
    float averageY = (_pi->position[1] + _pf->position[1]) / 2;
    float averageZ = (_pi->position[2] + _pf->position[2]) / 2;
    Translate(-averageX, -averageY, -averageZ);

    _pi->RotateInZ(ang);
    _pf->RotateInZ(ang);

    Translate(averageX, averageY, averageZ);
    ApplyTransform();
}

void Line::RotateAroundWorldInZ(float ang)
{

    _pi->RotateInZ(ang);
    _pf->RotateInZ(ang);

    ApplyTransform();
}

void Line::RotateInZ(float ang)
{
    float averageX = (_pi->position[0] + _pf->position[0]) / 2;
    float averageY = (_pi->position[1] + _pf->position[1]) / 2;
    float averageZ = (_pi->position[2] + _pf->position[2]) / 2;
    Translate(-averageX, -averageY, -averageZ);

    _pi->RotateInZ(ang);
    _pf->RotateInZ(ang);

    Translate(averageX, averageY, averageZ);
    ApplyTransform();
}

void Line::RotateAroundPointInY(float ang, float x, float y, float z)
{
    float averageX = (_pi->position[0] + _pf->position[0]) / 2;
    float averageY = (_pi->position[1] + _pf->position[1]) / 2;
    float averageZ = (_pi->position[2] + _pf->position[2]) / 2;
    Translate(-averageX, -averageY, -averageZ);

    _pi->RotateInY(ang);
    _pf->RotateInY(ang);

    Translate(averageX, averageY, averageZ);
    ApplyTransform();
}

void Line::RotateAroundWorldInY(float ang)
{
    _pi->RotateInY(ang);
    _pf->RotateInY(ang);

    ApplyTransform();
}

void Line::Project()
{
    _pi->SetWindow(xWinMin, yWinMin, xWinMax, yWinMax, dop);
    _pi->Project();
    _pf->SetWindow(xWinMin, yWinMin, xWinMax, yWinMax, dop);
    _pf->Project();
}

void Line::Unproject()
{
    _pi->SetWindow(xWinMin, yWinMin, xWinMax, yWinMax, dop);
    _pi->Unproject();
    _pf->SetWindow(xWinMin, yWinMin, xWinMax, yWinMax, dop);
    _pf->Unproject();
}

void Line::RotateInY(float ang)
{
    float averageX = (_pi->position[0] + _pf->position[0]) / 2;
    float averageY = (_pi->position[1] + _pf->position[1]) / 2;
    float averageZ = (_pi->position[2] + _pf->position[2]) / 2;
    Translate(-averageX, -averageY, -averageZ);

    _pi->RotateInY(ang);
    _pf->RotateInY(ang);

    Translate(averageX, averageY, averageZ);
    ApplyTransform();
}

void Line::RotateAroundPointInX(float ang, float x, float y, float z)
{
    float averageX = (_pi->position[0] + _pf->position[0]) / 2;
    float averageY = (_pi->position[1] + _pf->position[1]) / 2;
    float averageZ = (_pi->position[2] + _pf->position[2]) / 2;
    Translate(-averageX, -averageY, -averageZ);

    _pi->RotateInX(ang);
    _pf->RotateInX(ang);

    Translate(averageX, averageY, averageZ);
    ApplyTransform();
}

void Line::RotateAroundWorldInX(float ang)
{
    _pi->RotateInX(ang);
    _pf->RotateInX(ang);

    ApplyTransform();
}

void Line::RotateInX(float ang)
{
    float averageX = (_pi->position[0] + _pf->position[0]) / 2;
    float averageY = (_pi->position[1] + _pf->position[1]) / 2;
    float averageZ = (_pi->position[2] + _pf->position[2]) / 2;
    Translate(-averageX, -averageY, -averageZ);

    _pi->RotateInX(ang);
    _pf->RotateInX(ang);

    Translate(averageX, averageY, averageZ);
    ApplyTransform();
}

void Line::ScaleWorld(float scale)
{
    _pi->Scale(scale);
    _pf->Scale(scale);
    ApplyTransform();
    dop *= scale;
}

void Line::Scale(float scale)
{
    float averageX = (_pi->position[0] + _pf->position[0]) / 2;
    float averageY = (_pi->position[1] + _pf->position[1]) / 2;
    float averageZ = (_pi->position[2] + _pf->position[2]) / 2;
    Translate(-averageX, -averageY, -averageZ);
    _pi->Scale(scale);
    _pf->Scale(scale);
    ApplyTransform();
    Translate(averageX, averageY, averageZ);
    dop *= scale;
    //ApplyTransform();
}

void Line::Translate(float x, float y, float z)
{
    _pi->Translate(x, y, z);
    _pf->Translate(x, y, z);
    ApplyTransform();
}

void Line::ApplyTransform()
{
    _pi->ApplyTransform();
    _pf->ApplyTransform();
}

bool Line::IsBitOne(int number, int index, int mask)
{
    int res = number & mask;
    res = res >> index;
    return (res == 1);
}

Line *Line::clippingLiang()
{
    isShowing = true;
    clipped = true;
    Point *pointI = _pi;
    Point *pointF = _pf;
    dx = _pf->position[0] - _pi->position[0];
    dy = _pf->position[1] - _pi->position[1];
    vector<float> p = calculateP();
    vector<float> q = calculateQ();
    vector<float> r = calculateR(q, p);
    float u1 = 0;
    float u2 = 1;

    for (int k = 0; k < 4; k++)
    {
        // a linha vem de fora para dentro
        if (p[k] < 0)
        {
            if (r[k] > u1)
            {
                u1 = r[k];
            }
        }
        else if (p[k] > 0)
        {
            if (r[k] < u2)
            {
                u2 = r[k];
            }
        }
    }
    //não desenha nada!!
    if (u1 > u2)
    {
        isShowing = false;
        return 0;
    }

//se si > si2 a reta esta fora

    if (u1 != 0)
    {
        pointI = new Point(_pi->position[0] + u1 * dx,
                           _pi->position[1] + u1 * dy, 0);
    }
    else
    {
        pointI = _pi;
    }
    if (u2 != 1)
    {
        pointF = new Point(_pi->position[0] + u2 * dx,
                           _pi->position[1] + u2 * dy, 0);
    }
    else
    {
        pointF = _pf;
    }

    return newLine = new Line(pointI, pointF);

}

vector<float> Line::calculateP()
{
    vector<float> p;
    p.push_back(-dx);
    p.push_back(dx);
    p.push_back(-dy);
    p.push_back(dy);
    return p;
}

vector<float> Line::calculateQ()
{
    vector<float> q;
    q.push_back(_pi->position[0] - (xWinMin));
    q.push_back(xWinMax - (_pi->position[0]));
    q.push_back(_pi->position[1] - (yWinMin));
    q.push_back(yWinMax - (_pi->position[1]));

    return q;
}

vector<float> Line::calculateR(vector<float> q, vector<float> p)
{

    vector<float> r;
    r.push_back(q[0] / p[0]);
    r.push_back(q[1] / p[1]);
    r.push_back(q[2] / p[2]);
    r.push_back(q[3] / p[3]);

    return r;
}

void Line::Clip()
{
    CohenSuthernClipping();
}

void Line::CohenSuthernClipping()
{

    float p1X = _pi->position[0];
    float p1Y = _pi->position[1];
    float p2X = _pf->position[0];
    float p2Y = _pf->position[1];

    clippedLine = new Line(new Point(p1X, p1Y, 0), new Point(p2X, p2Y, 0));

    RCP1 = 0x00;
    RCP2 = 0x00;

    if (_pi->position[0] < xWinMin)
    {
        RCP1 = RCP1 | 0x01;
    }
    if (_pi->position[0] > xWinMax)
    {
        RCP1 = RCP1 | 0x02;
    }
    if (_pi->position[1] < yWinMin)
    {
        RCP1 = RCP1 | 0x04;
    }
    if (_pi->position[1] > yWinMax)
    {
        RCP1 = RCP1 | 0x08;
    }

    if (_pf->position[0] < xWinMin)
    {
        RCP2 = RCP2 | 0x01;
    }
    if (_pf->position[0] > xWinMax)
    {
        RCP2 = RCP2 | 0x02;
    }
    if (_pf->position[1] < yWinMin)
    {
        RCP2 = RCP2 | 0x04;
    }
    if (_pf->position[1] > yWinMax)
    {
        RCP2 = RCP2 | 0x08;
    }

    isShowing = true;
    clipped = true;

    if (RCP1 == 0 && RCP2 == 0)
    {
        clippedLine = new Line(new Point(p1X, p1Y, 0), new Point(p2X, p2Y, 0));
        return;
    }
    int comp = RCP1 & RCP2;
    if (comp != 0)
    {
        isShowing = false;
        return;
    }

    float m;
    bool coefIsInfi = false;

    if (_pf->position[0] != _pi->position[0])
    {
        m = (_pf->position[1] - _pi->position[1])
            / (_pf->position[0] - _pi->position[0]);
    }
    else
    {
        coefIsInfi = true;
    }

    if (IsBitOne(RCP1, 3, 0x08))
    {
        p1Y = yWinMax;
        if (!coefIsInfi)
        {
            float newp1X = (_pi->position[0]
                            + (1 / m) * (yWinMax - _pi->position[1]));
            if ((newp1X < xWinMin || newp1X > xWinMax))
            {
                if (RCP1 == 8)
                {
                    isShowing = false;
                    return;
                }
            }
            else
            {
                p1X = newp1X;
            }
        }
        else
        {
            p1X = _pi->position[0];
        }
    }

    if (IsBitOne(RCP1, 2, 0x04))
    {
        p1Y = yWinMin;
        if (!coefIsInfi)
        {
            float newp1X = (_pi->position[0]
                            + (1 / m) * (yWinMin - _pi->position[1]));
            if (newp1X < xWinMin || newp1X > xWinMax)
            {
                if (RCP1 == 4)
                {
                    isShowing = false;
                    return;
                }
            }
            else
            {
                p1X = newp1X;
            }
        }
        else
        {
            p1X = _pi->position[0];
        }
    }

    if (IsBitOne(RCP1, 1, 0x02))
    {
        p1X = xWinMax;
        float newp1Y = (_pi->position[1] + (m) * (xWinMax - _pi->position[0]));
        if (newp1Y < yWinMin || newp1Y > yWinMax)
        {
            if (RCP1 == 2)
            {
                isShowing = false;
                return;
            }
        }
        else
        {
            p1Y = newp1Y;
        }
    }

    if (IsBitOne(RCP1, 0, 0x01))
    {
        p1X = xWinMin;
        float newp1Y = (_pi->position[1] + (m) * (xWinMin - _pi->position[0]));
        if (newp1Y < yWinMin || newp1Y > yWinMax)
        {
            if (RCP1 == 1)
            {
                isShowing = false;
                return;
            }
        }
        else
        {
            p1Y = newp1Y;
        }
    }

    if (IsBitOne(RCP2, 3, 0x08))
    {
        p2Y = yWinMax;
        if (!coefIsInfi)
        {
            float newp2X = (_pf->position[0]
                            + (1 / m) * (yWinMax - _pf->position[1]));
            if (newp2X < xWinMin || newp2X > xWinMax)
            {
                if (RCP2 == 8)
                {
                    isShowing = false;
                    return;
                }
            }
            else
            {
                p2X = newp2X;
            }
        }
        else
        {
            p2X = _pf->position[0];
        }
    }

    if (IsBitOne(RCP2, 2, 0x04))
    {
        p2Y = yWinMin;
        if (!coefIsInfi)
        {
            float newp2X = (_pf->position[0]
                            + (1 / m) * (yWinMin - _pf->position[1]));
            if (newp2X < xWinMin || newp2X > xWinMax)
            {
                if (RCP2 == 4)
                {
                    isShowing = false;
                    return;
                }
            }
            else
            {
                p2X = newp2X;
            }
        }
        else
        {
            p2X = _pf->position[0];
        }
    }

    if (IsBitOne(RCP2, 1, 0x02))
    {
        p2X = xWinMax;
        float newp2Y = (_pf->position[1] + (m) * (xWinMax - _pf->position[0]));
        if (newp2Y < yWinMin || newp2Y > yWinMax)
        {
            if (RCP2 == 2)
            {
                isShowing = false;
                return;
            }
        }
        else
        {
            p2Y = newp2Y;
        }
    }

    if (IsBitOne(RCP2, 0, 0x01))
    {
        p2X = xWinMin;
        float newp2Y = (_pf->position[1] + (m) * (xWinMin - _pf->position[0]));
        if (newp2Y < yWinMin || newp2Y > yWinMax)
        {
            if (RCP2 == 1)
            {
                isShowing = false;
                return;
            }
        }
        else
        {
            p2Y = newp2Y;
        }
    }

    clippedLine = new Line(new Point(p1X, p1Y, 1), new Point(p2X, p2Y, 1));
}

void Line::Draw(cairo_t *cr)
{

    if (!isShowing)
    {
        return;
    }

    if (clipped)
    {
        float divWinXI = (clippedLine->_pi->position[0] - xWinMin)
                         / (xWinMax - xWinMin);
        float divWinYI = 1
                         - ((clippedLine->_pi->position[1] - yWinMin)
                            / (yWinMax - yWinMin));

        int xVpI = (divWinXI) * (xVpMax - 20) + 10;
        int yVpI = (divWinYI) * (yVpMax - 20) + 10;

        float divWinXF = (clippedLine->_pf->position[0] - xWinMin)
                         / (xWinMax - xWinMin);
        float divWinYF = 1
                         - ((clippedLine->_pf->position[1] - yWinMin)
                            / (yWinMax - yWinMin));

        int xVpF = (divWinXF) * (xVpMax - 20) + 10;
        int yVpF = (divWinYF) * (yVpMax - 20) + 10;

        // g_print("Point at Vp x %i \n", xVpI);
        cairo_move_to(cr, xVpI, yVpI);
        cairo_line_to(cr, xVpF, yVpF);

        return;
    }

    //cairo_set_line_width (cr, 0.9);

    float divWinXI = (_pi->position[0] - xWinMin) / (xWinMax - xWinMin);
    float divWinYI = 1 - ((_pi->position[1] - yWinMin) / (yWinMax - yWinMin));

    int xVpI = (divWinXI) * (xVpMax - 20) + 10;
    int yVpI = (divWinYI) * (yVpMax - 20) + 10;

    float divWinXF = (_pf->position[0] - xWinMin) / (xWinMax - xWinMin);
    float divWinYF = 1 - ((_pf->position[1] - yWinMin) / (yWinMax - yWinMin));

    int xVpF = (divWinXF) * (xVpMax - 20) + 10;
    int yVpF = (divWinYF) * (yVpMax - 20) + 10;

    // g_print("Point at Vp x %i \n", xVpI);
    cairo_move_to(cr, xVpI, yVpI);
    cairo_line_to(cr, xVpF, yVpF);

    //g_print("Creating Line\n");
}
