# Makefile is a part of 2HDME.
# Copyright (C) 2018 Joel Oredsson.
# 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
################################################################################
# @brief: Makefile for 2HDME 
# @author: Joel Oredsson
# 
# This is a makefile for 2HDME. Modify it to your needs! 
# For installation instructions see the README.
#
# Compiling RGE.cpp takes a long time because of the compiler optimizations of 
# the 2-loop RGEs ( ~10 min on a laptop).
#
# The source code is written in C++11 and uses the packages:
#   gsl: For mathematical functions and solving ODEs.
#   Eigen: For linear algebra.
#   gtkmm-3.0: For a graphical user interface. This is only used by GUI.cpp.
#
################################################################################

# Choose your C++ compiler here (in general g++ on Linux systems):
CC = g++

#Optimisation level, eg: -O3
OPT=-O3

# Paths for source code that needs compiling
VPATH=src src/demos
MKDIR_P = mkdir -p
OUT_DIR=bin lib

CFLAGS= -pthread -std=c++11 -Wall $(OPT) -Isrc

OBJDIR=lib
LIBDIR=$(OBJDIR)

# Directory for executables
BINDIR=bin

# Source code:
RGESRC=BaseModel.cpp RgeModel.cpp THDM.cpp RGE.cpp \
HelpFunctions.cpp FileSystem.cpp LoggingSystem.cpp SM.cpp\
THDM_bases.cpp Structures.cpp
OBJECTS=$(RGESRC:.cpp=.o)

LIB=lib2HDME.a
LDFLAGS+=-L$(LIBDIR) -l2HDME -lgsl -lgslcblas -lm
LIBS=

DEMOS=DemoRGE DemoSM
PROG=

# Comment these line to not compile the GUI
# The gtkmm-3.0 library is required for the GUI.
# CFLAGS+=`pkg-config gtkmm-3.0 --cflags --libs` 
# LDFLAGS+=`pkg-config gtkmm-3.0 --cflags --libs`
# SOURCES+=GUI.cpp
# PROG+=ThdmcGui

# Comment to disable GNU-Plot
CFLAGS+=-DGNUPLOT
RGESRC+=GnuPlotSystem.cpp

# To use HiggsBounds/HiggsSignals for Higgs constraints, put the corresponding 
# libraries in the "lib" subdirectory and uncomment the following lines.
# Note that both programs: HiggsBounds (> 4.2.0) and HiggsSignals (> 1.4.0) 
# must be present
# CFLAGS+=-DHiggsBounds
# LDFLAGS+=-L$(LIBDIR) -lHS -lHB -lgfortran
# SOURCES+=HBHS.cpp

# To enable Minuit functionality
# CFLAGS+=-DMINUIT
# CFLAGS+=-I /home/jojelen/Cpp/libs/Minuit2-5.34.14/inc
# LDFLAGS+=-L/home/jojelen/Cpp/libs/Minuit2-5.34.14/src/.libs -lMinuit2 -fopenmp
# RGESRC+=THDM_fitter.cpp

# To enable SPheno functionality
# CFLAGS+=-DSPHENO
# RGESRC+=SPheno.cpp

.PHONY: directories lib clean distclean

all: directories lib $(DEMOS) $(PROG)

directories: $(OUT_DIR)

# Creating folders in OUT_DIR
$(OUT_DIR):
	@ $(MKDIR_P) $(OUT_DIR)

$(OBJDIR)/%.o : %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

lib: $(addprefix $(LIBDIR)/, $(LIB))

$(addprefix $(LIBDIR)/, $(LIB)): $(addprefix $(OBJDIR)/, $(OBJECTS))
	@ echo "Making library $@..."
	@ ar rcs $@ $(addprefix $(OBJDIR)/, $(OBJECTS))  

%: %.cpp $(addprefix $(LIBDIR)/, $(LIB))
	@ $(CC) $< $(CFLAGS) $(LDFLAGS) $(addprefix $(LIBDIR)/, $(LIBS)) \
	-o $(addprefix $(BINDIR)/, $@)

clean:
	@ echo "Cleaning library"
	@ rm -f $(addprefix $(OBJDIR)/, *.o)
	@ rm -f $(addprefix $(LIBDIR)/, $(LIB))

distclean:
	@ make -s clean
	@ echo "Cleaning programs"
	@ rm -f $(PROG)
