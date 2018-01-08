/* Routines to use the transformation functions given in cart.cpp
 *
 * Written by and copyright (c) Mark Newman, modified, with permission, by Benjamin F. Maier
 *
 * See http://www.umich.edu/~mejn/ for further details.
 */

#ifndef __CARTOGRAM_H__
#define __CARTOGRAM_H__

#include "cart.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <tuple>

using namespace std;

void cast_density(vector < vector < double > > &density, double **rho, int xsize, int ysize, double OFFSET);

void create_grids(double *gridx, double *gridy, int xsize, int ysize);

vector < pair < double, double > >
     compute_cartogram(
             vector < vector < double > > density,
             double offset = 0.005,
             double blur = 0.0,
             bool show_progress = false
        );

#endif
