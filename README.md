# 2 Higgs Doublet Model Evolver v.1.2

2 Higgs Doublet Model Evolver (**2HDME**) is a C++ program that can evolve a
general 2HDM in renormalization energy scale, by solving the 1- or 2-loop
Renormalization Group Equations (RGEs).

The purpose of **2HDME** is to provide an API that consists of methods to 
manipulate a 2HDM model; thus the idea is that the user should write their own 
executable code that uses the THDM class of **2HDME**. An example is included
in *src/demos/DemoRGE.cpp*.

### Manual

A more detailed description of the program's structure, its classes and some of 
their functions can be found in the user manual at 
[https://arxiv.org/abs/1811.08215]. It also contains a very brief review of the
physics; a more in depth discussion of how **2HDME** can be used to perform
a RGE analysis of the 2HDM can be found in arXiv:1810.02588.

The source code is also fairly well documented; the header files contain 
comments about the classes and their functions.   

### Brief program structure summary

Inheritance structure:
    BaseModel -> RgeModel -> THDM
    BaseModel -> RgeModel -> SM
The main class of **2HDME** is THDM. It inherits RG evolution functionality
from RgeModel; which inherits basic input/output functionalities from BaseModel.
It requires a SM object to set up some of its parameters. The SM class also
inherits in the same way.

Both THDM and SM have their own set of RGEs which are specified in RGE.cpp.

All the scalar potential bases that are implemented are defined in 
*THDM_bases.cpp*.

See the manual for more details.

## Installation instructions

### Requirements 

* **2HDME** requires the library **Eigen** to perform linear algebra operations.
  See http://eigen.tuxfamily.org/ for installation instructions.  If you have
  installed it, but the compiler don't find it, you can add the path to its
  directory with CFLAGS+=-I/.../eigen3 in the Makefile.

* To solve the RGEs, **2HDME** uses the **GNU GSL** library, that is usually
  included in GNU/Linux distributions. See https://www.gnu.org/software/gsl/ for
  details.

* To use HiggsBounds and HiggsSignals, one needs to download and compile the two
  programs respectively. The libraries libHB and libHS should be placed in lib
  directory.

### Additional dependencies

These dependencies are optional and can be enabled/disabled by commenting the
relevant lines in the Makefile:

* The **2HDME** can automatically create simple plots of the RG running of the
  parameters with the help of **GNUPLOT**, see http://www.gnuplot.info/ for
  details.

* There is the possibility to link **2HDME** with **SPheno**
  [spheno.hepforge.org].  See the instructions below of how to install it.

### Installation

First, make sure that all the requirements are properly installed.  One might
need configure *Makefile* to link all the libraries if they are not installed in
the usual location. After that, one can proceed to compile: In terminal, type
```
make 
``` 
in *Makefile* directory. Please note that the RGEs in *RGE.cpp* are
not written in an optimal form. However, the compiler with optimization level
-03 will take care of this. This takes some time and compiling *RGE.cpp* takes
roughly 10 min on a laptop.

### Run Demo

To see that everything works, there is a demo that evolves a Z_2 symmetric CP
conserved 2HDM from the top mass scale to the Planck scale. The source code is
located in *src/demos/DemoRGE.cpp*.  To run it, type 
``` 
bin/DemoRGE 
``` 
in the terminal.  If the **GNUPLOT** functionality hasn't been disabled (by
commenting out lines in the Makefile), plots of the parameters should have been
created in *output/DemoRGE/plots*.

### Versions

* v.1.2 New EDM class. 
* v.1.1 Implemented oblique parameters, Global class with constant variables.
  Also new SLHA class for easier handle of input/output to textfiles.
* v.1.0 First version.

********************************************************************************

## Source code description

Directories:

* *SPhenoModelFiles*: Contains output from **SARAH** that should be used if one
wants to link SPheno.
* *src*: Source code
* *src/demos*: Short demo applications that use 2HDME.
* *src/RGEs*: Header files that contain the RGEs for 2HDM and SM in separate 
files.

Files in *src*:
* *BaseModel* (.h and .cpp): Abstract base class that RgeModel inherits from.
* *EDM* (.h and .cpp): Class that calculates the eEDM.
* *FileSystem* (.h and .cpp): Class that handles I/O to files.
* *GnuPlotSystem* (.h and .cpp): Class that can create gnuplot scripts and execute
commands that exports postscript plots.
* *Globals* (.h and .cpp): Collection of global constant variables.
* *HelpFunctions* (.h and .cpp): Miscellaneous structs and functions. Contains a 
class that is used to print tables to the console.
* *LoggingSystem* (.h and .cpp): Class that I/O to console for BaseModel
classes.
* *NewModel* (.h and .cpp): An example class that inherits from RgeModel. It
represents QED.
* *RGE* (.h and .cpp): Functions that calculate the RGEs that are contained in 
*src/RGEs*.
* *RgeModel* (.h and .cpp): Class that handles the RG evolution functionality for
derived classes. Inherits from BaseModel.
* *SM* (.h and .cpp): Class for standard model. Inherits from RgeModel.
* *Structures* (.h and .cpp): Miscellaneous structs.
* *THDM* (.h and .cpp): Class that represents the 2HDM. Inherits from RgeModel.
* *THDM_bases* (.h and .cpp): Contains structs that are different 
parameterizations of the 2HDM scalar potential. They have functions that 
transform them into eachother.

## Authors

Joel Oredsson ( joel_ored@hotmail.com )

### Citing 

Please cite [arXiv:1810.02588,1811.08215] if you use this program for a
publication.  And if you use the SPheno functionality with the model files that
are generated with SARAH, please cite the relevant papers as it says in
[spheno.hepforge.org] and [sarah.hepforge.org].

### Acknowledgments 

This project started off as an extension of **2HDMC**
[https://2hdmc.hepforge.org/]; therefore, the architectures of the two APIs are
very similar. 

This work has been done in collaboration with Johan Rathsman.

## License

[![DOI](https://zenodo.org/badge/149639202.svg)](https://zenodo.org/badge/latestdoi/149639202)

Copyright (C) 2018 Joel Oredsson.  2HDME is licenced under the GNU GPL v3 or
later, see LICENSE for details.



