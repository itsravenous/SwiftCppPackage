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


#ifndef FINGERPRINT_HPP
#define FINGERPRINT_HPP 1

#include <stdlib.h>
#include "element.hpp"

class Pair
{
friend class FingerPrint;

public:
    Pair() { dist = DBL_INIT; m1 = -1; m2 = -1; };
    ~Pair() {};

    int    getM1()   const { return m1; };
    int    getM2()   const { return m2; };
    double getDist() const { return dist; };
	void set(int _m1, int _m2, double _d) { m1 = _m1; m2= _m2; dist = _d; };

protected:
    int m1, m2;
    double dist;
};

class FingerPrint 
{
friend class Compare;

public:
    FingerPrint(const FingerPrint&);
    FingerPrint(double *ref, double *data, int nr);
    FingerPrint();
    ~FingerPrint();

    const FingerPrint& operator=(const FingerPrint&);
    void addReference();

    double distance(const FingerPrint&, Pair *pairs, int *paircnt, int affine_corr = 0);
	double determineMaxDist();
    void resetScore();
    void doAffine(double* matrix);
	void toArray(double *a);
	void calcNormFactor();

    const Element& getElt(int) const;
    int            getCnt() const        { return cnt; }
    double         getScore() const      { return score; };
    int            getPairCnt() const    { return paircnt; };

    void setScore(double s) { score = s; }
    void setPairCnt(int p) { paircnt = p; }

private:
    Element *elt;
    Point2D ref1, ref2, ref3;
    int cnt;

    double score, normfactor;
    int paircnt;
};

inline void FingerPrint::addReference()
{
	Element *temp = new Element[cnt+3];
	memcpy(&(temp[3]), elt, sizeof(Element)*cnt);

	Element e1; e1.set(ref1.x, ref1.y);
	Element e2; e2.set(ref2.x, ref2.y);
	Element e3; e3.set(ref3.x, ref3.y);
	temp[0] = e1;
	temp[1] = e2;
	temp[2] = e3;
	cnt += 3;

	delete [] elt;
	elt = temp;
}

inline const Element& FingerPrint::getElt(int i) const
{
    if(i<0 || i>= cnt) {
        Element *dummy = new Element();
		fprintf(stderr, "Parameter error in FingerPrint::getElt(int) i: %d  cnt: %d\n", i, cnt);
        return *dummy;
    }
 
    return elt[i];
}

inline const FingerPrint& FingerPrint::operator=(const FingerPrint& f)
{
    cnt        = f.cnt;
    score      = f.score;
    paircnt    = f.paircnt;
	normfactor = f.normfactor;

    if(elt) {
        delete [] elt;
		elt = 0;
    }

    ref1 = f.ref1;
    ref2 = f.ref2;
    ref3 = f.ref3;

    if(f.elt) {
        elt = new Element [cnt];
        memcpy(elt, f.elt, cnt*sizeof(Element));
    }

    return *this;
}

inline FingerPrint::FingerPrint(const FingerPrint& f)
{
    cnt        = f.cnt;
    score      = f.score;
    paircnt    = f.paircnt;
	normfactor = f.normfactor;

    elt = 0;
    if(f.elt) {
        elt = new Element [cnt];
        memcpy(elt, f.elt, cnt*sizeof(Element));
    }
    ref1 = f.ref1;
    ref2 = f.ref2;
    ref3 = f.ref3;
}

inline FingerPrint::FingerPrint()
{
    cnt		   = 0;
    elt		   = 0;
    score 	   = DBL_INIT;
    paircnt    = -1;
	normfactor = -1;
}

inline FingerPrint::~FingerPrint()
{
    if(elt)
        delete [] elt;
    elt = 0;
}

// take care of little and big Endian crap. Java clearly writes other Endian than Windows
inline void swap(int& i)
{
#ifdef _WINDOWS
    int tmp = i;
    unsigned char *p  = (unsigned char*) (void*) &i;
    unsigned char *c1 = (unsigned char*) (void*) &tmp;
    unsigned char *c2 = c1+1;
    unsigned char *c3 = c1+2;
    unsigned char *c4 = c1+3;

    *c1 = *(p+3);
    *c2 = *(p+2);
    *c3 = *(p+1);
    *c4 = *(p);

    i = tmp;
#endif
}

inline void swap(double& d)
{
#ifdef _WINDOWS
    double tmp = d;
    unsigned char *p  = (unsigned char*) (void*) &d;
    unsigned char *c1 = (unsigned char*) (void*) &tmp;
    unsigned char *c2 = c1+1;
    unsigned char *c3 = c1+2;
    unsigned char *c4 = c1+3;
    unsigned char *c5 = c1+4;
    unsigned char *c6 = c1+5;
    unsigned char *c7 = c1+6;
    unsigned char *c8 = c1+7;

    *c1 = *(p+7);
    *c2 = *(p+6);
    *c3 = *(p+5);
    *c4 = *(p+4);
    *c5 = *(p+3);
    *c6 = *(p+2);
    *c7 = *(p+1);
    *c8 = *(p);

    d = tmp;
#endif
}

#endif