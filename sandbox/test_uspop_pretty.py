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
A_orig = np.loadtxt('uspop.dat').T
print(f"Original data shape: {A_orig.shape}")

A = A_orig
print(f"Data shape: {A.shape}")

# Create coordinates for every point in the matrix
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
xmin, xmax = 330, 690
ymin, ymax = 160, 375

# Create figure with prettier styling - dark background
plt.style.use('dark_background')
fig, axes = plt.subplots(1, 2, figsize=(16, 7))

# Use a beautiful colormap
cmap = plt.cm.magma

# Use contour to get connected boundary lines for each unique density value
print("extracting boundary contours ...")
unique_vals = np.unique(A)
from matplotlib.path import Path

# Collect all contour path segments from original (split at MOVETO commands)
all_segments_orig = []  # list of continuous line segments
for val in unique_vals:
    mask = (A == val).astype(float)
    fig_tmp, ax_tmp = plt.subplots()
    cs = ax_tmp.contour(mask.T, levels=[0.5])
    plt.close(fig_tmp)
    # Extract paths and split at MOVETO commands
    for path in cs.get_paths():
        vertices = path.vertices
        codes = path.codes
        if codes is None:
            # No codes means single continuous path
            all_segments_orig.append(vertices.tolist())
        else:
            # Split at MOVETO (code 1)
            current_segment = []
            for v, c in zip(vertices, codes):
                if c == Path.MOVETO and current_segment:
                    all_segments_orig.append(current_segment)
                    current_segment = []
                current_segment.append(v.tolist())
            if current_segment:
                all_segments_orig.append(current_segment)

# Flatten all segment vertices for transformation
all_contour_coords_orig = []
segment_lengths = []
for seg in all_segments_orig:
    segment_lengths.append(len(seg))
    for v in seg:
        all_contour_coords_orig.append((v[0], v[1]))

# Transform contour coordinates through cartogram
print("transforming contour coordinates ...")
contour_coords_new = cart.remap_coordinates(all_contour_coords_orig, cartogram, *A.shape)

# Rebuild segments from transformed coordinates
all_segments_new = []
idx = 0
for length in segment_lengths:
    seg_new = []
    for i in range(length):
        seg_new.append(contour_coords_new[idx])
        idx += 1
    all_segments_new.append(seg_new)

# Plot original (left) as filled image
ax1 = axes[0]
im1 = ax1.imshow(orig_grid_colors.T, origin='lower', cmap=cmap,
                  extent=[orig_x.min(), orig_x.max(), orig_y.min(), orig_y.max()], aspect='equal')
# Draw contours on original - use contour directly
for val in unique_vals:
    mask = (A == val).astype(float)
    ax1.contour(mask.T, levels=[0.5], colors='white', linewidths=0.5,
                extent=[0, A.shape[0], 0, A.shape[1]], origin='lower')
ax1.set_xlim(xmin, xmax)
ax1.set_ylim(ymax, ymin)  # flip y-axis
ax1.set_title('Original Geography', fontsize=16, fontweight='bold', color='white', pad=10)

# Plot transformed (right) as filled image with transformed contours as lines
ax2 = axes[1]
im2 = ax2.imshow(grid_colors.T, origin='lower', cmap=cmap,
                  extent=[x_new.min(), x_new.max(), y_new.min(), y_new.max()], aspect='equal')
# Draw each transformed segment as a connected line
for seg in all_segments_new:
    px = [p[0] for p in seg]
    py = [p[1] for p in seg]
    ax2.plot(px, py, '-', linewidth=0.5, color='white', alpha=0.8)
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
