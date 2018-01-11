/* Routines to use the transformation functions given in cart.cpp
 *
 * Written by and copyright (c) Mark Newman, modified, with permission, by Benjamin F. Maier
 *
 * See http://www.umich.edu/~mejn/ for further details.
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

  for(int i = 0; i < coordinates.size(); i++)
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
