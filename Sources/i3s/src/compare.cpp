/*******************************************************************************
 *	I3S: Interactive Individual Identification System						   *
 *																			   *
 *	Copyright (C) 2004-2013  Jurgen den Hartog & Renate Reijns				   *
 *																			   *
 *	This program is free software; you can redistribute it and/or modify	   *
 *	it under the terms of the GNU General Public License as published by	   *
 *	the Free Software Foundation; either version 2 of the License, or		   *
 *	(at your option) any later version.										   *
 *																			   *
 *	This program is distributed in the hope that it will be useful,			   *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of			   *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			   *
 *	GNU General Public License for more details.							   *
 *																			   *
 *	You should have received a copy of the GNU General Public License		   *
 *	along with this program; see the file COPYING GPL v2.txt. If not,		   *
 *	write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,  *
 *	Boston, MA 02111-1307, USA.												   *
 *																			   *
 *******************************************************************************/


// #include <emscripten/bind.h>
// using namespace emscripten;
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// #include <windows.h>

#include "../incl/compare.hpp"
#include "../incl/affine.h"

using namespace std;

static int userMessage (const char *title, const char *mess);

bool Compare::compareTwo(FingerPrint& unknown, FingerPrint& fp, Pair *pairs, int *paircnt)
{
	if(!pairs)
	{
		char buf[100];
		sprintf(buf, "Parameter error in Compare::compareTwo   %d  %d\n", pairs, paircnt);
		userMessage ("Parameter error in Compare::compareTwo", buf);
		return false;
	}

	double matrix[6];
	unknown.addReference();
	fp.addReference();

	// Calculate linear transform to put spots within same reference space
	calcAffine(fp.ref1.getX(), fp.ref1.getY(),
			   fp.ref2.getX(), fp.ref2.getY(),
			   fp.ref3.getX(), fp.ref3.getY(),
			   unknown.ref1.getX(), unknown.ref1.getY(),
			   unknown.ref2.getX(), unknown.ref2.getY(),
			   unknown.ref3.getX(), unknown.ref3.getY(),
			   matrix);

	// printf("\n");
	// printf("Before affine: FGP1 kp1: %f, %f \n", fp.getElt(3).getCx(), fp.getElt(3).getCy());
	// printf("Before affine: FGP1 kp2: %f, %f \n", fp.getElt(4).getCx(), fp.getElt(4).getCy());
	// printf("Before affine: FGP1 kp3: %f, %f \n", fp.getElt(5).getCx(), fp.getElt(5).getCy());
	// printf("\n");
	// printf("Before affine: FGP2 kp1: %f, %f \n", unknown.getElt(3).getCx(), unknown.getElt(3).getCy());
	// printf("Before affine: FGP2 kp2: %f, %f \n", unknown.getElt(4).getCx(), unknown.getElt(4).getCy());
	// printf("Before affine: FGP2 kp3: %f, %f \n", unknown.getElt(5).getCx(), unknown.getElt(5).getCy());

	// Apply the linear transform to the fingerprint's points
	fp.doAffine(matrix);

	for (int idx = 0; idx < 6; idx ++) {
		// printf("m %f \n", matrix[idx]);
	}

	// printf("\n");
	// printf("After affine: FGP1 kp1: %f, %f \n", fp.getElt(3).getCx(), fp.getElt(3).getCy());
	// printf("After affine: FGP1 kp2: %f, %f \n", fp.getElt(4).getCx(), fp.getElt(4).getCy());
	// printf("After affine: FGP1 kp3: %f, %f \n", fp.getElt(5).getCx(), fp.getElt(5).getCy());
	// printf("\n");
	// printf("After affine: FGP2 kp1: %f, %f \n", unknown.getElt(3).getCx(), unknown.getElt(3).getCy());
	// printf("After affine: FGP2 kp2: %f, %f \n", unknown.getElt(4).getCx(), unknown.getElt(4).getCy());
	// printf("After affine: FGP2 kp3: %f, %f \n", unknown.getElt(5).getCx(), unknown.getElt(5).getCy());

	fp.distance(unknown, pairs, paircnt, -3);
/*
	if(*paircnt <= 8) {
		fp.setScore(1000.0);
		calcAllPotentialPairs(fp, unknown, pairs, *paircnt);
	}
	fprintf(stderr, "# pairs: %d\n", *paircnt);
*/

	exhaustiveSearch(fp, unknown, pairs, *paircnt);

	return true;
}

