import cCartogram

import numpy as np

xsize = 32
ysize = 32
A = np.random.randn(xsize,ysize) + 100

print cCartogram.compute_cartogram(A.tolist())



