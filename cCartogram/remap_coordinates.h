/* Routines to use the transformation functions given in cart.cpp
 *
 * Written by and copyright (c) Mark Newman, modified, with permission, by Benjamin F. Maier
 *
 * See http://www.umich.edu/~mejn/ for further details.
 */

#ifndef __REMAP_COORDINATES_H__
#define __REMAP_COORDINATES_H__

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <tuple>

using namespace std;

void cast_grids(
           vector < pair < double, double > > &cartogram,
           double **gridx, 
           double **gridy,
	       int xsize,
           int ysize
           );

vector < pair < double, double > >
     remap_coordinates(
             vector < pair < double, double > > coordinates,             
             vector < pair < double, double > > cartogram,
             int xsize,
             int ysize
        );

#endif
