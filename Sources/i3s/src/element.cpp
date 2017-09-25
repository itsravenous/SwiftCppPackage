/*******************************************************************************
 *   I3S: Interactive Individual Identification System                         *
 *                                                                             *
 *   Copyright (C) 2004-2013  Jurgen den Hartog & Renate Reijns                *
 *                                                                             *
 *   This program is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                       *
 *                                                                             *
 *   This program is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *   GNU General Public License for more details.                              *
 *                                                                             *
 *   You should have received a copy of the GNU General Public License         *
 *   along with this program; see the file COPYING GPL v2.txt. If not,         *
 *   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, *
 *   Boston, MA 02111-1307, USA.                                               *
 *                                                                             *
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../incl/element.hpp"
#include "../incl/fingerprint.hpp"

// extern double minRatioArea;
// extern double maxAngleDiff;
// extern double minRatioRatio;
// extern double maxRatioToCalcAngle;

// extern int I3S_type;

// extern bool debug;

double minRatioArea = 0.0;
double maxAngleDiff = 0.0;
double minRatioRatio = 0.0;
double maxRatioToCalcAngle = 0.0;
int I3S_type = 0;
// bool debug = true;

static void _doAffine(double& x, double& y, double *matrix);

inline double _absoluteDiff(double x1, double x2) {
	if(x1 < x2)
		return x2-x1;
	return x1-x2;
}

/***
    Determines similarity between two elements (1.0 is perfect). Each difference 
	in the various ratios introduces a penalty. 
***/
double Element::calcSimilarityRate(const Element& e) const
{
    double rate = 1.0;

	if(I3S_type == 0)	// CLASSIC
		return 1.0;

	if(area > 0 && e.area > 0) {
		double t = sqrt(area / e.area);
		if(t < 1.0)
			t = 1.0/t;
		t = 0.5*t + 0.5;
		rate *= t;
	}

	if(I3S_type == 2)	// PATTERN
		return rate;

	// This only matters to SPOT
    if(ratio > 0 && e.ratio > 0) {
        double t = ratio / e.ratio;
        if(t < 1.0)
            t = 1.0/t;
        t = 0.5*t + 0.5;
        rate *= t;
    }
	if(e.angle > -1 && angle > -1) {
		double diff = abs(e.angle - angle);
		if(diff < 180.0 - diff)		// calculate absolute and smallest possible angle
			diff = 180.0 - diff;
		// 0 is optimal (no penalty) and 90 is worst case (penalty of 50%)
		double t = diff/180.0 + 1.0;
		rate *= t;
	}

    return rate;
}

bool Element::matches(const Element& e) const
{
	if(I3S_type == 0)	// CLASSIC
		return true;

    // minRatioArea is defined in xml file
	if(isSingular() && e.isSingular())
		return true;

	if(isSingular() != e.isSingular())
		return false;

	if(withinRatio(area, e.area, minRatioArea) == false)
        return false;
	if(I3S_type == 2)	// PATTERN
		return true;

	// This only matters to SPOT
	if(e.ratio > -1 && ratio > -1)
        if(withinRatio(ratio, e.ratio, minRatioRatio) == false)
            return false;
	if(e.angle > -1 && angle > -1) {
		double diff = abs(e.angle - angle);
		if(diff > 180.0 - diff)		// calculate absolute and smallest possible angle
			diff = 180.0 - diff;
        if(diff > maxAngleDiff)
            return false;
	}

    return true;
}

void Element::doAffine(double *matrix)
{
    if(matrix == 0)
        return;

    for(int i=0; i<4; i++) {
        double tmpx = data[i].getX();
        double tmpy = data[i].getY();
        _doAffine(tmpx, tmpy, matrix);
        data[i].set(tmpx, tmpy);
    }
	cx = (data[0].getX() + data[1].getX())/ 2.0;
	cy = (data[0].getY() + data[1].getY())/ 2.0;

	calcShapeAndArea();	
}

void Element::calcShapeAndArea()
{
	if(I3S_type == 0 || isSingular()) {
		area = 0;
		ratio = -1;
		angle = -1;
		return;
	}

	double l1 = data[0].getDist(data[1]);
	double l2 = data[2].getDist(data[3]);
	area = l1*l2*3.141592653589793/4;		// area of ellipse is PI x radius1 x radius2. Divide by 4 as l1 and l2 are diameters

	if(I3S_type == 2) {		// PATTERN
		angle = ratio = -1;
		return;
	}

	// This only matters to SPOT
    if(_absoluteDiff(l1, l2) < 0.01)
		ratio = 1;
	else if(l1 < l2)
		ratio = l1 / l2;
	else
		ratio = l2 / l1;

	if(ratio < maxRatioToCalcAngle) {	// only calculate angle when there is an angle to calculate, i.e. sufficient difference between l1 and l2
		double dx = 0, dy = 0;
		if(l1 > l2) {	// calc angle from longest side
			dx = data[3].getX() - data[2].getX();
			dy = data[3].getY() - data[2].getY();
		} else {
			dx = data[1].getX() - data[0].getX();
			dy = data[1].getY() - data[0].getY();
		}
		if(dx < 0.0001 && dy < 0.0001)
			angle = -1;
		else {
			angle = atan2(dy, dx) * 180 / 3.141592653589793;
			if(angle < 0)
				angle += 180.0;
		}
	} else
		angle = -1;
}

static void _doAffine(double& x, double& y, double *matrix)
{
    double _x = x;
    double _y = y;

    x = matrix[0]*_x + matrix[1]*_y + matrix[2];
    y = matrix[3]*_x + matrix[4]*_y + matrix[5];
}