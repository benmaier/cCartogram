import numpy as np
import cCartogram as cart
import matplotlib.pyplot as pl

def convert_to_float(*args):
    """ convert the given arrays to float arrays """
    new = []
    for ar in args:
        ar = np.array(ar,dtype=float)
        new.append(ar)
    return new

def get_edges(A):
    """ 
    get a density matrix with discrete levels of density
    and return a list of coordinates of the edges
    """
    x_diff = A[1:,:] - A[:-1,:]
    y_diff = A[:,1:] - A[:,:-1]
    rowx, colx = np.nonzero(x_diff)
    rowy, coly = np.nonzero(y_diff)
    rowx, colx, rowy, coly = convert_to_float(rowx,colx,rowy,coly)
    return list(set(zip(rowx,colx) + zip(rowy,coly)))

# load transposed data from file and plot
A = np.loadtxt('uspop.dat').T
fig, ax = pl.subplots(2,1,figsize=(5,5))
ax[0].imshow(np.log(A).T)

# compute roughly coordinates of edges
coordinates = get_edges(A)
x = [ c[0] for c in coordinates ]
y = [ c[1] for c in coordinates ]

# plot edges
ax[0].plot(x,y,'o',markersize=0.5,color='k')

# get bounds
xlim = min(x), max(x)
ylim = min(y), max(y)

ax[0].set_xlim(xlim)
ax[0].set_ylim(ylim[::-1])

# compute cartogram
cartogram = cart.compute_cartogram(A.tolist(),show_progress=True)

# remap the edge coordinates
new_coords = cart.remap_coordinates(coordinates,cartogram,*A.shape)
x = [ c[0] for c in new_coords ]
y = [ c[1] for c in new_coords ]

# plot old density and transformed result 
ax[1].imshow(np.log(A).T)
ax[1].plot(x,y,'o',markersize=0.5,color='k')

# get bounds
xlim = min(x), max(x)
ylim = min(y), max(y)

ax[1].set_xlim(xlim)
ax[1].set_ylim(ylim[::-1])

# save figure
fig.savefig('uspop.png')

pl.show()
