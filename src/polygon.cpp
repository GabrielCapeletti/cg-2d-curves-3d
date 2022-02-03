#include "polygon.h"

Polygon::Polygon(std::vector<Point *> points)
{
    _points = points;
    conectFirstAndLast = true;
}

void Polygon::RotateAroundWorldInX(float ang)
{
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInX(ang);
    }
    ApplyTransform();
}

void Polygon::RotateAroundWorldInY(float ang)
{
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInY(ang);
    }
    ApplyTransform();
}

void Polygon::RotateAroundWorldInZ(float ang)
{
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInZ(ang);
    }
    ApplyTransform();
}

void Polygon::Unproject()
{
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->Unproject();
    }
}


void Polygon::Project()
{
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->Project();
    }
}

void Polygon::RotateAroundPointInZ(float ang, float x, float y, float z)
{
    Translate(-x, -y, -z);
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInZ(ang);
    }
    ApplyTransform();
    Translate(x, y, z);
}

void Polygon::RotateAroundPointInX(float ang, float x, float y, float z)
{
    Translate(-x, -y, -z);
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInX(ang);
    }
    ApplyTransform();
    Translate(x, y, z);
}

void Polygon::RotateAroundPointInY(float ang, float x, float y, float z)
{
    Translate(-x, -y, -z);
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInY(ang);
    }
    ApplyTransform();
    Translate(x, y, z);
}

void Polygon::Clip()
{
    isShowing = true;
    insidePoints();
    clipped = true;

}

void Polygon::insidePoints()
{
    _newPoints.clear();
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->Clip();
        if (_points[i]->isShowing)
        {
            _newPoints.push_back(new Point(_points[i]->position[0], _points[i]->position[1], _points[i]->position[2]));
            continue;
        }
        else if (!_points[i]->isShowing)
        {
            if (_points[i]->position[0] < xWinMin)
            { //left
                if (_points[i]->position[1] < yWinMin)
                {
                    clipLeftDown(_points[i]); //left down

                    continue;
                }
                else if (_points[i]->position[1] > yWinMax)
                {
                    clipLeftUp(_points[i]); //left up
                    continue;
                }
                else
                {
                    clipLeft(_points[i]);
                    continue;
                }
            }
            else if (_points[i]->position[0] > xWinMax)
            { //right
                if (_points[i]->position[1] < yWinMin)
                {
                    clipRightDown(_points[i]);
                    continue;
                }
                else if (_points[i]->position[1] > yWinMax)
                {
                    clipRightUp(_points[i]);
                    continue;
                }
                else
                {
                    clipRight(_points[i]);
                    continue;
                }
            }
            else if (_points[i]->position[1] > yWinMax)
            {
                clipUp(_points[i]);
                continue;
            }
            else if (_points[i]->position[1] < yWinMin)
            {
                clipDown(_points[i]);
                continue;
            }
        }
    }

}

void Polygon::clipLeftUp(Point *p)
{
    _newPoints.push_back(new Point(xWinMin, yWinMax, 0));
}

void Polygon::clipLeftDown(Point *p)
{
    _newPoints.push_back(new Point(xWinMin, yWinMin, 0));
}

void Polygon::clipRightUp(Point *p)
{
    _newPoints.push_back(new Point(xWinMax, yWinMax, 0));
}

void Polygon::clipRightDown(Point *p)
{
    _newPoints.push_back(new Point(xWinMax, yWinMin, 0));
}

void Polygon::clipLeft(Point *p)
{
    _newPoints.push_back(new Point(xWinMin, p->position[1], 0));
}

void Polygon::clipRight(Point *p)
{
    _newPoints.push_back(new Point(xWinMax, p->position[1], 0));
}

void Polygon::clipUp(Point *p)
{
    _newPoints.push_back(new Point(p->position[0], yWinMax, 0));
}

