#!/bin/bash

wget ftp://ftp.fftw.org/pub/fftw/fftw-3.3.7.tar.gz
tar -xvzf fftw-3.3.7.tar.gz
cd fftw-3.3.7
./configure && make
sudo make install
