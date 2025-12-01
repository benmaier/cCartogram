/* Routines to use the transformation functions given in cart.cpp
 *
 * Written by and copyright (c) Mark Newman, modified, with permission, by Benjamin F. Maier
 *
 * See http://www.umich.edu/~mejn/ for further details.

Original code's license.txt:

cart2.c, cart2v.c, cart.c, cart.h, cartv.c, interp.c, main.c: C code
for generating cartograms using the diffusion method of Gastner and
Newman

Written by Mark Newman <mejn@umich.edu>
Version 1.2.3: 24 NOV 2025
This software is released under the Revised BSD License.  You may use,
share, or modify this file freely subject to the license terms below.

Copyright 2025, Mark Newman

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */


#include "remap_coordinates.h"

using namespace std;

void cast_grids(
           vector < pair < double, double > > &cartogram,
           double **gridx, 
           double **gridy,
	       int xsize,
           int ysize
           )
{
  size_t count = 0;

  for (int j = 0; j <= ysize; j++) {
    for (int i = 0; i <= xsize; i++) {
        gridx[i][j] = cartogram[count].first;
        gridy[i][j] = cartogram[count].second;
        ++count;
    }
  }
}

vector < pair < double, double > >
     remap_coordinates(
             vector < pair < double, double > > coordinates,             
             vector < pair < double, double > > cartogram,
             int xsize,
             int ysize
        )
{
  double **gridx = new double*[xsize+1];
  double **gridy = new double*[xsize+1];

  /* Read in the grid of points */

  for (int i=0; i<=xsize; i++) 
  {
      gridx[i] = new double[ysize+1];
      gridy[i] = new double[ysize+1];
  }

  /*
  double **gridx;
  double **gridy;
  gridx = (double **) malloc((xsize+1)*sizeof(double*));
  gridy = (double **) malloc((xsize+1)*sizeof(double*));
  for (i=0; i<=xsize; i++)
  { 
      gridx[i] = (double *) malloc((ysize+1)*sizeof(double));
      gridy[i] = (double *) malloc((ysize+1)*sizeof(double));
  }
  */

  cast_grids(cartogram,gridx,gridy,xsize,ysize);

  vector < pair < double, double > > result;

  for(unsigned int i = 0; i < coordinates.size(); i++)
  {
    double xin = coordinates[i].first;
    double yin = coordinates[i].second;
    double xout, yout;
    if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
        throw domain_error("received a coordinate which is out of bounds"); 
    } else {
      int ix = xin;
      double dx = xin - ix;
      int iy = yin;
      double dy = yin - iy;
      xout = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy]
           + (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
      yout = (1-dx)*(1-dy)*gridy[ix][iy] + dx*(1-dy)*gridy[ix+1][iy]
           + (1-dx)*dy*gridy[ix][iy+1] + dx*dy*gridy[ix+1][iy+1];
    }
    result.push_back(make_pair(xout,yout));
  }

  for (int i=0; i<xsize+1; i++)
  {
      delete [] gridx[i];
      delete [] gridy[i];
      //free(gridx[i]);
  }

  delete [] gridx;
  delete [] gridy;
    
  return result;

}