void Polygon::clipDown(Point *p)
{
    _newPoints.push_back(new Point(p->position[0], yWinMin, 0));
}

void Polygon::RotateInZ(float ang)
{
    float averageX = 0; //(_pi->position[0] + _pf->position[0])/2;
    float averageY = 0; //(_pi->position[1] + _pf->position[1])/2;
    float averageZ = 0;
    float totalSumX = 0;
    float totalSumY = 0;
    float totalSumZ = 0;
    for (int i = 0; i < _points.size(); i++)
    {
        totalSumX += _points[i]->position[0];
        totalSumY += _points[i]->position[1];
        totalSumZ += _points[i]->position[2];
    }
    averageX = totalSumX / _points.size();
    averageY = totalSumY / _points.size();
    averageZ = totalSumZ / _points.size();
    Translate(-averageX, -averageY, -averageZ);
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInZ(ang);
    }
    ApplyTransform();
    Translate(averageX, averageY, averageZ);
}

void Polygon::RotateInX(float ang)
{
    float averageX = 0; //(_pi->position[0] + _pf->position[0])/2;
    float averageY = 0; //(_pi->position[1] + _pf->position[1])/2;
    float averageZ = 0;
    float totalSumX = 0;
    float totalSumY = 0;
    float totalSumZ = 0;
    for (int i = 0; i < _points.size(); i++)
    {
        totalSumX += _points[i]->position[0];
        totalSumY += _points[i]->position[1];
        totalSumZ += _points[i]->position[2];
    }
    averageX = totalSumX / _points.size();
    averageY = totalSumY / _points.size();
    averageZ = totalSumZ / _points.size();
    Translate(-averageX, -averageY, -averageZ);
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInX(ang);
    }
    ApplyTransform();
    Translate(averageX, averageY, averageZ);
}

void Polygon::RotateInY(float ang)
{
    float averageX = 0; //(_pi->position[0] + _pf->position[0])/2;
    float averageY = 0; //(_pi->position[1] + _pf->position[1])/2;
    float averageZ = 0;
    float totalSumX = 0;
    float totalSumY = 0;
    float totalSumZ = 0;
    for (int i = 0; i < _points.size(); i++)
    {
        totalSumX += _points[i]->position[0];
        totalSumY += _points[i]->position[1];
        totalSumZ += _points[i]->position[2];
    }
    averageX = totalSumX / _points.size();
    averageY = totalSumY / _points.size();
    averageZ = totalSumZ / _points.size();
    Translate(-averageX, -averageY, -averageZ);
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->RotateInY(ang);
    }
    ApplyTransform();
    Translate(averageX, averageY, averageZ);
}

void Polygon::ScaleWorld(float scale)
{
    for (int i = 0; i < _points.size(); i++)
    {
        //g_print("P x antes %f \n", _points[i]->position[0]);
        _points[i]->ScaleWorld(scale);
        //g_print("P x depois %f \n", _points[i]->position[0]);
    }
}

void Polygon::Scale(float scale)
{
    float averageX = 0; //(_pi->position[0] + _pf->position[0])/2;
    float averageY = 0; //(_pi->position[1] + _pf->position[1])/2;
    float averageZ = 0;
    float totalSumX = 0;
    float totalSumY = 0;
    float totalSumZ = 0;
    for (int i = 0; i < _points.size(); i++)
    {
        totalSumX += _points[i]->position[0];
        totalSumY += _points[i]->position[1];
        totalSumZ += _points[i]->position[2];
    }
    averageX = totalSumX / _points.size();
    averageY = totalSumY / _points.size();
    averageZ = totalSumZ / _points.size();
    Translate(-averageX, -averageY, -averageZ);
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->Scale(scale);
    }
    ApplyTransform();
    Translate(averageX, averageY, averageZ);
}

