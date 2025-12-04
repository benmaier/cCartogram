# The MIT License (MIT)
# Copyright (c) 2018 Benjamin F. Maier
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
import cCartogram as cart
import matplotlib.pyplot as plt
from scipy.interpolate import griddata

# load transposed data from file
A = np.loadtxt('uspop.dat').T
print(f"Data shape: {A.shape}")

# Create coordinates for every point in the original matrix
rows, cols = A.shape
x_orig = []
y_orig = []
colors = []

for i in range(rows):
    for j in range(cols):
        x_orig.append(float(i))
        y_orig.append(float(j))
        colors.append(A[i, j])

coordinates = list(zip(x_orig, y_orig))
colors = np.array(colors)

# Use log scale for colors and normalize
log_colors = np.log(colors + 1)  # +1 to avoid log(0)
log_colors_norm = (log_colors - log_colors.min()) / (log_colors.max() - log_colors.min())

# compute cartogram
print("computing cartogram ...")
cartogram = cart.compute_cartogram(A.tolist(), show_progress=True)

# remap all coordinates
print("remapping coordinates ...")
new_coords = cart.remap_coordinates(coordinates, cartogram, *A.shape)
x_new = np.array([c[0] for c in new_coords])
y_new = np.array([c[1] for c in new_coords])

# Create a filled image for the cartogram using nearest-neighbor interpolation
print("interpolating cartogram image ...")

orig_x = np.array(x_orig)
orig_y = np.array(y_orig)

# Create grid for transformed image
grid_res = 1.0
grid_x, grid_y = np.mgrid[x_new.min():x_new.max():grid_res, y_new.min():y_new.max():grid_res]
grid_colors = griddata(
    (x_new, y_new),
    log_colors_norm,
    (grid_x, grid_y),
    method='nearest'
)

# Create grid for original image
orig_grid_x, orig_grid_y = np.mgrid[orig_x.min():orig_x.max():grid_res, orig_y.min():orig_y.max():grid_res]
orig_grid_colors = griddata(
    (orig_x, orig_y),
    log_colors_norm,
    (orig_grid_x, orig_grid_y),
    method='nearest'
)

# Zoom bounds for the US region (same for both plots)
xmin, xmax = 180, 820
ymin, ymax = 160, 375

# Create figure with prettier styling - dark background
plt.style.use('dark_background')
fig, axes = plt.subplots(1, 2, figsize=(16, 7))

# Use a beautiful colormap
cmap = plt.cm.magma

# Plot original (left) as filled image
ax1 = axes[0]
im1 = ax1.imshow(orig_grid_colors.T, origin='lower', cmap=cmap,
                  extent=[orig_x.min(), orig_x.max(), orig_y.min(), orig_y.max()], aspect='equal')
ax1.set_xlim(xmin, xmax)
ax1.set_ylim(ymax, ymin)  # flip y-axis
ax1.set_title('Original Geography', fontsize=16, fontweight='bold', color='white', pad=10)

# Plot transformed (right) as filled image
ax2 = axes[1]
im2 = ax2.imshow(grid_colors.T, origin='lower', cmap=cmap,
                  extent=[x_new.min(), x_new.max(), y_new.min(), y_new.max()], aspect='equal')
ax2.set_xlim(xmin, xmax)
ax2.set_ylim(ymax, ymin)  # flip y-axis
ax2.set_title('Population Cartogram', fontsize=16, fontweight='bold', color='white', pad=10)

# Remove axes after setting limits
for ax in axes:
    ax.set_xticks([])
    ax.set_yticks([])
    for spine in ax.spines.values():
        spine.set_visible(False)

plt.tight_layout()
fig.savefig('uspop_pretty.png', dpi=150, bbox_inches='tight', facecolor='#1a1a2e')
print("Saved to uspop_pretty.png")

plt.show()
