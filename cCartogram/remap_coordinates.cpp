/* 
 * The MIT License (MIT)
 * Copyright (c) 2018, Mark Newman, Benjamin Maier
 *
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without 
 * restriction, including without limitation the rights to use, 
 * copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall 
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-
 * INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */

#include "remap_coordinates.h"

using namespace std;

int readpoints(
           vector < pair < double, double > > &cartogram,
           double **gridx, 
           double **gridy,
	       int xsize,
           int ysize
           )
{
  int ix,iy;

  size_t count = 0;

  for (iy=0; iy<=ysize; iy++) {
    for (ix=0; ix<=xsize; ix++) {
        gridx[ix][iy] = cartogram[count].first;
        gridy[ix][iy] = cartogram[count].second;
        ++count;
    }
  }

  return 0;
}

vector < pair < double, double > >
     remap_coordinates(
             vector < pair < double, double > > coordinates,             
             vector < pair < double, double > > cartogram,
             int xsize,
             int ysize
        )
{
  int ix,iy;
  double xin,yin;
  double xout,yout;
  double dx,dy;
  double **gridx,**gridy;

  /* Read in the grid of points */

  gridx = (double **) malloc((xsize+1)*sizeof(double*));
  for (int i=0; i<=xsize; i++) gridx[i] = (double *) malloc((ysize+1)*sizeof(double));
  gridy = (double **) malloc((xsize+1)*sizeof(double*));
  for (int i=0; i<=xsize; i++) gridy[i] = (double *) malloc((ysize+1)*sizeof(double));
  readpoints(cartogram,gridx,gridy,xsize,ysize);

  vector < pair < double, double > > result;

  for(int i = 0; i < coordinates.size(); i++)
  {
    xin = coordinates[i].first;
    yin = coordinates[i].second;
    if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
        throw domain_error("received a coordinate which is out of bounds"); 
    } else {
      ix = xin;
      dx = xin - ix;
      iy = yin;
      dy = yin - iy;
      xout = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy]
             + (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
      yout = (1-dx)*(1-dy)*gridy[ix][iy] + dx*(1-dy)*gridy[ix+1][iy]
             + (1-dx)*dy*gridy[ix][iy+1] + dx*dy*gridy[ix+1][iy+1];
    }
    result.push_back(make_pair(xout,yout));
  }

  return result;

}
