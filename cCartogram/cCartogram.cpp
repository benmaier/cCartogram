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

PYBIND11_PLUGIN(cCartogram) {
    py::module m("cCartogram", "Module to compute Mark Newman's cartograms.");
    
    m.def("compute_cartogram", &compute_cartogram, "Give a matrix (read: list of lists of floats) of density values and return a list of dislocations (x,y).",
            py::arg("density"),
            py::arg("offset") = 0.005,
            py::arg("blur") = 0.0,
            py::arg("show_progress") = false
            );

    m.def("remap_coordinates", &remap_coordinates, "Give a list of coordinates (read: list of tuples of two floats) and remap them according to the computed cartogram.",
            py::arg("coordinates"),
            py::arg("cartogram"),
            py::arg("xsize"),
            py::arg("ysize")
            );


    return m.ptr();

}
