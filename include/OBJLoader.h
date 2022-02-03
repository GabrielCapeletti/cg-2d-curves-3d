/*
 * OBJLoader.h
 *
 *  Created on: May 27, 2015
 *      Author: gabcap
 */

#ifndef OBJLOADER_H_
#define OBJLOADER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "polygon.h"
#include "point.h"

class OBJLoader
{
public:
    OBJLoader();

    virtual ~OBJLoader();

    vector<Point *> GetPoints();

private:
    vector<Point *> polyPoints;
};

#endif /* OBJLOADER_H_ */
