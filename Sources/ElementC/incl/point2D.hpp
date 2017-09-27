/*******************************************************************************
 *   I3S: Interactive Individual Identification System                         *
 *                                                                             *
 *   Copyright (C) 2004-2008  Jurgen den Hartog & Renate Reijns                *
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


#ifndef POINT2D_HPP
#define POINT2D_HPP 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#ifndef DBL_INIT
#define DBL_INIT -1000000000
#endif



class Point2D {
friend class FingerPrint;
friend class Lin2D;

public:
   Point2D();
   Point2D(const Point2D&);
   Point2D(double, double);
   ~Point2D() {};

   void set(double, double);
   void setX(double);
   void setY(double);
   double getX() const;
   double getY() const;

   double getDist(const Point2D&) const;
   double getDist(const double _x, const double _y) const;
   double getSqrDist(const Point2D&) const;
   double getSqrDist(const double _x, const double _y) const;

   bool operator==(const Point2D&) const;
   const Point2D& operator=(const Point2D&);

protected:
   double x, y;
};


inline Point2D::Point2D()
{
   x = DBL_INIT;
   y = DBL_INIT;
}
inline Point2D::Point2D(const Point2D& p)
{
   x = p.x;
   y = p.y;
}
inline Point2D::Point2D(double _x, double _y) 
{
   x = _x;
   y = _y;
}
inline bool Point2D::operator==(const Point2D& p) const 
{
   return (x == p.x && y == p.y);
}
inline const Point2D& Point2D::operator=(const Point2D& p)
{
   x = p.x;
   y = p.y;
   return *this;
}
inline void Point2D::set(double _x, double _y)
{
   x = _x;
   y = _y;
}
inline void Point2D::setX(double _x)
{
   x = _x;
}
inline void Point2D::setY(double _y)
{
   y = _y;
}
inline double Point2D::getX() const
{
   return x;
}
inline double Point2D::getY() const
{
   return y;
}
inline double Point2D::getDist(const Point2D& p) const
{
   return sqrt(getSqrDist(p));
}
inline double Point2D::getDist(const double _x, const double _y) const
{
   return sqrt(getSqrDist(_x, _y));
}
inline double Point2D::getSqrDist(const Point2D& p) const
{
   return (p.x - x)*(p.x - x) + (p.y - y)*(p.y - y);
}
inline double Point2D::getSqrDist(const double _x, const double _y) const
{
   return (_x - x)*(_x - x) + (_y - y)*(_y - y);
}



#endif