#include "CastResult.h"
#include "cartogram.h"
#include "math.h"
#include "matrix.h"
#include "mex.h"

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    double offset, blur;
    
    if (nrhs!=3)
    {
        mexPrintf("Got %d input arguments. ", nrhs);
        throw length_error("Invalid number of input arguments. Has to be 3.");
    }

    if (nlhs!=1)
    {
        mexPrintf("Got %d output arguments. ", nrhs);
        throw length_error("Invalid number of output arguments. Has to be 1.");
    }

    vector < vector < double > > density = get_matrix(prhs[0]); 

    read_single_value(prhs[1],offset);
    read_single_value(prhs[2],blur);
    vector < pair < double, double > >  
         coords = compute_cartogram(
                                        density,
                                        offset,
                                        blur
                                      );

    plhs[0] = cast_edgelist_to_matlab(coords.begin(), coords.end());
}
