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

  for (int i = 0; i <= ysize; i++) {
    for (int j = 0; j <= xsize; j++) {
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
  int ix,iy;
  double xin,yin;
  double xout,yout;
  double dx,dy;
  double **gridx = new double*[xsize+1];
  double **gridy = new double*[xsize+1];

  /* Read in the grid of points */

  for (int i=0; i<=xsize; i++) 
  {
      gridx[i] = new double[ysize+1];
      gridy[i] = new double[ysize+1];
  }

  cast_grids(cartogram,gridx,gridy,xsize,ysize);

  vector < pair < double, double > > result;

  for(int i = 0; i < coordinates.size(); i++)
  {
    xin = coordinates[i].first;
    yin = coordinates[i].second;
    if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
        throw domain_error("received a coordinate which is out of bounds"); 
    } else {
      int ix = xin;
      int dx = xin - ix;
      int iy = yin;
      int dy = yin - iy;
      int xout = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy]
               + (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
      int yout = (1-dx)*(1-dy)*gridy[ix][iy] + dx*(1-dy)*gridy[ix+1][iy]
               + (1-dx)*dy*gridy[ix][iy+1] + dx*dy*gridy[ix+1][iy+1];
    }
    result.push_back(make_pair(xout,yout));
  }

  for (int i=0; i<=xsize; i++)
  {
      delete gridx[i];
      delete gridy[i];
  }

  delete gridx;
  delete gridy;
    
  return result;

}
