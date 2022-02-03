#include "point.h"
#include <iostream>

using std::cout;
using std::endl;

Point::Point(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
    position[3] = 1;

    nonProjectedPosition[0] = x;
    nonProjectedPosition[1] = y;
    nonProjectedPosition[2] = z;
    nonProjectedPosition[3] = 1;

    SetTransformToIdentity();
    isShowing = true;
}

void Point::RotateAroundPointInX(float ang, float x, float y, float z)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    float angInRads = ang * (M_PI / 180);

    matrix[1][1] = cos(angInRads);
    matrix[1][2] = sin(angInRads);
    matrix[2][1] = -sin(angInRads);
    matrix[2][2] = cos(angInRads);

    Translate(x, y, z);
    AddToTransform(matrix);
    Translate(-x, -y, -z);
    ApplyTransform();
}

void Point::RotateAroundWorldInX(float ang)
{
    RotateInX(ang);
}

void Point::RotateInX(float ang)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    float angInRads = ang * (M_PI / 180);

    matrix[1][1] = cos(angInRads);
    matrix[1][2] = sin(angInRads);
    matrix[2][1] = -sin(angInRads);
    matrix[2][2] = cos(angInRads);

    AddToTransform(matrix);
    ApplyTransform();

}

void Point::RotateAroundPointInY(float ang, float x, float y, float z)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    float angInRads = ang * (M_PI / 180);

    matrix[0][0] = cos(angInRads);
    matrix[0][2] = -sin(angInRads);
    matrix[2][0] = sin(angInRads);
    matrix[2][2] = cos(angInRads);

    Translate(x, y, z);
    AddToTransform(matrix);
    Translate(-x, -y, -z);
    ApplyTransform();
}

void Point::RotateAroundWorldInY(float ang)
{
    RotateInY(ang);
}

void Point::RotateInY(float ang)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    float angInRads = ang * (M_PI / 180);

    matrix[0][0] = cos(angInRads);
    matrix[0][2] = -sin(angInRads);
    matrix[2][0] = sin(angInRads);
    matrix[2][2] = cos(angInRads);

    AddToTransform(matrix);
    ApplyTransform();

}

void Point::RotateAroundPointInZ(float ang, float x, float y, float z)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    float angInRads = ang * (M_PI / 180);

    matrix[0][0] = cos(angInRads);
    matrix[0][1] = sin(angInRads);
    matrix[1][0] = -sin(angInRads);
    matrix[1][1] = cos(angInRads);

    Translate(x, y, z);
    AddToTransform(matrix);
    Translate(-x, -y, -z);
    ApplyTransform();
}

void Point::RotateAroundWorldInZ(float ang)
{
    RotateInZ(ang);
}

void Point::RotateInZ(float ang)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    float angInRads = ang * (M_PI / 180);

    matrix[0][0] = cos(angInRads);
    matrix[0][1] = -sin(angInRads);
    matrix[1][0] = sin(angInRads);
    matrix[1][1] = cos(angInRads);

    AddToTransform(matrix);
    ApplyTransform();

}

void Point::ScaleWorld(float scale)
{
    Scale(scale);
    ApplyTransform();
}

void Point::Scale(float scale)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    matrix[0][0] = scale;
    matrix[1][1] = scale;
    matrix[2][2] = scale;

    AddToTransform(matrix);
}

void Point::Project()
{
    //g_print("Before Projection x %f xNOn %f / z %f/ dop %f\n",position[0],nonProjectedPosition[0],position[2],dop);
    position[0] = nonProjectedPosition[0] / (position[2] / dop);
    position[1] = nonProjectedPosition[1] / (position[2] / dop);
    //g_print("After Projection x %f xNOn %f / z %f/ dop %f\n",position[0],nonProjectedPosition[0],position[2],dop);
}

void Point::Unproject()
{
    float x = nonProjectedPosition[0];
    float y = nonProjectedPosition[1];
    position[0] = x;
    position[1] = y;
}

void Point::Translate(float x, float y, float z)
{
    float matrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
            if (i == j)
            {
                matrix[i][j] = 1;
            }
        }
    }
    matrix[3][0] = x;
    matrix[3][1] = y;
    matrix[3][2] = z;
    AddToTransform(matrix);
}

void Point::ApplyTransform()
{
    float cellValue;
    float buffer[4];
    for (int j = 0; j < 4; j++)
    {
        cellValue = 0;
        for (int k = 0; k < 4; k++)
        {
            cellValue += position[k] * transformMatrix[k][j];
        }
        buffer[j] = cellValue;
    }
    position[0] = buffer[0];
    position[1] = buffer[1];
    position[2] = buffer[2];

    nonProjectedPosition[0] = buffer[0];
    nonProjectedPosition[1] = buffer[1];
    nonProjectedPosition[2] = buffer[2];
    SetTransformToIdentity();
}

void Point::Clip()
{
    if (position[0] > xWinMin && position[0] < xWinMax)
    {
        if (position[1] > yWinMin && position[1] < yWinMax)
        {
            isShowing = true;  //cout<<"aparece"<<endl;
            return;
        }
    }
    isShowing = false;
}

void Point::Draw(cairo_t *cr)
{
    //cairo_rectangle (cr, cx, cy, 6, 6);
    //	 0  - -200   / 300  - -200       * 500
    if (!isShowing)
    {
        return;
    }

    // g_print("Desenha Point \n");

    float divWin = (position[0] - xWinMin) / (xWinMax - xWinMin);
    float divWinY = 1 - ((position[1] - yWinMin) / (yWinMax - yWinMin));

    xVp = (divWin) * (xVpMax - 0);
    yVp = (divWinY) * (yVpMax - 0);

    // g_print("Point at Vp x %i \n", xVp);

    cairo_rectangle(cr, xVp, yVp, 6, 6);

    //g_print("Creating rectangle %i %i \n",cx,cy);
}
