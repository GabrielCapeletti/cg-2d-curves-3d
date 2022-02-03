#include "superBezier.h"

superBezier::superBezier(Point *G[4][4])
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
    matrixM = new float *[4];
    matrixMT = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        matrixM[i] = new float[4];
        matrixMT[i] = new float[4];
    }
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
    matrixMT = transpose(matrixM);

    drawWithFixedS();
    drawWithFixedT();
    poliClip = new Polygon(_pointsCurve);
    poliClip->conectFirstAndLast = false;
}

void superBezier::drawWithFixedS()
{
    Point *newPoint;

    vector<float> S;
    for (float s = 0.00; s < 1.00; s += 0.1)
    {
        //g_print("t %f  \n", s);
        S.clear();
        S.push_back(pow(s, 3));
        S.push_back(pow(s, 2));
        S.push_back(s);
        S.push_back(1);
        vector<float> T;
        vector<float> result = multiplayVM(S, matrixM);
        vector<float> result1 = multiplayVM(result, Gx);
        vector<float> result2 = multiplayVM(result1, matrixMT);

        vector<float> result3 = multiplayVM(result, Gy);
        vector<float> result4 = multiplayVM(result3, matrixMT);

        vector<float> result5 = multiplayVM(result, Gz);
        vector<float> result6 = multiplayVM(result5, matrixMT);
        for (float t = 0.00; t < 1.00; t += 0.1)
        {
            //g_print("t %f  \n", t);
            T.clear();
            T.push_back(pow(t, 3));
            T.push_back(pow(t, 2));
            T.push_back(t);
            T.push_back(1);

            float x = multiplayVV(result2, T);
            float y = multiplayVV(result4, T);
            float z = multiplayVV(result6, T);
            newPoint = new Point(x, y, z);
            _pointsCurve.push_back(newPoint);

        }

    }
}

void superBezier::drawWithFixedT()
{
    Point *newPoint;
    vector<float> T;
    for (float t = 0.00; t < 1.00; t += 0.1)
    {
        //g_print("t %f  \n", s);
        //g_print("t %f  \n", t);
        T.clear();
        T.push_back(pow(t, 3));
        T.push_back(pow(t, 2));
        T.push_back(t);
        T.push_back(1);
        vector<float> S;
        for (float s = 0.00; s < 1.00; s += 0.1)
        {
            S.clear();
            S.push_back(pow(s, 3));
            S.push_back(pow(s, 2));
            S.push_back(s);
            S.push_back(1);
            vector<float> result = multiplayVM(S, matrixM);
            vector<float> result1 = multiplayVM(result, Gx);
            vector<float> result2 = multiplayVM(result1, matrixMT);

            vector<float> result3 = multiplayVM(result, Gy);
            vector<float> result4 = multiplayVM(result3, matrixMT);

            vector<float> result5 = multiplayVM(result, Gz);
            vector<float> result6 = multiplayVM(result5, matrixMT);
            float x = multiplayVV(result2, T);
            float y = multiplayVV(result4, T);
            float z = multiplayVV(result6, T);
            newPoint = new Point(x, y, z);
            _pointsCurve.push_back(newPoint);

        }


    }
}

float superBezier::multiplayVV(vector<float> v1, vector<float> v2)
{
    float p = 0;
    for (int i = 0; i < 4; i++)
    {
        p = v1[i] * v2[i] + p;
    }

    return p;

}

float **superBezier::transpose(float **matrix)
{
    float **mt = new float *[4];
    for (int i = 0; i < 4; i++)
    {
        mt[i] = new float[4];
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mt[i][j] = matrix[j][i];
        }
    }
    return mt;
}

vector<float> superBezier::multiplayVM(vector<float> T, float **matrixM)
{
    vector<float> r;
    for (int j = 0; j < T.size(); j++)
    {
        r.push_back(0);
    }
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
    }
    //g_print("x %f y %f\n", x, y);
    return r;

}

void superBezier::Clip()
{
    poliClip->Clip();
}

void superBezier::SetWindow(float _xWinMin, float _yWinMin, float _xWinMax,
                            float _yWinMax, float _dop)
{
    xWinMax = _xWinMax;
    xWinMin = _xWinMin;
    yWinMax = _yWinMax;
    yWinMin = _yWinMin;
    dop = _dop;
    poliClip->SetWindow(_xWinMin, _yWinMin, _xWinMax, _yWinMax, _dop);
}

void superBezier::Project()
{
    poliClip->Project();
}

void superBezier::Unproject()
{
    poliClip->Unproject();
}

void superBezier::SetViewPort(float _xVpMax, float _yVpMax)
{
    xVpMax = _xVpMax;
    yVpMax = _yVpMax;
    poliClip->SetViewPort(_xVpMax, _yVpMax);
}

void superBezier::RotateInX(float ang)
{
    poliClip->RotateInX(ang);
}

void superBezier::RotateAroundWorldInX(float ang)
{
    poliClip->RotateAroundWorldInX(ang);
}

void superBezier::RotateAroundPointInX(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInX(ang, x, y, z);
}

void superBezier::RotateInY(float ang)
{
    poliClip->RotateInY(ang);
}

void superBezier::RotateAroundWorldInY(float ang)
{
    poliClip->RotateAroundWorldInY(ang);
}

void superBezier::RotateAroundPointInY(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInY(ang, x, y, z);
}

void superBezier::RotateInZ(float ang)
{
    poliClip->RotateInZ(ang);
}

void superBezier::RotateAroundWorldInZ(float ang)
{
    poliClip->RotateAroundWorldInZ(ang);
}

void superBezier::RotateAroundPointInZ(float ang, float x, float y, float z)
{
    poliClip->RotateAroundPointInZ(ang, x, y, z);
}

void superBezier::ScaleWorld(float scale)
{
    poliClip->ScaleWorld(scale);
}

void superBezier::Scale(float scale)
{
    poliClip->Scale(scale);
}

void superBezier::Translate(float x, float y, float z)
{
    poliClip->Translate(x, y, z);
}

void superBezier::ApplyTransform()
{
    poliClip->ApplyTransform();
}

void superBezier::Draw(cairo_t *cr)
{
    poliClip->Draw(cr);
}

float superBezier::TransformToVpX(float x)
{
    float divWin = (x - xWinMin) / (xWinMax - xWinMin);
    float xVp = (divWin) * (xVpMax - 20) + 10;

    return xVp;
}

float superBezier::TransformToVpY(float y)
{
    float divWinY = 1 - ((y - yWinMin) / (yWinMax - yWinMin));
    float yVp = (divWinY) * (yVpMax - 20) + 10;
    //yVp = posWinY*(yVpMax - yVpMin);

    return yVp;
}