void Compare::calcAllPotentialPairs(const FingerPrint& from, const FingerPrint& to, Pair* pairs, int& paircnt)
{
	// calc maximum distance between ref's
	double d12 = from.ref1.getSqrDist(from.ref2);
	double d13 = from.ref1.getSqrDist(from.ref3);
	double d23 = from.ref2.getSqrDist(from.ref3);
	double maxSqDist = d12;
	if(d13 > maxSqDist)
		maxSqDist = d13;
	if(d23 > maxSqDist)
		maxSqDist = d23;
	maxSqDist = maxSqDist / 16.0;	// maximum distance is only 25% of the maximum distance between ref points
									// because of working with squares divide bij 4^2
	paircnt = 0;

	for(int i=0; i<from.cnt; i++) {
		double close1 = 1000000000.0;
		double close2 = 1000000000.0;
		int index1 = -1;
		int index2 = -1;

		for(int j=0; j<to.cnt; j++)
		{
			if(from.elt[i].matches(to.elt[j]) == false)
				continue;
			double d = from.elt[i].sqrDist(to.elt[j]);
			if(d > maxSqDist)
				continue;
			if(d < close1) {
				close2 = close1;
				index2 = index1;
				close1 = d;
				index1 = j;
			} else if(d < close2) {
				close2 = d;
				index2 = j;
			}
		}

		if(index1 > -1) {
			pairs[paircnt].set(i, index1, close1);
			paircnt++;
		}
		if(index2 > -1) {
			pairs[paircnt].set(i, index2, close2);
			paircnt++;
		}
	}
}

void Compare::exhaustiveSearch(FingerPrint& fp, const FingerPrint& unknown, Pair *pairs, int& paircnt)
{
	double matrix[6];

	FingerPrint best(fp);
	Pair bestPairs[100];
	int bestPairCnt=0;

	for(int j=0; j<paircnt-2; j++)
		for(int k=j+1; k<paircnt-1; k++)
			for(int l=k+1; l<paircnt; l++) {
				FingerPrint test(fp);

				Element from1 = test.getElt(pairs[j].getM1());
				Element from2 = test.getElt(pairs[k].getM1());
				Element from3 = test.getElt(pairs[l].getM1());
				Element to1	= unknown.getElt(pairs[j].getM2());
				Element to2	= unknown.getElt(pairs[k].getM2());
				Element to3	= unknown.getElt(pairs[l].getM2());

				// skip as many transformations in an early stage as possible
				//if(from1.matches(to1) == false || from2.matches(to2) == false || from3.matches(to3) == false)
				//	continue;

				if(calcAffine(from1.getCx(), from1.getCy(), from2.getCx(), from2.getCy(), from3.getCx(), from3.getCy(),
						      to1.getCx(),	 to1.getCy(),   to2.getCx(),   to2.getCy(),   to3.getCx(),   to3.getCy(),
						      matrix) == false)
				    continue;

				test.doAffine(matrix);
				Pair tmppairs[100];
				int tmppaircnt = 0;

				test.distance(unknown, tmppairs, &tmppaircnt, -3);

				if(test.getScore() < best.getScore())
				{
					memcpy(bestPairs, tmppairs, 100*sizeof(Pair));
					bestPairCnt = tmppaircnt;
					best = test;
				}
			}

	fp = best;

	if(bestPairCnt)
	{
		paircnt = bestPairCnt;
		memcpy(pairs, bestPairs, bestPairCnt*sizeof(Pair));
	}
}

int* Compare::dummyCompare() {
	int b = 7;
	int *a = &b;
	return a;
}

static int userMessage (const char *title, const char *mess)
{
	return 0;
   // return ::MessageBox(NULL, mess, title, MB_OK | MB_ICONWARNING | MB_APPLMODAL | MB_SETFOREGROUND );
}

// C API for comparing FingerPrints
extern "C" double Compare_compareTwo(FingerPrint& unknown, FingerPrint& fp) {
 	Pair *pairs = new Pair[100];
 	int paircnt = 0;
 	Compare::compareTwo(unknown, fp, pairs, &paircnt);
 	// printf("score %f", fp.getScore());
 	return fp.getScore();
}
