# cCartogram

Python and MATLAB wrappers for Mark Newman's C-library to compute cartograms 
using the method by
[Gastner and Newman (2004)](http://www.pnas.org/cgi/content/abstract/101/20/7499]).

Make sure to check out the [web site of the original code](http://www-personal.umich.edu/~mejn/cart/).

## Examples

### Python

Find an example below. For more examples, check out the `sandbox` subfolder
where you can find a script for the `uspop` sample from the original repository.

```python
import numpy as np
import cCartogram as cart

xsize = 128
ysize = 128

# random density matrix
A = np.random.randn(xsize,ysize) + 3

# compute cartogram
cartogram = cart.compute_cartogram(A.tolist())

# let's transform some coordinates
old_coords = [ (3.5, 5.),
               (64., 99.)
             ]
# those have to be a list of tuples of floats!

new_coords = cart.remap_coordinates(old_coords,
                                    cartogram,
                                    xsize,
                                    ysize
                                    )
```

### MATLAB

Check out the examples in `sandbox`.

## Install

For all systems, first clone this repository.

### Prerequisites

#### pybind11

Be sure you have [pybind11](https://github.com/pybind/pybind11) installed. 
Even though `pip` should take care of it, sometimes building might fail.

#### fftw3

You need to have the C library `fftw3` installed. You can [download it here](http://www.fftw.org/download.html). Make sure you read the notes on [installation](http://www.fftw.org/fftw3_doc/Installation-and-Customization.html).

Otherwise, run `./install_fftw.sh` which contains the following lines of code.

```bash
wget ftp://ftp.fftw.org/pub/fftw/fftw-3.3.7.tar.gz
tar -xvzf fftw-3.3.7.tar.gz
cd fftw-3.3.7
./configure && make
sudo make install
```


### Python

    $ sudo pip install ./pycartogram

If your compiler has trouble finding the library `fftw3` please note that you have to add both the include and library paths to `setup.py`. I've added `/usr/local/include` and `/usr/local/lib` since those are my standard library paths. If this is different on your machine you have to add the paths to the `include_dirs` and `library_dirs` arguments in lines `27-34` of `setup.py`.

### Matlab (Mac OSX)

You need to have the current XCode version installed (free in AppStore). Open Matlab and change into the directory of the repository. At first, there's two files you need to change.

```matlab
>>> cd /path/to/repository
>>> edit ([matlabroot '/bin/maci64/mexopts/clang++_maci64.xml'])
>>> edit ([matlabroot '/bin/maci64/mexopts/clang_maci64.xml'])
```

In both files, copy lines matching occurences of `MacOSX10.x.sdk` and change `MacOSX10.x.sdk` to `MacOSX10.11.sdk`(or whichever current version of XCode you're using).

Furthermore, it's important to know where your `fftw3` library was installed to. For me it's `/usr/local/lib` and `/usr/local/include`, so those are added as flags for MATLAB's compiling tool `mex` in `setup.m`. If those paths are different for you, please change them in lines `9-10` of `setup.m`.

Now, run

```matlab
>>> setup
```
