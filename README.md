# 2 Higgs Doublet Model Evolver

2 Higgs Doublet Model Evolver (**2HDME**) is a C++ program that can evolve a general
2HDM in renormalization energy scale, by solving the 1- or 2-loop 
Renormalization Group Equations (RGEs).

See manual for detailed instructions of how to use this program.

Please cite the manual if you use this program for a publication.

## Installation instructions

### Requirements 

* **2HDME** requires the library **Eigen** to perform linear algebra operations.
See http://eigen.tuxfamily.org/ for installation instructions.
If you have installed it, but the compiler don't find it, you can add the path
to its directory with 
  CFLAGS+=-I/.../eigen3 
in the Makefile.

* To solve the RGEs, **2HDME** uses the **GNU GSL** library, that is usually included in
GNU/Linux distributions. See https://www.gnu.org/software/gsl/ for details.

### Additional dependencies

These dependencies are optional and can be enabled/disabled by commenting the
relevant lines in the Makefile:

* The **2HDME** can automatically create simple plots of the RG running of the 
parameters with the help of **GNUPLOT**, see http://www.gnuplot.info/ for details.

* There is the possibility to link **2HDME** with **SPheno** [spheno.hepforge.org].
See the instructions below of how to install it.

### Installation

First, make sure that all the requirements are properly installed.
One might need configure *Makefile* to link all the libraries if they are not installed
in the usual location. After that, one can proceed to compile: In terminal, type
```
make
```
in *Makefile* directory. Please note that the RGEs in *RGE.cpp* are not written in an 
optimal form. However, the compiler with optimization level -03 will take care
of this. This takes some time and compiling *RGE.cpp* takes roughly 10 min on a laptop.

### Run Demo

To see that everything works, there is a demo that evolves a Z_2 symmetric CP 
conserved 2HDM from the top mass scale to the Planck scale. The source code is located in *src/demos/DemoRGE.cpp*.
To run it, type
```
bin/DemoRGE
```
in the terminal.
If the **GNUPLOT** functionality hasn't been disabled (by commenting out lines in
the Makefile), plots of the parameters should have been created in 
*output/DemoRGE/plots*.

### SPheno linkage

To enable **SPheno**, one must have a working installation of **SPheno** [spheno.hepforge.org] 
in the folder of **2HDME**. The folders name is by default *SPheno-4.0.3*, but can be changed by 
modifying *SPHENO_PATH* in *src/SPheno.cpp*. 

Next up, the model files for 2HDM must be compiled with your **SPheno**. The needed
model files have been created with **SARAH** [sarah.hepforge.org] and are included 
in *SPhenoModelFiles/THDM_GEN*. Copy *THDM_GEN* to the **SPheno** directory and run
*make* to compile it.

When **SPheno** and the 2HDM model files have been compiled, the SPheno functionality
in **2HDME** can be enabled by removing the relevant comments in the *Makefile*.

See *DemoRGE* for an example of how to compute loop corrected masses with **SPheno**. 

********************************************************************************

## Authors

Joel Oredsson ( joel_ored@hotmail.com )

### Acknowledgments 

This project started off as an extension of **2HDMC** [https://2hdmc.hepforge.org/];
therefore, the architectures of the two APIs are very similar. 

I would like to thank my PhD supervisor, Johan Rathsman, for support.

## License

[![DOI](https://zenodo.org/badge/149639202.svg)](https://zenodo.org/badge/latestdoi/149639202)

Copyright (C) 2018 Joel Oredsson.
2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.



