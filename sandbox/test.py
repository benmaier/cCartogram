# The MIT License (MIT)
# Copyright (c) 2018 Benjamin Maier
# 
# Permission is hereby granted, free of charge, to any person 
# obtaining a copy of this software and associated documentation 
# files (the "Software"), to deal in the Software without 
# restriction, including without limitation the rights to use, 
# copy, modify, merge, publish, distribute, sublicense, and/or 
# sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall 
# be included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-
# INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
# AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
# OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
# IN THE SOFTWARE.

import numpy as np
import matplotlib.pyplot as pl

import cCartogram as cart

# dimensions of the input matrix
xsize = 128
ysize = 128

np.random.seed = 137

use_sinus = True
#use_sinus = False

if use_sinus:

    # put twodimensional sinus function in a matrix
    smallA = np.ones((xsize/2,ysize/2))
    halfsize = xsize/2
    for i in range(halfsize):
        for j in range(halfsize):
            smallA[i,j] = 2+np.sin(i/float(halfsize)*2*np.pi) *\
                            np.sin(j/float(halfsize)*2*np.pi)

    # but bigger matrix with mean value of A around it
    mean_A = smallA.mean()
    bigA = mean_A * np.ones((xsize,ysize))
    bigA[xsize/4:3*xsize/4,ysize/4:3*ysize/4] = smallA

else:
    # generate four blocks of random values
    vals = 3 + np.random.randn(2,2)

    # get big matrix containing their mean
    mean_A = vals.mean()
    bigA = mean_A * np.ones((xsize,ysize))
    quartersize = xsize/4

    # put the blocks in the big matrix
    for i in range(2):
        for j in range(2):
            bigA[ (i+1)*quartersize:(i+2)*(quartersize),
                  (j+1)*quartersize:(j+2)*(quartersize) \
                ] = vals[i,j] * np.ones((quartersize,quartersize))

                
# compute coordinates for lattice lines
quartersize = xsize/4
incre_coord = np.linspace(quartersize,3.*quartersize,100)
const_coord = quartersize * np.ones_like(incre_coord)

lines = [ (i*const_coord-0.5,incre_coord-0.5) for i in range(1,4) ] +\
        [ (incre_coord-0.5,i*const_coord-0.5) for i in range(1,4) ]
            
# show density matrix (transposed because
# otherwise matplotlib would not figure out the right coordinate system
fig, ax = pl.subplots(1,2,figsize=(8,4))
ax[0].imshow(bigA.T)
ax[1].imshow(bigA.T)

# plot lines
for x,y in lines:
    ax[0].plot(x,y,'k')

# generate cartograms
cartogram = cart.compute_cartogram(bigA.tolist(),offset=0.005,blur=5,show_progress=True)

# transform lines according to cartogram
for x,y in lines:
    coo = zip(x,y)
    coords = cart.remap_coordinates(coo,cartogram,xsize,ysize)
    x = [c[0] for c in coords]
    y = [c[1] for c in coords]
    ax[1].plot(x,y,'r')

if use_sinus:
    fig.savefig('sinus.png')
else:
    fig.savefig('blocks.png')

pl.show()
