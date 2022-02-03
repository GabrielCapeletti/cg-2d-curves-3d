#include "superSpline.h"

superSpline::superSpline(Point *G[4][4])
{
    delta_s = 0.05;
    delta_t = 0.05;
    float ns = 20;
    float nt = 20;

    defineGxGyGz(G);
    createMBMatrix();
    calculateCoefficients();
    createDeltaMatrices(delta_s, delta_t);
    createForwardDiffMatrices();

    Polygon *pol;

    for (int i = 0; i < ns; i++)
    {
        pol = drawForwardDiffSup(ns, forwardDiffX[0][0], forwardDiffX[0][1],
                                 forwardDiffX[0][2], forwardDiffX[0][3], forwardDiffY[0][0],
                                 forwardDiffY[0][1], forwardDiffY[0][2], forwardDiffY[0][3],
                                 forwardDiffZ[0][0], forwardDiffZ[0][1], forwardDiffZ[0][2],
                                 forwardDiffZ[0][3]);
        polygons.push_back(pol);
        UpdateForwardDiffMatrices();
    }

    createForwardDiffMatrices();

    transpose(forwardDiffX);
    transpose(forwardDiffY);
    transpose(forwardDiffZ);

    for (int i = 0; i < nt; i++)
    {
        pol = drawForwardDiffSup(ns, forwardDiffX[0][0], forwardDiffX[0][1],
                                 forwardDiffX[0][2], forwardDiffX[0][3], forwardDiffY[0][0],
                                 forwardDiffY[0][1], forwardDiffY[0][2], forwardDiffY[0][3],
                                 forwardDiffZ[0][0], forwardDiffZ[0][1], forwardDiffZ[0][2],
                                 forwardDiffZ[0][3]);
        polygons.push_back(pol);
        UpdateForwardDiffMatrices();
    }
}

void superSpline::defineGxGyGz(Point *G[4][4])
{
    Gx = new float *[4];
    Gy = new float *[4];
    Gz = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        Gx[i] = new float[4];
        Gy[i] = new float[4];
        Gz[i] = new float[4];
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Gx[i][j] = G[i][j]->position[0];
            Gy[i][j] = G[i][j]->position[1];
            Gz[i][j] = G[i][j]->position[2];

        }
    }

}

Polygon *superSpline::drawForwardDiffSup(float n, float x, float dx, float dx2,
                                         float dx3, float y, float dy, float dy2, float dy3, float z, float dz,
                                         float dz2, float dz3)
{
    oldX = x;
    oldY = y;
    oldZ = z;
    Polygon *poliClip;
    std::vector < Point * > _pointsCurve;
//	g_print("forward vector x : %f , %f, %f, %f  \n", x, dx, dx2, dx3);
//	g_print("forward vector y : %f , %f, %f, %f  \n", y, dy, dy2, dy3);
    _pointsCurve.push_back(new Point(oldX, oldY, oldZ));
    for (int i = 0; i < n; i++)
    {
        x = x + dx;
        dx = dx + dx2;
        dx2 = dx2 + dx3;

        y = y + dy;
        dy = dy + dy2;
        dy2 = dy2 + dy3;

        z = z + dz;
        dz = dz + dz2;
        dz2 = dz2 + dz3;

        _pointsCurve.push_back(new Point(x, y, z));
        //g_print("point vector : %f %f \n", x, y);
        oldX = x;
        oldY = y;
        oldZ = z;
    }
    poliClip = new Polygon(_pointsCurve);
    poliClip->conectFirstAndLast = false;
    return poliClip;
}

void superSpline::calculateCoefficients()
{
    float **ans = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        ans[i] = new float[4];
    }

    ans = multMatrix4x4(matrixMBS, Gx);
    Cx = multMatrix4x4(ans, matrixMBS);

    ans = multMatrix4x4(matrixMBS, Gy);
    Cy = multMatrix4x4(ans, matrixMBS);

    ans = multMatrix4x4(matrixMBS, Gz);
    Cz = multMatrix4x4(ans, matrixMBS);

}

void superSpline::UpdateForwardDiffMatrices()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            forwardDiffX[i][j] = forwardDiffX[i][j] + forwardDiffX[i + 1][j];
            forwardDiffY[i][j] = forwardDiffY[i][j] + forwardDiffY[i + 1][j];
            forwardDiffZ[i][j] = forwardDiffZ[i][j] + forwardDiffZ[i + 1][j];
        }
    }

}

void superSpline::createForwardDiffMatrices()
{
    float **ans = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        ans[i] = new float[4];
    }
    // DD(x)
    ans = multMatrix4x4(Es, Cx);
    forwardDiffX = multMatrix4x4(ans, Et);
    // DD(y)
    ans = multMatrix4x4(Es, Cy);
    forwardDiffY = multMatrix4x4(ans, Et);
    // DD(z)
    ans = multMatrix4x4(Es, Cz);
    forwardDiffZ = multMatrix4x4(ans, Et);
}

void superSpline::createMBMatrix()
{
    matrixMBS = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        matrixMBS[i] = new float[4];
    }
    matrixMBS[0][0] = -1;
    matrixMBS[1][0] = 3;
    matrixMBS[2][0] = -3;
    matrixMBS[3][0] = 1;
    matrixMBS[0][1] = 3;
    matrixMBS[1][1] = -6;
    matrixMBS[2][1] = 3;
    matrixMBS[3][1] = 0;
    matrixMBS[0][2] = -3;
    matrixMBS[1][2] = 3;
    matrixMBS[2][2] = 0;
    matrixMBS[3][2] = 0;
    matrixMBS[0][3] = 1;
    matrixMBS[1][3] = 0;
    matrixMBS[2][3] = 0;
    matrixMBS[3][3] = 0;

}

