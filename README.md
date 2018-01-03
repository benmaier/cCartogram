# pycartogram

A python wrapper for Mark Newman's C-library to compute cartograms 
using the method by
[Gastner and Newman (2004)](http://www.pnas.org/cgi/content/abstract/101/20/7499]).

## Install

For all systems, first clone this repository.

### Matlab (Mac OSX)

You need to have the current XCode version installed (free in AppStore). Open Matlab and change into the directory of the repository. At first, there's two files you need to change.

```matlab
>>> cd /path/to/repository
>>> edit ([matlabroot '/bin/maci64/mexopts/clang++_maci64.xml'])
>>> edit ([matlabroot '/bin/maci64/mexopts/clang_maci64.xml'])
```

In both files, copy lines matching occurences of `MacOSX10.x.sdk` and change `MacOSX10.x.sdk` to `MacOSX10.11.sdk`(or whichever current version of XCode you're using).

Now, run


```matlab
>>> setup
```

### Python

    $ sudo pip install ./pycartogram

