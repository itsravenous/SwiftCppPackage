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

// #include <emscripten/bind.h>
// using namespace emscripten;
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../incl/fingerprint.hpp"

static void filterOutDuplicatePairs(Pair *pairs, int *paircnt, double& totaldist);
static void removePair(Pair *pairs, int d, int *paircnt, double& totaldist);
static int paircmp(const void* p1, const void* p2);


#define sqr(x) ((x)*(x))

// extern double maxAllowedDistance;
// extern double minRelativeDistance;

double maxAllowedDistance = 0.1;
double minRelativeDistance = 3.0;

bool debug = true;

FingerPrint::FingerPrint(double *ref, double *data, int nr) {
  // printf("FingerPrint created with %d points\n", nr);
	ref1.x = ref[0];
	ref1.y = ref[1];
	ref2.x = ref[2];
	ref2.y = ref[3];
	ref3.x = ref[4];
	ref3.y = ref[5];

	cnt		= nr;
    elt		= new Element[nr];
    score	= DBL_INIT;
    paircnt	= -1;

  int _nr = nr * 2;
  if (nr % 2 == 0) _nr = _nr - 1;
  // printf("nr: %d %d\n", nr, _nr);
  for(int i=0; i<nr; i++) {
    // printf("i: %d %f\n", i*8, data[i*8]);
    elt[i].set(&(data[i*8]));
  }
	calcNormFactor();
}

void FingerPrint::toArray(double *a)
{
	for(int i=0; i<cnt; i++)
	{
		a[i*8  ] = elt[i].data[0].getX();
		a[i*8+1] = elt[i].data[0].getY();
		a[i*8+2] = elt[i].data[1].getX();
		a[i*8+3] = elt[i].data[1].getY();
		a[i*8+4] = elt[i].data[2].getX();
		a[i*8+5] = elt[i].data[2].getY();
		a[i*8+6] = elt[i].data[3].getX();
		a[i*8+7] = elt[i].data[3].getY();
	}

}

/***
	from the three reference points the max distance (squared for efficiency) in the image is estimated.
	the absolute allowed squared distance is calculated by multiplying with maxAllowedDistance, a user
	defined parameter. This parameter is also squared. So 0.01 means you allow only spots to match with
	eachother if they are maximally 10% of the maximum distance apart.
***/
double FingerPrint::determineMaxDist() {
	// reference points have been added to list as the first three elements!

	double maxDist = elt[0].sqrDist(elt[1]);
	double tmpDist = elt[0].sqrDist(elt[2]);
	if(tmpDist > maxDist)
		maxDist = tmpDist;
	tmpDist = elt[1].sqrDist(elt[2]);
	if(tmpDist > maxDist)
		maxDist = tmpDist;

	return maxDist * maxAllowedDistance;		// maxAllowedDistance is determined in xml file coming with the database
}
double FingerPrint::distance(const FingerPrint& f, Pair *pairs, int *nrpairs, int affine_corr)
{
	  double maxSqrDist = determineMaxDist();
    double totaldist = 0;
    paircnt = 0;

    if(pairs == 0 || nrpairs == 0)
        return -1;

    *nrpairs = 0;

    /***
	   process all possible point pairs. this is the most time consuming part of I3S. JdH May 13, 2007
	   calculation is done as match as possible with squared distances to prevent unnecessary sqrt operations.
	  ***/
    for(int i=0; i<cnt; i++)
    {
        double minSqrDst = 1000000000;
        double second  = 1000000000;
        int minj = -1;

        for(int j=0; j<f.cnt; j++)
        {
			// only compare similar elements
			if(elt[i].matches(f.elt[j]) == false)
				continue;

		    double sqrDist = elt[i].sqrDist(f.elt[j]);

            if(sqrDist < minSqrDst)
            {
                second = minSqrDst;
                minSqrDst = sqrDist;
                minj = j;
            } else if(sqrDist < second)
			    second = sqrDist;
        }

		/***
		   only accept a spot if the second best candidate is at a user defined relative distance and
		   the minimum distance is below a user defined relative maximum.
		   minRelativeDistance is defined in xml file coming with the database
		***/
		if(minSqrDst*minRelativeDistance <= second && minSqrDst < maxSqrDist)
        {
            double sqrtd = sqrt(minSqrDst);
      			const Element& from = elt[i];
      			const Element& to   = f.elt[minj];

      			totaldist += sqrtd*from.calcSimilarityRate(to);
            paircnt++;

            pairs[*nrpairs].m1 = i;
            pairs[*nrpairs].m2 = minj;
            pairs[*nrpairs].dist = sqrtd;
            *nrpairs = *nrpairs + 1;
        }
    }

    filterOutDuplicatePairs(pairs, nrpairs, totaldist);


    paircnt = *nrpairs + affine_corr;

   if(paircnt <= 0)
      score = 1000000.0;
   else
      score = (f.normfactor*totaldist) / sqr(paircnt);

   // extra penalty for low number of pairs
   if(paircnt == 1)
      score = score * 4.0;
   else
      if(paircnt == 2)
         score = score * 3.0;
	  else if(paircnt == 3)
         score = score * 2.0;

    paircnt -= affine_corr;

   // specific penalty for unpaired spots, notPairedRatio will alwyas be < 1. So dividing by it makes score bigger
   double notPairedRatio = ((double) (2.0*paircnt)) / (f.cnt + cnt);
   score = score / (notPairedRatio*notPairedRatio);

    return score;
}

/***
	This function tries to normalize results between different searches. It does not have any effect on the final
	ordering of the database in a search. Normalization helps to make sure that a score of e.g. 4.5 means more or
	less the same recognition quality between various searches in the database. Otherwise the calculated score
	depends largely on the
***/
void FingerPrint::calcNormFactor() {
	double tot = ref1.getDist(ref2);
	tot += ref1.getDist(ref3);
	tot += ref2.getDist(ref3);

	normfactor = 10000.0 / tot;		// 10000 is arbitrary value, but gives results of good matches somewhere between 0 and 20
}

void FingerPrint::resetScore()
{
   score = 1000000.0;
}

void FingerPrint::doAffine(double *matrix)
{
   if(elt == 0 || matrix == 0)
      return;

   for(int i=0; i<cnt; i++)
	   elt[i].doAffine(matrix);
}

static void filterOutDuplicatePairs(Pair *pairs, int *paircnt, double& totaldist)
{
   if(pairs == 0 || paircnt == 0 || *paircnt <= 0)
      return;

   qsort(pairs, *paircnt, sizeof(Pair), paircmp);

   for(int i=1; i<*paircnt; i++)
   {
      if(pairs[i-1].getM2() != pairs[i].getM2())
         continue;

      if(pairs[i-1].getDist() < pairs[i].getDist())
         removePair(pairs, i, paircnt, totaldist);
      else
         removePair(pairs, i-1, paircnt, totaldist);
      i--;
   }

}

static void removePair(Pair *pairs, int d, int *paircnt, double& totaldist)
{
   totaldist -= pairs[d].getDist();
   for(int i=d; i<*paircnt-1; i++)
      pairs[i] = pairs[i+1];
   *paircnt = *paircnt - 1;
}

static int paircmp(const void* p1, const void* p2)
{
   const Pair *i1 = (const Pair*) p1;
   const Pair *i2 = (const Pair*) p2;

   if(i1->getM2() < i2->getM2())
      return -1;
   return 1;
}

// C API for FingerPrint creation
extern "C" void* FingerPrint_create() {
   return new FingerPrint();
}