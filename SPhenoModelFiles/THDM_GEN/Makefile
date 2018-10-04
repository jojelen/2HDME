modname = THDM_GEN  # as an example  
PreDef = -DGENERATIONMIXING -DONLYDOUBLE
# setting various paths  
InDir = ../include
Mdir = ${InDir}
VPATH = 3-Body-Decays:LoopDecays:TwoLoopMasses:Observables:SM 
name = ../lib/libSPhenoTHDM_GEN.a
 
# check if SARAH module and SPheno are compatibel  
minV=330.00 
cVersion =1#$(shell expr $(version) \>= $(minV))
#  
# options for various compilers  
#  
# Default Compiler  
F90=gfortran
comp= -c -O -module ${Mdir} -I${InDir}  
LFlagsB= -O  
# Intels ifort,debug modus  
ifeq (${F90},ifortg)  
F90=ifort  
comp= -c -g -module ${Mdir} -I${InDir}  
LFlagsB= -g  
endif  
# gfortran  
ifeq (${F90},gfortran)  
comp= -c -g -ffree-line-length-none -J${Mdir} -I${InDir}  
LFlagsB= -g  
endif  
# g95  
ifeq (${F90},g95)  
comp= -c -O -fmod=${Mdir} -I${InDir}  
LFlagsB= -O  
endif  
# Lahey F95 compiler  
ifeq (${F90},lf95)  
comp=-c -O -M ${Mdir} -I${InDir}  
LFlagsB=-O  
endif  
# NAG f95/2003  
ifeq (${F90},nagfor)  
comp= -c -O -mdir ${Mdir} -I${InDir}  
LFlagsB= -O -DONLYDOUBLE -mdir ${MDir} -I${InDir}  
endif   
.SUFFIXES : .o .ps .f90 .F90 .a 
bin/SPhenoTHDM_GEN:
ifeq (${cVersion},1)
	 cd ../src ; ${MAKE} F90=${F90} 
	 ${MAKE} F90=${F90} ${name} 
	 ${MAKE} F90=${F90} SPhenoTHDM_GEN.o 
	 ${F90} -o SPhenoTHDM_GEN ${LFlagsB} SPhenoTHDM_GEN.o ../lib/libSPhenoTHDM_GEN.a ../lib/libSPheno.a
	 mv SPhenoTHDM_GEN ../bin
	 rm SPhenoTHDM_GEN.o  
${name}:  ${name}(Settings.o) ${name}(Model_Data_THDM_GEN.o)  \
 ${name}(RGEs_THDM_GEN.o)   \
 ${name}(Couplings_THDM_GEN.o) ${name}(TreeLevelMasses_THDM_GEN.o) ${name}(TadpoleEquations_THDM_GEN.o) \
 ${name}(LoopCouplings_THDM_GEN.o) ${name}(CouplingsForDecays_THDM_GEN.o) \
 ${name}(TreeLevel_Decays_THDM_GEN.o) \
 ${name}(Fu.o)  ${name}(Fe.o)  ${name}(Fd.o)  \
${name}(AddLoopFunctions.o) ${name}(Kinematics.o) \
 ${name}(2LPoleFunctions.o) ${name}(2LPole_THDM_GEN.o) \
 ${name}(LoopMasses_THDM_GEN.o) \
 ${name}(RGEs_SM_HC.o) ${name}(Couplings_SM_HC.o) ${name}(TreeLevelMasses_SM_HC.o) ${name}(LoopMasses_SM_HC.o)   \
${name}(CouplingsCT_THDM_GEN.o) \
${name}(Bremsstrahlung.o) ${name}(DecayFFS.o) ${name}(DecayFFV.o) ${name}(DecaySSS.o) ${name}(DecaySSV.o) ${name}(DecaySFF.o) ${name}(DecaySVV.o) \
 ${name}(LoopDecayFu_THDM_GEN.o)  ${name}(LoopDecayFe_THDM_GEN.o)  ${name}(LoopDecayFd_THDM_GEN.o)  ${name}(LoopDecayhh_THDM_GEN.o)  ${name}(LoopDecayAh_THDM_GEN.o)  ${name}(LoopDecayHm_THDM_GEN.o) \
 ${name}(Wrapper_LoopDecay_Fu_THDM_GEN.o)  ${name}(Wrapper_LoopDecay_Fe_THDM_GEN.o)  ${name}(Wrapper_LoopDecay_Fd_THDM_GEN.o)  ${name}(Wrapper_LoopDecay_hh_THDM_GEN.o)  ${name}(Wrapper_LoopDecay_Ah_THDM_GEN.o)  ${name}(Wrapper_LoopDecay_Hm_THDM_GEN.o) ${name}(OneLoopDecays_THDM_GEN.o) \
 ${name}(BranchingRatios_THDM_GEN.o) ${name}(HiggsCS_THDM_GEN.o) ${name}(RunSM_THDM_GEN.o) \
 ${name}(Unitarity_THDM_GEN.o) \
${name}(FlavorKit_LFV_THDM_GEN.o) ${name}(FlavorKit_QFV_THDM_GEN.o) ${name}(FlavorKit_Observables_THDM_GEN.o)\
 ${name}(LowEnergy_THDM_GEN.o) \
 ${name}(Boundaries_THDM_GEN.o)  ${name}(InputOutput_THDM_GEN.o) 
else 
	 @echo -------------------------------------------------------------------  
	 @echo ERROR:  
	 @echo The installed SPheno is version not compatibel with this module 
	 @echo Please, upgrade at least to SPheno version 3.3.0.  
	 @echo The current SPheno version can be downloaded from 
	 @echo http://www.hepforge.org/downloads/spheno 
	 @echo ------------------------------------------------------------------- 
endif 
clean: 
	 rm -f *.o *~ */*.o */*~
cleanall: 
	 rm -f bin/SPheno3 lib/*.a *~ */*.o */*~ include/*
#
# Suffix rules
#
.f90.a:
	 ${F90} ${comp} $< 
	 ar -ruc -U $@ $*.o
	 rm -f $*.o
.F90.a:
	 ${F90} ${comp} ${PreDef} $< 
	 ar -ruc -U $@ $*.o
	 rm -f $*.o
.f90.o:
	 ${F90} ${comp} $<
.f90.ps:
	 a2ps -o $*.ps $<
.h.ps:
	 a2ps -o $*.ps $<
