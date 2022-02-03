#include "curve.h"

Curve::Curve(std::vector<Point *> points)
{
    _points = points;
    bezier(_points);
}

void Curve::bezier(vector<Point *> p)
{
    Point *newPoint;

    matrixM[0][0] = -1;
    matrixM[1][0] = 3;
    matrixM[2][0] = -3;
    matrixM[3][0] = 1;
    matrixM[0][1] = 3;
    matrixM[1][1] = -6;
    matrixM[2][1] = 3;
    matrixM[3][1] = 0;
    matrixM[0][2] = -3;
    matrixM[1][2] = 3;
    matrixM[2][2] = 0;
    matrixM[3][2] = 0;
    matrixM[0][3] = 1;
    matrixM[1][3] = 0;
    matrixM[2][3] = 0;
    matrixM[3][3] = 0;
    vector<float> T;
    for (float t = 0.00; t < 1.00; t += 0.1)
    {
        //g_print("t %f  \n", t);
        T.clear();
        T.push_back(pow(t, 3));
        T.push_back(pow(t, 2));
        T.push_back(t);
        T.push_back(1);
        newPoint = multiplay(T, matrixM, p);
        _pointsCurve.push_back(newPoint);

    }

    poliClip = new Polygon(_pointsCurve);
    poliClip->conectFirstAndLast = false;
}

Point *Curve::multiplay(vector<float> T, float matrixM[4][4],
                        vector<Point *> p)
{
    vector<float> r;
    for (int j = 0; j < T.size(); j++)
    {
        r.push_back(0);
    }
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < sizeof(matrixM); i++)
    { // para cada coluna de M
        for (int j = 0; j < T.size(); j++)
        { //para cada linha de M
            //g_print("linha %d coluna %d \n", j, i);
            //g_print("t %f \n",T[j]);
            //g_print("matriz %f \n",matrixM[j][i]);
            r[i] = T[j] * matrixM[j][i] + r[i];
            //g_print("r %f \n",r[i]);
        }
        x = r[i] * p[i]->position[0] + x;
        y = r[i] * p[i]->position[1] + y;
        z = r[i] * p[i]->position[2] + z;

    }
    //g_print("x %f y %f\n", x, y);
    return new Point(x, y, z);

}

void Curve::Clip()
{
    poliClip->Clip();
}

void Curve::SetWindow(float _xWinMin, float _yWinMin, float _xWinMax,
                      float _yWinMax, float _dop)
{
    xWinMax = _xWinMax;
    xWinMin = _xWinMin;
    yWinMax = _yWinMax;
    yWinMin = _yWinMin;
    dop = _dop;
    poliClip->SetWindow(_xWinMin, _yWinMin, _xWinMax, _yWinMax, _dop);
}

void Curve::Project()
{
    poliClip->Project();
}

void Curve::Unproject()
{
    poliClip->Unproject();
}


void Curve::SetViewPort(float _xVpMax, float _yVpMax)
{
    xVpMax = _xVpMax;
    yVpMax = _yVpMax;
    poliClip->SetViewPort(_xVpMax, _yVpMax);
}

void Curve::RotateInX(float ang)
{
    poliClip->RotateInX(ang);
}

void Curve::RotateAroundWorldInX(float ang)
{
    poliClip->RotateAroundWorldInX(ang);
}

void Curve::RotateAroundPointInX(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInX(ang, x, y, z);
}

void Curve::RotateInY(float ang)
{
    poliClip->RotateInY(ang);
}

void Curve::RotateAroundWorldInY(float ang)
{
    poliClip->RotateAroundWorldInY(ang);
}

void Curve::RotateAroundPointInY(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInY(ang, x, y, z);
}

void Curve::RotateInZ(float ang)
{
    poliClip->RotateInZ(ang);
}

void Curve::RotateAroundWorldInZ(float ang)
{
    poliClip->RotateAroundWorldInZ(ang);
}

void Curve::RotateAroundPointInZ(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInZ(ang, x, y, z);
}

void Curve::ScaleWorld(float scale)
{
    poliClip->ScaleWorld(scale);
}

void Curve::Scale(float scale)
{
    poliClip->Scale(scale);
}

void Curve::Translate(float x, float y, float z)
{
    poliClip->Translate(x, y, z);
}

void Curve::ApplyTransform()
{
    poliClip->ApplyTransform();
}

void Curve::Draw(cairo_t *cr)
{
    poliClip->Draw(cr);
}

float Curve::TransformToVpX(float x)
{
    float divWin = (x - xWinMin) / (xWinMax - xWinMin);
    float xVp = (divWin) * (xVpMax - 20) + 10;

    return xVp;
}

float Curve::TransformToVpY(float y)
{
    float divWinY = 1 - ((y - yWinMin) / (yWinMax - yWinMin));
    float yVp = (divWinY) * (yVpMax - 20) + 10;
    //yVp = posWinY*(yVpMax - yVpMin);

    return yVp;
}

