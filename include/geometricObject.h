#ifndef __GEOMETRICOBJECT_H_INCLUDED__
#define __GEOMETRICOBJECT_H_INCLUDED__

#include <gtk/gtk.h>
#include <math.h>

class GeometricObject
{
protected:
    float xWinMax;
    float xWinMin;
    float yWinMax;
    float yWinMin;
    float xVpMax;
    float yVpMax;
    float xVpMin;
    float yVpMin;
    float dop;
    float transformMatrix[4][4];
    float vUp[4];

public:

    bool isShowing;

    virtual void Clip()
    {
    };

    virtual void Draw(cairo_t *cr)
    {
    };

    virtual void Translate(float x, float y, float z) = 0;

    virtual void Project() = 0;

    virtual void Unproject() = 0;

    virtual void Scale(float scale)
    {
    };

    virtual void ScaleWorld(float scale)
    {
    };

    virtual void RotateInX(float ang)
    {
    };

    virtual void RotateAroundPointInX(float ang, float x, float y, float z)
    {};

    virtual void RotateAroundPointInY(float ang, float x, float y, float z)
    {};

    virtual void RotateAroundPointInZ(float ang, float x, float y, float z)
    {};

    virtual void RotateAroundWorldInX(float ang)
    {
    };

    virtual void RotateInY(float ang)
    {
    };

    virtual void RotateAroundWorldInY(float ang)
    {
    };

    virtual void RotateInZ(float ang)
    {
    };

    virtual void RotateAroundWorldInZ(float ang)
    {
    };

    virtual void ApplyTransform()
    {
    };

    virtual float **MultiplyMatrix(float m1[4][4], float m2[4][4])
    {
        float cellValue = 0;
        float **resultMatrix = new float *[4];
        for (int l = 0; l < 4; l++)
        {
            resultMatrix[l] = new float[4];
        }
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                cellValue = 0;
                for (int k = 0; k < 4; k++)
                {
                    cellValue += m1[i][k] * m2[k][j];
                }
                resultMatrix[i][j] = cellValue;
            }
        }
        return resultMatrix;
    };

    //COnfigurar VUP 3D
    virtual void SetVUp(float ang)
    {
        vUp[0] = sin(ang);
        vUp[1] = cos(ang);
        vUp[2] = 1;
    }

    virtual void AddToTransform(float matrix[4][4])
    {
        float **resultMatrix = MultiplyMatrix(matrix, transformMatrix);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                transformMatrix[i][j] = resultMatrix[i][j];
            }
        }
    };

    virtual void SetTransformToIdentity()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                transformMatrix[i][j] = 0;
                if (i == j)
                {
                    transformMatrix[i][j] = 1;
                }
            }
        }
    }

    virtual void SetWindow(float _xWinMin, float _yWinMin, float _xWinMax,
                           float _yWinMax, float _dop)
    {
        xWinMax = _xWinMax;
        xWinMin = _xWinMin;
        yWinMax = _yWinMax;
        yWinMin = _yWinMin;
        dop = _dop;
    };

    virtual void SetViewPort(float _xVpMax, float _yVpMax)
    {
        xVpMax = _xVpMax;
        yVpMax = _yVpMax;
    };
};

#endif