void Polygon::SetWindow(float _xWinMin, float _yWinMin, float _xWinMax,
                        float _yWinMax, float _dop)
{
    xWinMax = _xWinMax;
    xWinMin = _xWinMin;
    yWinMax = _yWinMax;
    yWinMin = _yWinMin;
    dop = _dop;
    for (int i = 0; i < _points.size(); i++)
    {
        _points[i]->SetWindow(_xWinMin, _yWinMin, _xWinMax, _yWinMax, _dop);
    }
};

void Polygon::Translate(float x, float y, float z)
{

    for (int i = 0; i < _points.size(); ++i)
    {
        _points[i]->Translate(x, y, z);
    }

    ApplyTransform();
    g_print("z%f\n", _points[0]->position[2]);
}

void Polygon::ApplyTransform()
{
    for (int i = 0; i < _points.size(); ++i)
    {
        _points[i]->ApplyTransform();
    }
}

void Polygon::Draw(cairo_t *cr)
{
    //g_print("Setting winx %f winy %f \n",xWinMin,xWinMax);

    if (!isShowing)
    {
        return;
    }

    if (clipped)
    {
        cairo_set_line_width(cr, 0.9);
        cairo_move_to(cr, TransformToVpX(_newPoints[0]->position[0]),
                      TransformToVpY(_newPoints[0]->position[1]));
        // cairo_rectangle (cr, TransformToVpX(_newPoints[0]->position[0]), TransformToVpY( _newPoints[0]->position[1]), 6, 6);
        for (int i = 1; i < _newPoints.size(); ++i)
        {
            //cairo_rectangle (cr, TransformToVpX(_newPoints[i]->position[0]), TransformToVpY( _newPoints[i]->position[1]), 6, 6);
            cairo_line_to(cr, (int) TransformToVpX(_newPoints[i]->position[0]),
                          (int) TransformToVpY(_newPoints[i]->position[1]));
            cairo_move_to(cr, TransformToVpX(_newPoints[i]->position[0]),
                          TransformToVpY(_newPoints[i]->position[1]));

//			g_print("Creating Line x %f y %f \n",
//					(_newPoints[i]->position[0]),
//					(_newPoints[i]->position[1]));
        }
        if (conectFirstAndLast)
        {
            cairo_line_to(cr, (int) TransformToVpX(_newPoints[0]->position[0]),
                          (int) TransformToVpY(_newPoints[0]->position[1]));
        }
//	    g_print("Creating Line x %f y %f \n",(int)TransformToVpX(_newPoints[1]->position[0]),(int)TransformToVpY( _newPoints[1]->position[1]));
    }

//	cairo_set_line_width(cr, 0.9);
//	cairo_move_to(cr, TransformToVpX(_points[0]->position[0]),
//			TransformToVpY(_points[0]->position[1]));
//	// cairo_rectangle (cr, TransformToVpX(_newPoints[0]->position[0]), TransformToVpY( _newPoints[0]->position[1]), 6, 6);
//	for (int i = 1; i < _points.size(); ++i) {
//		//cairo_rectangle (cr, TransformToVpX(_newPoints[i]->position[0]), TransformToVpY( _newPoints[i]->position[1]), 6, 6);
//		cairo_line_to(cr, (int) TransformToVpX(_points[i]->position[0]),
//				(int) TransformToVpY(_points[i]->position[1]));
//		cairo_move_to(cr, TransformToVpX(_points[i]->position[0]),
//				TransformToVpY(_points[i]->position[1]));
//
//	}

}

float Polygon::TransformToVpX(float x)
{
    float divWin = (x - xWinMin) / (xWinMax - xWinMin);
    float xVp = (divWin) * (xVpMax - 20) + 10;
    return xVp;
}

float Polygon::TransformToVpY(float y)
{
    float divWinY = 1 - ((y - yWinMin) / (yWinMax - yWinMin));
    float yVp = (divWinY) * (yVpMax - 20) + 10;
    //yVp = posWinY*(yVpMax - yVpMin);

    return yVp;
}
