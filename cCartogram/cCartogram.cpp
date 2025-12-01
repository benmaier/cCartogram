/* 
 * The MIT License (MIT)
 * Copyright (c) 2018, Benjamin F. Maier
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

#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cartogram.h"
#include "remap_coordinates.h"

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(cCartogram, m) {

    m.doc() = R"pbdoc(
        cCartogram: Density-equalizing cartogram computation

        A Python wrapper for Mark Newman's C implementation of the diffusion-based
        cartogram algorithm by Gastner and Newman (PNAS, 2004).

        Cartograms are map projections where geographic regions are rescaled
        according to some variable (e.g., population), distorting the geometry
        while preserving topology.

        Functions
        ---------
        compute_cartogram
            Compute a cartogram transformation grid from a density matrix.
        remap_coordinates
            Transform coordinates using a precomputed cartogram grid.

        Example
        -------
        >>> import cCartogram as cart
        >>> import numpy as np
        >>> density = np.random.rand(64, 64) + 1  # Must be positive
        >>> cartogram = cart.compute_cartogram(density.tolist())
        >>> coords = [(10.5, 20.0), (30.0, 40.5)]
        >>> new_coords = cart.remap_coordinates(coords, cartogram, 64, 64)

        References
        ----------
        - Gastner & Newman (2004): https://doi.org/10.1073/pnas.0400280101
        - Original C code: http://www-personal.umich.edu/~mejn/cart/
    )pbdoc";

    m.def("compute_cartogram", &compute_cartogram, R"pbdoc(
        Compute a density-equalizing cartogram transformation grid.

        Uses the diffusion method of Gastner and Newman to compute a cartogram
        from a 2D density matrix. The result is a displacement grid that can be
        used with `remap_coordinates` to transform arbitrary coordinates.

        Parameters
        ----------
        density : list of lists of float
            A 2D matrix of density values (e.g., population density). All values
            must be positive. The matrix shape determines the cartogram grid size.
        offset : float, optional
            Small positive value added to density to avoid division by zero and
            improve numerical stability. Default is 0.005.
        blur : float, optional
            Gaussian blur radius applied to the density before computation.
            Useful for smoothing noisy input data. Default is 0.0 (no blur).
        show_progress : bool, optional
            If True, print progress information during computation. Default is False.

        Returns
        -------
        list of tuple of (float, float)
            A flattened list of (x, y) displacement pairs representing the
            transformed grid coordinates. The list has length xsize * ysize,
            stored in row-major order.

        Example
        -------
        >>> import numpy as np
        >>> import cCartogram as cart
        >>> # Create a density matrix with higher values in one region
        >>> density = np.ones((128, 128))
        >>> density[32:96, 32:96] = 4.0  # Higher density in center
        >>> cartogram = cart.compute_cartogram(density.tolist(), blur=2.0)
    )pbdoc",
            py::arg("density"),
            py::arg("offset") = 0.005,
            py::arg("blur") = 0.0,
            py::arg("show_progress") = false
            );

    m.def("remap_coordinates", &remap_coordinates, R"pbdoc(
        Transform coordinates according to a precomputed cartogram.

        Takes a list of (x, y) coordinates and transforms them using the
        displacement grid computed by `compute_cartogram`. Coordinates are
        interpolated bilinearly between grid points.

        Parameters
        ----------
        coordinates : list of tuple of (float, float)
            List of (x, y) coordinate pairs to transform. Coordinates should
            be in the range [0, xsize) and [0, ysize) respectively.
        cartogram : list of tuple of (float, float)
            The cartogram displacement grid returned by `compute_cartogram`.
        xsize : int
            Width of the original density matrix used to compute the cartogram.
        ysize : int
            Height of the original density matrix used to compute the cartogram.

        Returns
        -------
        list of tuple of (float, float)
            The transformed coordinates in the same order as the input.

        Example
        -------
        >>> import cCartogram as cart
        >>> # After computing a cartogram...
        >>> old_coords = [(10.5, 20.0), (50.0, 75.5)]
        >>> new_coords = cart.remap_coordinates(old_coords, cartogram, 128, 128)
        >>> for old, new in zip(old_coords, new_coords):
        ...     print(f"{old} -> {new}")
    )pbdoc",
            py::arg("coordinates"),
            py::arg("cartogram"),
            py::arg("xsize"),
            py::arg("ysize")
            );
}