void superSpline::createDeltaMatrices(float ds, float dt)
{
    Es = new float *[4];
    Et = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        Es[i] = new float[4];
        Et[i] = new float[4];
    }
    Es[0][0] = 0;
    Es[0][1] = 0;
    Es[0][2] = 0;
    Es[0][3] = 1;

    Es[1][0] = delta_s * delta_s * delta_s;
    Es[1][1] = delta_s * delta_s;
    Es[1][2] = delta_s;
    Es[1][3] = 0;

    Es[2][0] = 6 * delta_s * delta_s * delta_s;
    Es[2][1] = 2 * delta_s * delta_s;
    Es[2][2] = 0;
    Es[2][3] = 0;

    Es[3][0] = 6 * delta_s * delta_s * delta_s;
    Es[3][1] = 0;
    Es[3][2] = 0;
    Es[3][3] = 0;

    // Delta t
    Et[0][0] = 0;
    Et[0][1] = 0;
    Et[0][2] = 0;
    Et[0][3] = 1;

    Et[1][0] = delta_t * delta_t * delta_t;
    Et[1][1] = delta_t * delta_t;
    Et[1][2] = delta_t;
    Et[1][3] = 0;

    Et[2][0] = 6 * delta_t * delta_t * delta_t;
    Et[2][1] = 2 * delta_t * delta_t;
    Et[2][2] = 0;
    Et[2][3] = 0;

    Et[3][0] = 6 * delta_t * delta_t * delta_t;
    Et[3][1] = 0;
    Et[3][2] = 0;
    Et[3][3] = 0;

    // Transpose E(delta t)
    transpose(Et);

}

float **superSpline::multMatrix4x4(float **mat1, float **mat2)
{
    float **result = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        result[i] = new float[4];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i][j] = 0.0;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result[i][j] = result[i][j] + (mat1[i][k] * mat2[k][j]);
            }
        }
    }
    return result;
}

void superSpline::transpose(float **mat)
{
    float **result;
    for (int i = 0; i < 4; i++)
    {
        result[i] = new float[4];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i][j] = mat[j][i];
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[i][j] = result[i][j];
        }
    }
}

void superSpline::Clip()
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->Clip();
    }
}

void superSpline::SetWindow(float _xWinMin, float _yWinMin, float _xWinMax,
                            float _yWinMax, float _dop)
{
    xWinMax = _xWinMax;
    xWinMin = _xWinMin;
    yWinMax = _yWinMax;
    yWinMin = _yWinMin;
    dop = _dop;
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->SetWindow(_xWinMin, _yWinMin, _xWinMax, _yWinMax, _dop);
    }
}

void superSpline::SetViewPort(float _xVpMax, float _yVpMax)
{
    xVpMax = _xVpMax;
    yVpMax = _yVpMax;
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->SetViewPort(_xVpMax, _yVpMax);
    }
}

void superSpline::RotateInX(float ang)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateInX(ang);
    }
}

void superSpline::RotateAroundWorldInX(float ang)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateAroundWorldInX(ang);
    }
}

void superSpline::RotateAroundPointInX(float ang, float x, float y, float z)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateAroundPointInX(ang, x, y, z);
    }
}

void superSpline::RotateInY(float ang)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateInY(ang);
    }
}

void superSpline::RotateAroundWorldInY(float ang)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateAroundWorldInY(ang);
    }
}

void superSpline::RotateAroundPointInY(float ang, float x, float y, float z)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateAroundPointInY(ang, x, y, z);
    }
}

void superSpline::RotateInZ(float ang)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateInZ(ang);
    }
}

void superSpline::RotateAroundWorldInZ(float ang)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateAroundWorldInZ(ang);
    }
}

void superSpline::RotateAroundPointInZ(float ang, float x, float y, float z)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->RotateAroundPointInZ(ang, x, y, z);
    }
}

void superSpline::ScaleWorld(float scale)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->ScaleWorld(scale);
    }
}

void superSpline::Scale(float scale)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->Scale(scale);
    }
}

void superSpline::Translate(float x, float y, float z)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->Translate(x, y, z);
    }
}

void superSpline::ApplyTransform()
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->ApplyTransform();
    }
}

void superSpline::Project()
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->Project();
    }
}

void superSpline::Unproject()
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->Unproject();
    }
}

void superSpline::Draw(cairo_t *cr)
{
    for (int i = 0; i < sizeof(polygons); i++)
    {
        polygons[i]->Draw(cr);
    }
}

float superSpline::TransformToVpX(float x)
{
    float divWin = (x - xWinMin) / (xWinMax - xWinMin);
    float xVp = (divWin) * (xVpMax - 20) + 10;

    return xVp;
}

float superSpline::TransformToVpY(float y)
{
    float divWinY = 1 - ((y - yWinMin) / (yWinMax - yWinMin));
    float yVp = (divWinY) * (yVpMax - 20) + 10;
    //yVp = posWinY*(yVpMax - yVpMin);

    return yVp;
}
