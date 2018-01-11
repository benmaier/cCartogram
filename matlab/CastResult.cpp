/* 
 * The MIT License (MIT)
 * Copyright (c) 2018, Benjamin Maier
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
#include "CastResult.h"
#include <stdexcept>

using namespace std;

vector < pair < double, double > > get_edgelist(const mxArray * m_edges)
{
    double *m_edgelist;
    size_t numdims, dimx, dimy;
    const mwSize *dims;

    m_edgelist = mxGetPr(m_edges);
    dims = mxGetDimensions(m_edges);
    numdims = mxGetNumberOfDimensions(m_edges);
    dimx = int( dims[0] );
    dimy = int( dims[1] );

    //if (numdims != 2)
    //{
    //    mexPrintf("Wrong dimensions in edgelist: [%d,%d]",dimx,dimy);
    //    throw length_error();
    //}

    vector < pair<double,double> > edges;

    if (dimx>0)
    {
        if (dimy != 2)
        {
            mexPrintf("Wrong dimensions in edgelist: [%d,%d]\n",dimx,dimy);
            throw length_error("");
        }

        for(size_t x=0; x<dimx; x++)
        {
            double p[dimy];
 
            for(size_t y=0; y<dimy; y++)
                p[y] = m_edgelist[x+y*dimx];

            edges.push_back( make_pair(p[0], p[1]) );
        }

        return edges;
    }
    else if (dimy == 0)
    {
        return edges;
    }
    else
    {
        mexPrintf("Wrong dimensions in edgelist: [%d,%d]\n",dimx,dimy);
        throw length_error("");
    }

}

vector < vector < double > > get_matrix(const mxArray * m_)
{
    double *m_new;
    size_t numdims, dimx, dimy;
    const mwSize *dims;

    m_new = mxGetPr(m_);
    dims = mxGetDimensions(m_);
    numdims = mxGetNumberOfDimensions(m_);
    dimx = int( dims[0] );
    dimy = int( dims[1] );

    //if (numdims != 2)
    //{
    //    mexPrintf("Wrong dimensions in edgelist: [%d,%d]",dimx,dimy);
    //    throw length_error();
    //}

    vector < vector < double > > A(dimx, vector < double >(dimy));

    if (dimx>0)
    {
        if (dimy < 2)
        {
            mexPrintf("Wrong dimensions in matrix: [%d,%d]\n",dimx,dimy);
            throw length_error("");
        }

        for(size_t x=0; x<dimx; x++)
        {
            for(size_t y=0; y<dimy; y++)
                A[x][y] = m_new[x+y*dimx] ;
        }

        return A;
    }
    else if (dimy == 0)
    {
        return A;
    }
    else
    {
        mexPrintf("Wrong dimensions in matrix: [%d,%d]\n",dimx,dimy);
        throw length_error("");
    }

}
