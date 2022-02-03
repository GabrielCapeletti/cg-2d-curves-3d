#include "spline.h"

Spline::Spline(std::vector<Point *> points)
{
    delta = 0.01;
    _points = points;

    for (int i = 0; i < 4; i++)
    {
        Gx[i] = _points[i]->position[0];
        Gy[i] = _points[i]->position[1];
    }
//	g_print("G vector x : %f , %f, %f, %f  \n", Gx[0], Gx[1], Gx[2], Gx[3]);
//	g_print("G vector y : %f , %f, %f, %f  \n", Gy[0], Gy[1], Gy[2], Gy[3]);

    matrixMBS[0][0] = (-1.0f / 6.0f);
    matrixMBS[1][0] = 1.0f / 2.0f;
    matrixMBS[2][0] = -1.0f / 2.0f;
    matrixMBS[3][0] = 1.0f / 6.0f;
    matrixMBS[0][1] = 1.0f / 2.0f;
    matrixMBS[1][1] = -1.0f;
    matrixMBS[2][1] = 0;
    matrixMBS[3][1] = 2.0f / 3.0f;
    matrixMBS[0][2] = -1.0f / 2.0f;
    matrixMBS[1][2] = 1.0f / 2.0f;
    matrixMBS[2][2] = 1.0f / 2.0f;
    matrixMBS[3][2] = 1.0f / 6.0f;
    matrixMBS[0][3] = 1.0f / 6.0f;
    matrixMBS[1][3] = 0;
    matrixMBS[2][3] = 0;
    matrixMBS[3][3] = 0;

    matrixE[0][0] = 0;
    matrixE[1][0] = pow(delta, 3);
    matrixE[2][0] = 6 * pow(delta, 3);
    matrixE[3][0] = 6 * pow(delta, 3);
    matrixE[0][1] = 0;
    matrixE[1][1] = pow(delta, 2);
    matrixE[2][1] = 2 * pow(delta, 2);
    matrixE[3][1] = 0;
    matrixE[0][2] = 0;
    matrixE[1][2] = delta;
    matrixE[2][2] = 0;
    matrixE[3][2] = 0;
    matrixE[0][3] = 1;
    matrixE[1][3] = 0;
    matrixE[2][3] = 0;
    matrixE[3][3] = 0;

    Cx = MultiplyCoefMatrix(matrixMBS, Gx);
    Cy = MultiplyCoefMatrix(matrixMBS, Gy);

//	g_print("Coef vector x : %f , %f, %f, %f  \n", Cx[0], Cx[1], Cx[2], Cx[3]);
//	g_print("Coef vector y : %f , %f, %f, %f  \n", Cy[0], Cy[1], Cy[2], Cy[3]);

    forwardDiffX = MultiplyCoefMatrix(matrixE, Cx);
    forwardDiffY = MultiplyCoefMatrix(matrixE, Cy);

    ForwardDiferences(100, forwardDiffX[0], forwardDiffX[1], forwardDiffX[2],
                      forwardDiffX[3], forwardDiffY[0], forwardDiffY[1], forwardDiffY[2],
                      forwardDiffY[3]);

    poliClip = new Polygon(_pointsCurve);
    poliClip->conectFirstAndLast = false;
}

float *Spline::MultiplyCoefMatrix(float m1[4][4], float m2[4])
{
    float cellValue = 0;
    float *resultMatrix = new float[4];

    for (int i = 0; i < 4; i++)
    {
        cellValue = 0;
        for (int k = 0; k < 4; k++)
        {
            cellValue += m1[i][k] * m2[k];
            //	g_print("result matrix multiply : %f * %f = %f \n", m1[i][k],m2[k],(m1[i][k] * m2[k]));
        }

        resultMatrix[i] = cellValue;

    }
    return resultMatrix;
}

void Spline::ForwardDiferences(int n, float x, float dx, float dx2, float dx3,
                               float y, float dy, float dy2, float dy3)
{
    oldX = x;
    oldY = y;

//	g_print("forward vector x : %f , %f, %f, %f  \n", x, dx, dx2, dx3);
//	g_print("forward vector y : %f , %f, %f, %f  \n", y, dy, dy2, dy3);
    _pointsCurve.push_back(new Point(oldX, oldY, 1));
    for (int i = 0; i < n; i++)
    {
        x = x + dx;
        dx = dx + dx2;
        dx2 = dx2 + dx3;

        y = y + dy;
        dy = dy + dy2;
        dy2 = dy2 + dy3;

        _pointsCurve.push_back(new Point(x, y, 1));
        //g_print("point vector : %f %f \n", x, y);
        oldX = x;
        oldY = y;
    }
}

void Spline::Clip()
{
    poliClip->Clip();
}

void Spline::SetWindow(float _xWinMin, float _yWinMin, float _xWinMax,
                       float _yWinMax, float _dop)
{
    xWinMax = _xWinMax;
    xWinMin = _xWinMin;
    yWinMax = _yWinMax;
    yWinMin = _yWinMin;
    dop = _dop;
    poliClip->SetWindow(_xWinMin, _yWinMin, _xWinMax, _yWinMax, _dop);
}

void Spline::SetViewPort(float _xVpMax, float _yVpMax)
{
    xVpMax = _xVpMax;
    yVpMax = _yVpMax;
    poliClip->SetViewPort(_xVpMax, _yVpMax);
}

void Spline::RotateInX(float ang)
{
    poliClip->RotateInX(ang);
}

void Spline::RotateAroundWorldInX(float ang)
{
    poliClip->RotateAroundWorldInX(ang);
}

void Spline::RotateAroundPointInX(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInX(ang, x, y, z);
}

void Spline::RotateInY(float ang)
{
    poliClip->RotateInY(ang);
}

void Spline::RotateAroundWorldInY(float ang)
{
    poliClip->RotateAroundWorldInY(ang);
}

void Spline::RotateAroundPointInY(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInY(ang, x, y, z);
}

void Spline::RotateInZ(float ang)
{
    poliClip->RotateInZ(ang);
}

void Spline::RotateAroundWorldInZ(float ang)
{
    poliClip->RotateAroundWorldInZ(ang);
}

void Spline::RotateAroundPointInZ(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInZ(ang, x, y, z);
}

void Spline::ScaleWorld(float scale)
{
    poliClip->ScaleWorld(scale);
}

void Spline::Scale(float scale)
{
    poliClip->Scale(scale);
}

void Spline::Translate(float x, float y, float z)
{
    poliClip->Translate(x, y, z);
}

void Spline::ApplyTransform()
{
    poliClip->ApplyTransform();
}

void Spline::Project()
{
    poliClip->Project();
}

void Spline::Unproject()
{
    poliClip->Unproject();
}

void Spline::Draw(cairo_t *cr)
{
    poliClip->Draw(cr);
}

float Spline::TransformToVpX(float x)
{
    float divWin = (x - xWinMin) / (xWinMax - xWinMin);
    float xVp = (divWin) * (xVpMax - 20) + 10;

    return xVp;
}

float Spline::TransformToVpY(float y)
{
    float divWinY = 1 - ((y - yWinMin) / (yWinMax - yWinMin));
    float yVp = (divWinY) * (yVpMax - 20) + 10;
    //yVp = posWinY*(yVpMax - yVpMin);

    return yVp;
}
