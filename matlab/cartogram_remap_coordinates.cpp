#include "CastResult.h"
#include "remap_coordinates.h"
#include "math.h"
#include "matrix.h"
#include "mex.h"

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int xsize, ysize;
    
    if (nrhs!=4)
    {
        mexPrintf("Got %d input arguments. ", nrhs);
        throw length_error("Invalid number of input arguments. Has to be 4.");
    }

    if (nlhs!=1)
    {
        mexPrintf("Got %d output arguments. ", nrhs);
        throw length_error("Invalid number of output arguments. Has to be 1.");
    }

    vector < pair < double, double > > coordinates = get_edgelist(prhs[0]); 
    vector < pair < double, double > > cartogram = get_edgelist(prhs[1]); 

    read_single_value(prhs[2],xsize);
    read_single_value(prhs[3],ysize);
    vector < pair < double, double > >  
         coords = remap_coordinates(
                                        coordinates,
                                        cartogram,
                                        xsize,
                                        ysize
                                      );

    plhs[0] = cast_edgelist_to_matlab(coords.begin(), coords.end());
}
