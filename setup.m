% use C++ in the following
mex -setup C++;

% compile function
mex -v CXXFLAGS='-O3 -std=c++11 -stdlib=libc++ -I./cCartogram -I./matlab -L/usr/local/lib -lfftw3' matlab/cartogram_compute.cpp matlab/CastResult.cpp cCartogram/cartogram.cpp cCartogram/cart.cpp 
mex -v CXXFLAGS='-O3 -std=c++11 -stdlib=libc++ -I./cCartogram -I./matlab -L/usr/local/lib -lfftw3' matlab/cartogram_remap_coordinates.cpp matlab/CastResult.cpp cCartogram/remap_coordinates.cpp cCartogram/cart.cpp 

% move compiled functions to new folder
mkdir matlabbuild
movefile('cartogram_compute*','./matlabbuild');
movefile('cartogram_remap_coordinates*','./matlabbuild');

% add path to matlab environment via startup-file in user directory
up = userpath;
startuppath = [up(1:end-1),'/startup.m'];
libpath = [pwd,'/matlabbuild'];

fid = fopen(startuppath, 'at');  % append to possibly existing startup-file
fprintf(fid,['\naddpath(''',libpath,''')\n']);
fclose(fid);

% add path for this session
addpath(libpath);
