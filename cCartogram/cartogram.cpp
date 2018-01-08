/* Routines to use the transformation functions given in cart.cpp
 *
 * Written by and copyright (c) Mark Newman, modified, with permission, by Benjamin F. Maier
 *
 * See http://www.umich.edu/~mejn/ for further details.
 */


#include "cartogram.h"

using namespace std;

void cast_density(vector < vector < double > > &density,
                  double **rho,
                  int xsize,
                  int ysize,
                  double offset)
{
  double mean = 0.;
  double sum = 0.;

  for (int i=0; i<ysize; i++) {
    for (int j=0; j<xsize; j++) {
      rho[i][j] = density[i][j];
      sum += rho[i][j];
    }
  }

  mean = sum / (xsize*ysize);
  for (int i = 0; i<ysize; i++) {
    for (int j = 0; j<xsize; j++) {
      rho[ix][iy] += offset*mean;
    }
  }
}


void create_grids(double *gridx,
                  double *gridy,
                  int xsize,
                  int ysize)
{
  int count = 0;

  for (int i = 0; i<=ysize; i++) {
    for (int j = 0; j<=xsize; j++) {
      gridx[count] = i;
      gridy[count] = j;
      count++;
    }
  }
}


vector < pair < double, double > >
     compute_cartogram(
             vector < vector < double > > density,
             double offset,
             double blur,
             bool show_progress
        )
{
  int xsize,ysize;
  double *gridx,*gridy;
  double **rho;

  xsize = density.size();
  ysize = density[0].size();

  //cout << "xsize = " << xsize << "; ysize = " << ysize << endl;

  /* Allocate space for the cartogram code to use */

  cart_makews(xsize,ysize);

  /* Read in the population data, transform it, then destroy it again */

  rho = cart_dmalloc(xsize,ysize);
  cast_density(density,rho,xsize,ysize,offset);

  cart_transform(rho,xsize,ysize);
  cart_dfree(rho);

  /* Create the grid of points */

  gridx = (double *) malloc((xsize+1)*(ysize+1)*sizeof(double));
  gridy = (double *) malloc((xsize+1)*(ysize+1)*sizeof(double));
  create_grids(gridx,gridy,xsize,ysize);

  /* Make the cartogram */

  cart_makecart(gridx,
                gridy,
                (xsize+1)*(ysize+1),
                xsize,
                ysize,
                blur,
                show_progress);

  /* Write out the final positions of the grid points */

  vector < pair < double, double > > result;

  for (int i=0; i<(xsize+1)*(ysize+1); i++)
  {
      result.push_back(make_pair(gridx[i],gridy[i]));
  }

  cart_freews(xsize,ysize);
  free(gridx);
  free(gridy);

  return result;

}
