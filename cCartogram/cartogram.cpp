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

#include "cartogram.h"

using namespace std;

int readpop(vector < vector < double > > &density, double **rho, int xsize, int ysize, double offset)
{
  int ix,iy;
  double mean;
  double sum=0.0;

  for (iy=0; iy<ysize; iy++) {
    for (ix=0; ix<xsize; ix++) {
      sum += rho[ix][iy];
    }
  }

  mean = sum/(xsize*ysize);
  for (iy=0; iy<ysize; iy++) {
    for (ix=0; ix<xsize; ix++) {
      rho[ix][iy] += offset*mean;
    }
  }

  return 0;
}


void creategrid(double *gridx, double *gridy, int xsize, int ysize)
{
  int ix,iy;
  int i = 0;

  for (iy=0; iy<=ysize; iy++) {
    for (ix=0; ix<=xsize; ix++) {
      gridx[i] = ix;
      gridy[i] = iy;
      i++;
    }
  }
}


vector < pair < double, double > >
     compute_cartogram(
             vector < vector < double > > density,
             double offset,
             double blur
        )
{
  int xsize,ysize;
  double *gridx,*gridy;  // Array for grid points
  double **rho;          // Initial population density

  xsize = density.size();
  ysize = density[0].size();

  cout << "xsize = " << xsize << "; ysize = " << ysize << endl;

  /* Allocate space for the cartogram code to use */

  cart_makews(xsize,ysize);

  /* Read in the population data, transform it, then destroy it again */

  rho = cart_dmalloc(xsize,ysize);
  readpop(density,rho,xsize,ysize,offset);

  cart_transform(rho,xsize,ysize);
  cart_dfree(rho);

  /* Create the grid of points */

  gridx = (double *) malloc((xsize+1)*(ysize+1)*sizeof(double));
  gridy = (double *) malloc((xsize+1)*(ysize+1)*sizeof(double));
  creategrid(gridx,gridy,xsize,ysize);

  /* Make the cartogram */

  cart_makecart(gridx,gridy,(xsize+1)*(ysize+1),xsize,ysize,blur);

  /* Write out the final positions of the grid points */

  vector < pair < double, double > > result;

  for (int i=0; i<xsize*ysize; i++) 
  {
      result.push_back(make_pair(gridx[i],gridy[i]));
  }

  cart_freews(xsize,ysize);
  free(gridx);
  free(gridy);

  return result;

}
