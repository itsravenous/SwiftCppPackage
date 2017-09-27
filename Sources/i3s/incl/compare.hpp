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

#ifndef COMPARE_HPP
#define COMPARE_HPP 1


#include "fingerprint.hpp"

class Compare
{
public:
	Compare() {};
	~Compare(){};

    Compare(const Compare&);                     // not implemented on purpose
    const Compare& operator=(const Compare&);    // not implemented on purpose

    static bool compareTwo(FingerPrint& u, FingerPrint& f, Pair *pairs, int *paircnt);
    static void exhaustiveSearch(FingerPrint&, const FingerPrint&, Pair *pairs, int& paircnt);
    static void calcAllPotentialPairs(const FingerPrint& from, const FingerPrint& to, Pair* pairs, int& paircnt);
    static int* dummyCompare();
};

#endif