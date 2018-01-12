% file encoding: utf8
% The MIT License (MIT)
% Copyright (c) 2018 Benjamin F. Maier
% 
% Permission is hereby granted, free of charge, to any person 
% obtaining a copy of this software and associated documentation 
% files (the "Software"), to deal in the Software without 
% restriction, including without limitation the rights to use, 
% copy, modify, merge, publish, distribute, sublicense, and/or 
% sell copies of the Software, and to permit persons to whom the 
% Software is furnished to do so, subject to the following conditions:
% 
% The above copyright notice and this permission notice shall 
% be included in all copies or substantial portions of the Software.
% 
% THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
% EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
% OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-
% INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
% BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
% AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
% OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
% IN THE SOFTWARE.

% loading data
density = importdata('uspop.dat');

% this data is a bit mean in the sense that it's transposed
% hence in plotting we have to switch x and y
A = density;

% finding coordinates of edges
xdiff = A(2:end,1:end) - A(1:(end-1),1:end);
ydiff = A(1:end,2:end) - A(1:end,1:(end-1));

[rowx, colx] = find(xdiff);
[rowy, coly] = find(ydiff);

edges = [rowx, colx; rowy, coly];

% plotting density
imagesc(log(A));
hold on;
plot(edges(:,2),edges(:,1),'.','MarkerSize',0.1);

% computing cartogram
offset = 0.005;
blur = 4;
cart = cartogram_compute(A,offset,blur);

% transforming to new coordinates
% matlab matrix coordinates start at 1 while
% the cartogram algorithm starts at 0
% hence we have to shift the coordinates by 1
[xsize, ysize] = size(A);
new_coords = cartogram_remap_coordinates(edges-1,cart,xsize,ysize) + 1;

% plotting new edges
plot(new_coords(:,2),new_coords(:,1),'d','MarkerSize',1)
