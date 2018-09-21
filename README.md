# 2 Higgs Doublet Model Evolver README

2 Higgs Doublet Model Evolver (2HDME) is a C++ program that can evolve a general
2HDM in renormalization energy scale, by solving the 1- or 2-loop 
Renormalization Group Equations (RGEs).

See manual for detailed instructions of how to use this program.

Please cite the manual if you use this program for a publication.

## Installation instructions

### Dependencies 

* 2HDME requires the library Eigen to perform linear algebra operations.
See http://eigen.tuxfamily.org/ for installation instructions.
If you have installed it, but the compiler don't find it, you can add the path
to its directory with 
  CFLAGS+=-I/.../eigen3 
in the Makefile.

* To solve the RGEs, 2HDME uses the GNU GSL library, that is usually included in
GNU/Linux distributions. See https://www.gnu.org/software/gsl/ for details.

* The 2HDME can automatically create simple plots of the RG running of the 
parameters with the help of GNUPLOT, see http://www.gnuplot.info/ for details.
This functionality can be disabled by commenting the relevant lines in the 
Makefile.

### Compile

In terminal, type
```
make
```
in Makefile directory. The RGEs in RGE.cpp are not written in an 
optimal form. However, the compiler with optimization level -03 will take care
of this. This takes some time and compiling RGE.cpp takes ~10 min on a laptop.

### Run Demo

To see that everything works, there is a demo that evolves a Z2 symmetric CP 
conserved 2HDM from the top mass scale to the Planck scale. The source code is located in src/demos/DemoRGE.cpp.
To run it, type
```
bin/DemoRGE
```
in the terminal.
If the GNUPLOT functionality hasn't been disabled (by commenting out lines in
the Makefile), plots of the parameters should have been created in 
output/DemoRGE/plots.

********************************************************************************

## Authors

Joel Oredsson ( joel_ored@hotmail.com )

## License

Copyright (C) 2018 Joel Oredsson.
2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
Alarm clock



