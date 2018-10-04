! -----------------------------------------------------------------------------  
! This file was automatically created by SARAH version 4.13.0 
! SARAH References: arXiv:0806.0538, 0909.2863, 1002.0840, 1207.0906, 1309.7223  
! (c) Florian Staub, 2013  
! ------------------------------------------------------------------------------  
! File created at 15:20 on 22.8.2018   
! ----------------------------------------------------------------------  
 
 
Module Unitarity_THDM_GEN 
 
Use Control 
Use Settings 
Use LoopFunctions 
Use Mathematics 
Use Model_Data_THDM_GEN 
Use RGEs_THDM_GEN 
Use LoopMasses_THDM_GEN 
Use TreeLevelMasses_THDM_GEN 
Use Couplings_THDM_GEN 
Use Tadpoles_THDM_GEN 
 Use StandardModel 
 
Logical :: IncludeGoldstoneContributions=.true. 
Logical :: IncludeGoldstoneExternal=.true. 
Logical :: AddR=.true. 
Real(dp) :: cut_elements = 0.0001_dp 
Real(dp) :: cut_amplitudes = 0.01_dp 
Logical :: Ignore_poles=.false. 
 
Contains 

Subroutine ScatteringEigenvalues(vdinput,vuinput,g1input,g2input,g3input,             & 
& Lam6input,Lam5input,Lam7input,Lam1input,Lam4input,Lam3input,Lam2input,eta1Uinput,      & 
& eta2Uinput,eta1Dinput,eta1Linput,eta2Dinput,eta2Linput,M12input,M112input,             & 
& M222input,delta0,kont)

Implicit None 
Integer, Intent(inout) :: kont 
Integer :: ierr 
Real(dp) :: g1,g2,g3

Complex(dp) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Real(dp) :: vd,vu

Real(dp) :: gD(133) 
Real(dp) :: tz,dt,q,q2,mudim 
Real(dp), Intent(in) :: delta0 
Integer :: iter 
Complex(dp) :: scatter_matrix(36,36) 
Complex(dp) :: rot_matrix(36,36) 
Real(dp) :: eigenvalues_matrix(36), test(2), unitarity_s, scattering_eigenvalue, step_size
Real(dp),Intent(in) :: g1input,g2input,g3input,vdinput,vuinput

Complex(dp),Intent(in) :: Lam6input,Lam5input,Lam7input,Lam1input,Lam4input,Lam3input,Lam2input,eta1Uinput(3,3),& 
& eta2Uinput(3,3),eta1Dinput(3,3),eta1Linput(3,3),eta2Dinput(3,3),eta2Linput(3,3),       & 
& M12input,M112input,M222input

max_scattering_eigenvalue = 0._dp 

g1 = g1input 
g2 = g2input 
g3 = g3input 
Lam6 = Lam6input 
Lam5 = Lam5input 
Lam7 = Lam7input 
Lam1 = Lam1input 
Lam4 = Lam4input 
Lam3 = Lam3input 
Lam2 = Lam2input 
eta1U = eta1Uinput 
eta2U = eta2Uinput 
eta1D = eta1Dinput 
eta1L = eta1Linput 
eta2D = eta2Dinput 
eta2L = eta2Linput 
M12 = M12input 
M112 = M112input 
M222 = M222input 
vd = vdinput 
vu = vuinput 
scatter_matrix=0._dp 
scatter_matrix(1,27) =-1._dp*Lam1
scatter_matrix(1,29) =-(sqrt(2._dp)*Conjg(Lam6))
scatter_matrix(1,34) =-1._dp*(Lam5)
scatter_matrix(2,28) =-1._dp*Lam1
scatter_matrix(2,30) =-Conjg(Lam6)
scatter_matrix(2,32) =-Conjg(Lam6)
scatter_matrix(2,35) =-1._dp*(Lam5)
scatter_matrix(3,27) =-(sqrt(2._dp)*Lam6)
scatter_matrix(3,29) =-1._dp*(Lam3) - Lam4
scatter_matrix(3,34) =-(sqrt(2._dp)*Conjg(Lam7))
scatter_matrix(4,28) =-1._dp*(Lam6)
scatter_matrix(4,30) =-1._dp*(Lam3)
scatter_matrix(4,32) =-1._dp*(Lam4)
scatter_matrix(4,35) =-Conjg(Lam7)
scatter_matrix(5,5) =-2._dp*Lam1
scatter_matrix(5,7) =-2*Conjg(Lam6)
scatter_matrix(5,13) =-1._dp*Lam1
scatter_matrix(5,15) =-Conjg(Lam6)
scatter_matrix(5,18) =-2._dp*(Lam6)
scatter_matrix(5,20) =-1._dp*(Lam3) - Lam4
scatter_matrix(5,24) =-1._dp*(Lam6)
scatter_matrix(5,26) =-1._dp*(Lam3)
scatter_matrix(6,12) =-1._dp*Lam1
scatter_matrix(6,14) =-Conjg(Lam6)
scatter_matrix(6,23) =-1._dp*(Lam6)
scatter_matrix(6,25) =-1._dp*(Lam4)
scatter_matrix(7,5) =-2*Conjg(Lam6)
scatter_matrix(7,7) =-2._dp*(Lam5)
scatter_matrix(7,13) =-Conjg(Lam6)
scatter_matrix(7,15) =-1._dp*(Lam5)
scatter_matrix(7,18) =-1._dp*(Lam3) - Lam4
scatter_matrix(7,20) =-2*Conjg(Lam7)
scatter_matrix(7,24) =-1._dp*(Lam4)
scatter_matrix(7,26) =-Conjg(Lam7)
scatter_matrix(8,12) =-Conjg(Lam6)
scatter_matrix(8,14) =-1._dp*(Lam5)
scatter_matrix(8,23) =-1._dp*(Lam3)
scatter_matrix(8,25) =-Conjg(Lam7)
scatter_matrix(9,31) =-1._dp*Lam1
scatter_matrix(9,33) =-(sqrt(2._dp)*Conjg(Lam6))
scatter_matrix(9,36) =-1._dp*(Lam5)
scatter_matrix(10,28) =-1._dp*(Lam6)
scatter_matrix(10,30) =-1._dp*(Lam4)
scatter_matrix(10,32) =-1._dp*(Lam3)
scatter_matrix(10,35) =-Conjg(Lam7)
scatter_matrix(11,31) =-(sqrt(2._dp)*Lam6)
scatter_matrix(11,33) =-1._dp*(Lam3) - Lam4
scatter_matrix(11,36) =-(sqrt(2._dp)*Conjg(Lam7))
scatter_matrix(12,6) =-1._dp*Lam1
scatter_matrix(12,8) =-Conjg(Lam6)
scatter_matrix(12,19) =-1._dp*(Lam6)
scatter_matrix(12,21) =-1._dp*(Lam4)
scatter_matrix(13,5) =-1._dp*Lam1
scatter_matrix(13,7) =-Conjg(Lam6)
scatter_matrix(13,13) =-2._dp*Lam1
scatter_matrix(13,15) =-2*Conjg(Lam6)
scatter_matrix(13,18) =-1._dp*(Lam6)
scatter_matrix(13,20) =-1._dp*(Lam3)
scatter_matrix(13,24) =-2._dp*(Lam6)
scatter_matrix(13,26) =-1._dp*(Lam3) - Lam4
scatter_matrix(14,6) =-Conjg(Lam6)
scatter_matrix(14,8) =-1._dp*(Lam5)
scatter_matrix(14,19) =-1._dp*(Lam3)
scatter_matrix(14,21) =-Conjg(Lam7)
scatter_matrix(15,5) =-Conjg(Lam6)
scatter_matrix(15,7) =-1._dp*(Lam5)
scatter_matrix(15,13) =-2*Conjg(Lam6)
scatter_matrix(15,15) =-2._dp*(Lam5)
scatter_matrix(15,18) =-1._dp*(Lam4)
scatter_matrix(15,20) =-Conjg(Lam7)
scatter_matrix(15,24) =-1._dp*(Lam3) - Lam4
scatter_matrix(15,26) =-2*Conjg(Lam7)
scatter_matrix(16,27) =-Conjg(Lam5)
scatter_matrix(16,29) =-(sqrt(2._dp)*Lam7)
scatter_matrix(16,34) =-1._dp*Lam2
scatter_matrix(17,28) =-Conjg(Lam5)
scatter_matrix(17,30) =-1._dp*(Lam7)
scatter_matrix(17,32) =-1._dp*(Lam7)
scatter_matrix(17,35) =-1._dp*Lam2
scatter_matrix(18,5) =-2._dp*(Lam6)
scatter_matrix(18,7) =-1._dp*(Lam3) - Lam4
scatter_matrix(18,13) =-1._dp*(Lam6)
scatter_matrix(18,15) =-1._dp*(Lam4)
scatter_matrix(18,18) =-2*Conjg(Lam5)
scatter_matrix(18,20) =-2._dp*(Lam7)
scatter_matrix(18,24) =-Conjg(Lam5)
scatter_matrix(18,26) =-1._dp*(Lam7)
scatter_matrix(19,12) =-1._dp*(Lam6)
scatter_matrix(19,14) =-1._dp*(Lam3)
scatter_matrix(19,23) =-Conjg(Lam5)
scatter_matrix(19,25) =-1._dp*(Lam7)
scatter_matrix(20,5) =-1._dp*(Lam3) - Lam4
scatter_matrix(20,7) =-2*Conjg(Lam7)
scatter_matrix(20,13) =-1._dp*(Lam3)
scatter_matrix(20,15) =-Conjg(Lam7)
scatter_matrix(20,18) =-2._dp*(Lam7)
scatter_matrix(20,20) =-2._dp*Lam2
scatter_matrix(20,24) =-1._dp*(Lam7)
scatter_matrix(20,26) =-1._dp*Lam2
scatter_matrix(21,12) =-1._dp*(Lam4)
scatter_matrix(21,14) =-Conjg(Lam7)
scatter_matrix(21,23) =-1._dp*(Lam7)
scatter_matrix(21,25) =-1._dp*Lam2
scatter_matrix(22,31) =-Conjg(Lam5)
scatter_matrix(22,33) =-(sqrt(2._dp)*Lam7)
scatter_matrix(22,36) =-1._dp*Lam2
scatter_matrix(23,6) =-1._dp*(Lam6)
scatter_matrix(23,8) =-1._dp*(Lam3)
scatter_matrix(23,19) =-Conjg(Lam5)
scatter_matrix(23,21) =-1._dp*(Lam7)
scatter_matrix(24,5) =-1._dp*(Lam6)
scatter_matrix(24,7) =-1._dp*(Lam4)
scatter_matrix(24,13) =-2._dp*(Lam6)
scatter_matrix(24,15) =-1._dp*(Lam3) - Lam4
scatter_matrix(24,18) =-Conjg(Lam5)
scatter_matrix(24,20) =-1._dp*(Lam7)
scatter_matrix(24,24) =-2*Conjg(Lam5)
scatter_matrix(24,26) =-2._dp*(Lam7)
scatter_matrix(25,6) =-1._dp*(Lam4)
scatter_matrix(25,8) =-Conjg(Lam7)
scatter_matrix(25,19) =-1._dp*(Lam7)
scatter_matrix(25,21) =-1._dp*Lam2
scatter_matrix(26,5) =-1._dp*(Lam3)
scatter_matrix(26,7) =-Conjg(Lam7)
scatter_matrix(26,13) =-1._dp*(Lam3) - Lam4
scatter_matrix(26,15) =-2*Conjg(Lam7)
scatter_matrix(26,18) =-1._dp*(Lam7)
scatter_matrix(26,20) =-1._dp*Lam2
scatter_matrix(26,24) =-2._dp*(Lam7)
scatter_matrix(26,26) =-2._dp*Lam2
scatter_matrix(27,1) =-1._dp*Lam1
scatter_matrix(27,3) =-(sqrt(2._dp)*Lam6)
scatter_matrix(27,16) =-Conjg(Lam5)
scatter_matrix(28,2) =-1._dp*Lam1
scatter_matrix(28,4) =-1._dp*(Lam6)
scatter_matrix(28,10) =-1._dp*(Lam6)
scatter_matrix(28,17) =-Conjg(Lam5)
scatter_matrix(29,1) =-(sqrt(2._dp)*Conjg(Lam6))
scatter_matrix(29,3) =-1._dp*(Lam3) - Lam4
scatter_matrix(29,16) =-(sqrt(2._dp)*Lam7)
scatter_matrix(30,2) =-Conjg(Lam6)
scatter_matrix(30,4) =-1._dp*(Lam3)
scatter_matrix(30,10) =-1._dp*(Lam4)
scatter_matrix(30,17) =-1._dp*(Lam7)
scatter_matrix(31,9) =-1._dp*Lam1
scatter_matrix(31,11) =-(sqrt(2._dp)*Lam6)
scatter_matrix(31,22) =-Conjg(Lam5)
scatter_matrix(32,2) =-Conjg(Lam6)
scatter_matrix(32,4) =-1._dp*(Lam4)
scatter_matrix(32,10) =-1._dp*(Lam3)
scatter_matrix(32,17) =-1._dp*(Lam7)
scatter_matrix(33,9) =-(sqrt(2._dp)*Conjg(Lam6))
scatter_matrix(33,11) =-1._dp*(Lam3) - Lam4
scatter_matrix(33,22) =-(sqrt(2._dp)*Lam7)
scatter_matrix(34,1) =-1._dp*(Lam5)
scatter_matrix(34,3) =-(sqrt(2._dp)*Conjg(Lam7))
scatter_matrix(34,16) =-1._dp*Lam2
scatter_matrix(35,2) =-1._dp*(Lam5)
scatter_matrix(35,4) =-Conjg(Lam7)
scatter_matrix(35,10) =-Conjg(Lam7)
scatter_matrix(35,17) =-1._dp*Lam2
scatter_matrix(36,9) =-1._dp*(Lam5)
scatter_matrix(36,11) =-(sqrt(2._dp)*Conjg(Lam7))
scatter_matrix(36,22) =-1._dp*Lam2
Call EigenSystem( oo16pi*scatter_matrix,eigenvalues_matrix,rot_matrix,ierr,test) 

scattering_eigenvalue=MaxVal(Abs(eigenvalues_matrix)) 
max_scattering_eigenvalue=scattering_eigenvalue 
If (max_scattering_eigenvalue.gt.0.5_dp) TreeUnitarity=0._dp 
End Subroutine ScatteringEigenvalues

Subroutine ScatteringEigenvaluesWithTrilinears(vdinput,vuinput,g1input,               & 
& g2input,g3input,Lam6input,Lam5input,Lam7input,Lam1input,Lam4input,Lam3input,           & 
& Lam2input,eta1Uinput,eta2Uinput,eta1Dinput,eta1Linput,eta2Dinput,eta2Linput,           & 
& M12input,M112input,M222input,delta0,kont)

Implicit None 
Integer, Intent(inout) :: kont 
Integer :: ierr 
Logical :: Pole_Present, SPole_Present, TPole_Present, UPole_Present 
Integer :: RemoveTUpoles(99) 
Real(dp) :: g1,g2,g3

Complex(dp) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Real(dp) :: vd,vu

Complex(dp) :: cplAhAhAh(2,2,2),cplAhAhhh(2,2,2),cplAhhhhh(2,2,2),cplAhHmcHm(2,2,2),cplhhhhhh(2,2,2),& 
& cplhhHmcHm(2,2,2),cplAhAhAhAh(2,2,2,2),cplAhAhAhhh(2,2,2,2),cplAhAhhhhh(2,2,2,2),      & 
& cplAhAhHmcHm(2,2,2,2),cplAhhhhhhh(2,2,2,2),cplAhhhHmcHm(2,2,2,2),cplhhhhhhhh(2,2,2,2), & 
& cplhhhhHmcHm(2,2,2,2),cplHmHmcHmcHm(2,2,2,2),cplAhhhVZ(2,2),cplAhHmcVWm(2,2),          & 
& cplAhcHmVWm(2,2),cplhhHmcVWm(2,2),cplhhcHmVWm(2,2),cplHmcHmVP(2,2),cplHmcHmVZ(2,2),    & 
& cplhhcVWmVWm(2),cplhhVZVZ(2),cplHmcVWmVP(2),cplHmcVWmVZ(2),cplcHmVPVWm(2),             & 
& cplcHmVWmVZ(2),cplAhAhcVWmVWm(2,2),cplAhAhVZVZ(2,2),cplAhHmcVWmVP(2,2),cplAhHmcVWmVZ(2,2),& 
& cplAhcHmVPVWm(2,2),cplAhcHmVWmVZ(2,2),cplhhhhcVWmVWm(2,2),cplhhhhVZVZ(2,2),            & 
& cplhhHmcVWmVP(2,2),cplhhHmcVWmVZ(2,2),cplhhcHmVPVWm(2,2),cplhhcHmVWmVZ(2,2),           & 
& cplHmcHmVPVP(2,2),cplHmcHmVPVZ(2,2),cplHmcHmcVWmVWm(2,2),cplHmcHmVZVZ(2,2),            & 
& cplVGVGVG,cplcVWmVPVWm,cplcVWmVWmVZ,cplcFdFdAhL(3,3,2),cplcFdFdAhR(3,3,2),             & 
& cplcFeFeAhL(3,3,2),cplcFeFeAhR(3,3,2),cplcFuFuAhL(3,3,2),cplcFuFuAhR(3,3,2),           & 
& cplcFdFdhhL(3,3,2),cplcFdFdhhR(3,3,2),cplcFuFdcHmL(3,3,2),cplcFuFdcHmR(3,3,2),         & 
& cplcFeFehhL(3,3,2),cplcFeFehhR(3,3,2),cplcFvFecHmL(3,3,2),cplcFvFecHmR(3,3,2),         & 
& cplcFuFuhhL(3,3,2),cplcFuFuhhR(3,3,2),cplcFdFuHmL(3,3,2),cplcFdFuHmR(3,3,2),           & 
& cplcFeFvHmL(3,3,2),cplcFeFvHmR(3,3,2),cplcFdFdVGL(3,3),cplcFdFdVGR(3,3),               & 
& cplcFdFdVPL(3,3),cplcFdFdVPR(3,3),cplcFdFdVZL(3,3),cplcFdFdVZR(3,3),cplcFuFdcVWmL(3,3),& 
& cplcFuFdcVWmR(3,3),cplcFeFeVPL(3,3),cplcFeFeVPR(3,3),cplcFeFeVZL(3,3),cplcFeFeVZR(3,3),& 
& cplcFvFecVWmL(3,3),cplcFvFecVWmR(3,3),cplcFuFuVGL(3,3),cplcFuFuVGR(3,3),               & 
& cplcFuFuVPL(3,3),cplcFuFuVPR(3,3),cplcFdFuVWmL(3,3),cplcFdFuVWmR(3,3),cplcFuFuVZL(3,3),& 
& cplcFuFuVZR(3,3),cplcFeFvVWmL(3,3),cplcFeFvVWmR(3,3),cplcFvFvVZL(3,3),cplcFvFvVZR(3,3),& 
& cplVGVGVGVG1,cplVGVGVGVG2,cplVGVGVGVG3,cplcVWmVPVPVWm1,cplcVWmVPVPVWm2,cplcVWmVPVPVWm3,& 
& cplcVWmVPVWmVZ1,cplcVWmVPVWmVZ2,cplcVWmVPVWmVZ3,cplcVWmcVWmVWmVWm1,cplcVWmcVWmVWmVWm2, & 
& cplcVWmcVWmVWmVWm3,cplcVWmVWmVZVZ1,cplcVWmVWmVZVZ2,cplcVWmVWmVZVZ3,cplcgGgGVG,         & 
& cplcgWmgAVWm,cplcgWpCgAcVWm,cplcgWmgWmVP,cplcgWmgWmVZ,cplcgAgWmcVWm,cplcgZgWmcVWm,     & 
& cplcgWpCgWpCVP,cplcgAgWpCVWm,cplcgZgWpCVWm,cplcgWpCgWpCVZ,cplcgWmgZVWm,cplcgWpCgZcVWm, & 
& cplcgWmgWmAh(2),cplcgWpCgWpCAh(2),cplcgZgAhh(2),cplcgWmgAHm(2),cplcgWpCgAcHm(2),       & 
& cplcgWmgWmhh(2),cplcgZgWmcHm(2),cplcgWpCgWpChh(2),cplcgZgWpCHm(2),cplcgZgZhh(2),       & 
& cplcgWmgZHm(2),cplcgWpCgZcHm(2)

Real(dp) :: MAh(2),MAh2(2),MFd(3),MFd2(3),MFe(3),MFe2(3),MFu(3),MFu2(3),Mhh(2),Mhh2(2),           & 
& MHm(2),MHm2(2),MVWm,MVWm2,MVZ,MVZ2,TW,v,ZA(2,2),ZH(2,2),ZP(2,2),ZZ(2,2),               & 
& alphaH,betaH

Complex(dp) :: ZDR(3,3),ZER(3,3),ZUR(3,3),ZDL(3,3),ZEL(3,3),ZUL(3,3),ZW(2,2)

Real(dp),Intent(in) :: g1input,g2input,g3input,vdinput,vuinput

Complex(dp),Intent(in) :: Lam6input,Lam5input,Lam7input,Lam1input,Lam4input,Lam3input,Lam2input,eta1Uinput(3,3),& 
& eta2Uinput(3,3),eta1Dinput(3,3),eta1Linput(3,3),eta2Dinput(3,3),eta2Linput(3,3),       & 
& M12input,M112input,M222input

Complex(dp) :: scatter_matrix1(14,14) 
Complex(dp) :: scatter_matrix1B(14,14) 
Complex(dp) :: rot_matrix1(14,14) 
Real(dp) :: eigenvalues_matrix1(14)
Complex(dp) :: scatter_matrix2(8,8) 
Complex(dp) :: scatter_matrix2B(8,8) 
Complex(dp) :: rot_matrix2(8,8) 
Real(dp) :: eigenvalues_matrix2(8)
Complex(dp) :: scatter_matrix3(3,3) 
Complex(dp) :: scatter_matrix3B(3,3) 
Complex(dp) :: rot_matrix3(3,3) 
Real(dp) :: eigenvalues_matrix3(3)
Real(dp) :: step_size,scattering_eigenvalue_trilinears, unitarity_s, test(2) 
Real(dp) :: gD(133) 
Real(dp) :: tz,dt,q,q2,mudim, max_element_removed  
Real(dp), Intent(in) :: delta0 
Integer :: iter, i, count 
g1 = g1input 
g2 = g2input 
g3 = g3input 
Lam6 = Lam6input 
Lam5 = Lam5input 
Lam7 = Lam7input 
Lam1 = Lam1input 
Lam4 = Lam4input 
Lam3 = Lam3input 
Lam2 = Lam2input 
eta1U = eta1Uinput 
eta2U = eta2Uinput 
eta1D = eta1Dinput 
eta1L = eta1Linput 
eta2D = eta2Dinput 
eta2L = eta2Linput 
M12 = M12input 
M112 = M112input 
M222 = M222input 
vd = vdinput 
vu = vuinput 
Call TreeMasses(MAh,MAh2,MFd,MFd2,MFe,MFe2,MFu,MFu2,Mhh,Mhh2,MHm,MHm2,MVWm,           & 
& MVWm2,MVZ,MVZ2,TW,ZDR,ZER,ZUR,v,ZDL,ZEL,ZUL,ZA,ZH,ZP,ZW,ZZ,alphaH,betaH,               & 
& vd,vu,g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U,eta2U,eta1D,eta1L,             & 
& eta2D,eta2L,M12,M112,M222,.True.,kont)

Call AllCouplingsReallyAll(Lam6,Lam5,Lam7,Lam1,Lam2,vd,vu,ZA,Lam4,Lam3,               & 
& ZH,ZP,g1,g2,TW,g3,eta1D,eta2D,ZDL,ZDR,eta1L,eta2L,ZEL,ZER,eta1U,eta2U,ZUL,             & 
& ZUR,cplAhAhAh,cplAhAhhh,cplAhhhhh,cplAhHmcHm,cplhhhhhh,cplhhHmcHm,cplAhAhAhAh,         & 
& cplAhAhAhhh,cplAhAhhhhh,cplAhAhHmcHm,cplAhhhhhhh,cplAhhhHmcHm,cplhhhhhhhh,             & 
& cplhhhhHmcHm,cplHmHmcHmcHm,cplAhhhVZ,cplAhHmcVWm,cplAhcHmVWm,cplhhHmcVWm,              & 
& cplhhcHmVWm,cplHmcHmVP,cplHmcHmVZ,cplhhcVWmVWm,cplhhVZVZ,cplHmcVWmVP,cplHmcVWmVZ,      & 
& cplcHmVPVWm,cplcHmVWmVZ,cplAhAhcVWmVWm,cplAhAhVZVZ,cplAhHmcVWmVP,cplAhHmcVWmVZ,        & 
& cplAhcHmVPVWm,cplAhcHmVWmVZ,cplhhhhcVWmVWm,cplhhhhVZVZ,cplhhHmcVWmVP,cplhhHmcVWmVZ,    & 
& cplhhcHmVPVWm,cplhhcHmVWmVZ,cplHmcHmVPVP,cplHmcHmVPVZ,cplHmcHmcVWmVWm,cplHmcHmVZVZ,    & 
& cplVGVGVG,cplcVWmVPVWm,cplcVWmVWmVZ,cplcFdFdAhL,cplcFdFdAhR,cplcFeFeAhL,               & 
& cplcFeFeAhR,cplcFuFuAhL,cplcFuFuAhR,cplcFdFdhhL,cplcFdFdhhR,cplcFuFdcHmL,              & 
& cplcFuFdcHmR,cplcFeFehhL,cplcFeFehhR,cplcFvFecHmL,cplcFvFecHmR,cplcFuFuhhL,            & 
& cplcFuFuhhR,cplcFdFuHmL,cplcFdFuHmR,cplcFeFvHmL,cplcFeFvHmR,cplcFdFdVGL,               & 
& cplcFdFdVGR,cplcFdFdVPL,cplcFdFdVPR,cplcFdFdVZL,cplcFdFdVZR,cplcFuFdcVWmL,             & 
& cplcFuFdcVWmR,cplcFeFeVPL,cplcFeFeVPR,cplcFeFeVZL,cplcFeFeVZR,cplcFvFecVWmL,           & 
& cplcFvFecVWmR,cplcFuFuVGL,cplcFuFuVGR,cplcFuFuVPL,cplcFuFuVPR,cplcFdFuVWmL,            & 
& cplcFdFuVWmR,cplcFuFuVZL,cplcFuFuVZR,cplcFeFvVWmL,cplcFeFvVWmR,cplcFvFvVZL,            & 
& cplcFvFvVZR,cplVGVGVGVG1,cplVGVGVGVG2,cplVGVGVGVG3,cplcVWmVPVPVWm1,cplcVWmVPVPVWm2,    & 
& cplcVWmVPVPVWm3,cplcVWmVPVWmVZ1,cplcVWmVPVWmVZ2,cplcVWmVPVWmVZ3,cplcVWmcVWmVWmVWm1,    & 
& cplcVWmcVWmVWmVWm2,cplcVWmcVWmVWmVWm3,cplcVWmVWmVZVZ1,cplcVWmVWmVZVZ2,cplcVWmVWmVZVZ3, & 
& cplcgGgGVG,cplcgWmgAVWm,cplcgWpCgAcVWm,cplcgWmgWmVP,cplcgWmgWmVZ,cplcgAgWmcVWm,        & 
& cplcgZgWmcVWm,cplcgWpCgWpCVP,cplcgAgWpCVWm,cplcgZgWpCVWm,cplcgWpCgWpCVZ,               & 
& cplcgWmgZVWm,cplcgWpCgZcVWm,cplcgWmgWmAh,cplcgWpCgWpCAh,cplcgZgAhh,cplcgWmgAHm,        & 
& cplcgWpCgAcHm,cplcgWmgWmhh,cplcgZgWmcHm,cplcgWpCgWpChh,cplcgZgWpCHm,cplcgZgZhh,        & 
& cplcgWmgZHm,cplcgWpCgZcHm)

max_scattering_eigenvalue_trilinears = 0._dp 
If (unitarity_steps.eq.1) Then  
  step_size = 0._dp
Else  
 If (unitarity_steps.gt.0) Then 
  step_size = ((Abs(unitarity_s_max)) -(abs(unitarity_s_min)))/(1._dp*(Abs(unitarity_steps)-1)) 
 Else 
  step_size = (log(Abs(unitarity_s_max)) -log(abs(unitarity_s_min)))/(1._dp*(Abs(unitarity_steps)-1)) 
 End if 
End if 
Do iter=0,Abs(unitarity_steps)-1 
If (unitarity_steps.lt.0) Then 
  unitarity_s=exp(log(unitarity_s_min) + iter*step_size)**2 
Else 
  unitarity_s=(unitarity_s_min + iter*step_size)**2 
End if 
!! 1. sub-matrix  
Pole_Present = .false. 
max_element_removed = 0._dp 
RemoveTUpoles = 0 
scatter_matrix1=0._dp 
If (IncludeGoldstoneExternal) scatter_matrix1(1,1) = a0_AhAh_AhAh(unitarity_s,1,1,1,1,1,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,2) = a0_AhAh_AhAh(unitarity_s,1,1,1,2,1,2) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,3) = a0_AhAh_AhAh(unitarity_s,1,1,2,2,1,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,4) = a0_AhAh_Ahhh(unitarity_s,1,1,1,1,1,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,5) = a0_AhAh_Ahhh(unitarity_s,1,1,1,2,1,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,6) = a0_AhAh_Ahhh(unitarity_s,1,1,2,1,1,6) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,7) = a0_AhAh_Ahhh(unitarity_s,1,1,2,2,1,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,8) = a0_AhAh_hhhh(unitarity_s,1,1,1,1,1,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,9) = a0_AhAh_hhhh(unitarity_s,1,1,1,2,1,9) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,10) = a0_AhAh_hhhh(unitarity_s,1,1,2,2,1,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,11) = a0_AhAh_HmHmc(unitarity_s,1,1,1,1,1,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,12) = a0_AhAh_HmHmc(unitarity_s,1,1,1,2,1,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,13) = a0_AhAh_HmHmc(unitarity_s,1,1,2,1,1,13) 
If (IncludeGoldstoneExternal) scatter_matrix1(1,14) = a0_AhAh_HmHmc(unitarity_s,1,1,2,2,1,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,1) = a0_AhAh_AhAh(unitarity_s,1,2,1,1,2,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,2) = a0_AhAh_AhAh(unitarity_s,1,2,1,2,2,2) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,3) = a0_AhAh_AhAh(unitarity_s,1,2,2,2,2,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,4) = a0_AhAh_Ahhh(unitarity_s,1,2,1,1,2,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,5) = a0_AhAh_Ahhh(unitarity_s,1,2,1,2,2,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,6) = a0_AhAh_Ahhh(unitarity_s,1,2,2,1,2,6) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,7) = a0_AhAh_Ahhh(unitarity_s,1,2,2,2,2,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,8) = a0_AhAh_hhhh(unitarity_s,1,2,1,1,2,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,9) = a0_AhAh_hhhh(unitarity_s,1,2,1,2,2,9) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,10) = a0_AhAh_hhhh(unitarity_s,1,2,2,2,2,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,11) = a0_AhAh_HmHmc(unitarity_s,1,2,1,1,2,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,12) = a0_AhAh_HmHmc(unitarity_s,1,2,1,2,2,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,13) = a0_AhAh_HmHmc(unitarity_s,1,2,2,1,2,13) 
If (IncludeGoldstoneExternal) scatter_matrix1(2,14) = a0_AhAh_HmHmc(unitarity_s,1,2,2,2,2,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,1) = a0_AhAh_AhAh(unitarity_s,2,2,1,1,3,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,2) = a0_AhAh_AhAh(unitarity_s,2,2,1,2,3,2) 
scatter_matrix1(3,3) = a0_AhAh_AhAh(unitarity_s,2,2,2,2,3,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,4) = a0_AhAh_Ahhh(unitarity_s,2,2,1,1,3,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,5) = a0_AhAh_Ahhh(unitarity_s,2,2,1,2,3,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,6) = a0_AhAh_Ahhh(unitarity_s,2,2,2,1,3,6) 
scatter_matrix1(3,7) = a0_AhAh_Ahhh(unitarity_s,2,2,2,2,3,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,8) = a0_AhAh_hhhh(unitarity_s,2,2,1,1,3,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,9) = a0_AhAh_hhhh(unitarity_s,2,2,1,2,3,9) 
scatter_matrix1(3,10) = a0_AhAh_hhhh(unitarity_s,2,2,2,2,3,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,11) = a0_AhAh_HmHmc(unitarity_s,2,2,1,1,3,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,12) = a0_AhAh_HmHmc(unitarity_s,2,2,1,2,3,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(3,13) = a0_AhAh_HmHmc(unitarity_s,2,2,2,1,3,13) 
scatter_matrix1(3,14) = a0_AhAh_HmHmc(unitarity_s,2,2,2,2,3,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,1) = a0_Ahhh_AhAh(unitarity_s,1,1,1,1,4,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,2) = a0_Ahhh_AhAh(unitarity_s,1,1,1,2,4,2) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,3) = a0_Ahhh_AhAh(unitarity_s,1,1,2,2,4,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,4) = a0_Ahhh_Ahhh(unitarity_s,1,1,1,1,4,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,5) = a0_Ahhh_Ahhh(unitarity_s,1,1,1,2,4,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,6) = a0_Ahhh_Ahhh(unitarity_s,1,1,2,1,4,6) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,7) = a0_Ahhh_Ahhh(unitarity_s,1,1,2,2,4,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,8) = a0_Ahhh_hhhh(unitarity_s,1,1,1,1,4,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,9) = a0_Ahhh_hhhh(unitarity_s,1,1,1,2,4,9) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,10) = a0_Ahhh_hhhh(unitarity_s,1,1,2,2,4,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,11) = a0_Ahhh_HmHmc(unitarity_s,1,1,1,1,4,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,12) = a0_Ahhh_HmHmc(unitarity_s,1,1,1,2,4,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,13) = a0_Ahhh_HmHmc(unitarity_s,1,1,2,1,4,13) 
If (IncludeGoldstoneExternal) scatter_matrix1(4,14) = a0_Ahhh_HmHmc(unitarity_s,1,1,2,2,4,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,1) = a0_Ahhh_AhAh(unitarity_s,1,2,1,1,5,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,2) = a0_Ahhh_AhAh(unitarity_s,1,2,1,2,5,2) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,3) = a0_Ahhh_AhAh(unitarity_s,1,2,2,2,5,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,4) = a0_Ahhh_Ahhh(unitarity_s,1,2,1,1,5,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,5) = a0_Ahhh_Ahhh(unitarity_s,1,2,1,2,5,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,6) = a0_Ahhh_Ahhh(unitarity_s,1,2,2,1,5,6) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,7) = a0_Ahhh_Ahhh(unitarity_s,1,2,2,2,5,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,8) = a0_Ahhh_hhhh(unitarity_s,1,2,1,1,5,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,9) = a0_Ahhh_hhhh(unitarity_s,1,2,1,2,5,9) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,10) = a0_Ahhh_hhhh(unitarity_s,1,2,2,2,5,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,11) = a0_Ahhh_HmHmc(unitarity_s,1,2,1,1,5,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,12) = a0_Ahhh_HmHmc(unitarity_s,1,2,1,2,5,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,13) = a0_Ahhh_HmHmc(unitarity_s,1,2,2,1,5,13) 
If (IncludeGoldstoneExternal) scatter_matrix1(5,14) = a0_Ahhh_HmHmc(unitarity_s,1,2,2,2,5,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,1) = a0_Ahhh_AhAh(unitarity_s,2,1,1,1,6,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,2) = a0_Ahhh_AhAh(unitarity_s,2,1,1,2,6,2) 
scatter_matrix1(6,3) = a0_Ahhh_AhAh(unitarity_s,2,1,2,2,6,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,4) = a0_Ahhh_Ahhh(unitarity_s,2,1,1,1,6,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,5) = a0_Ahhh_Ahhh(unitarity_s,2,1,1,2,6,5) 
scatter_matrix1(6,6) = a0_Ahhh_Ahhh(unitarity_s,2,1,2,1,6,6) 
scatter_matrix1(6,7) = a0_Ahhh_Ahhh(unitarity_s,2,1,2,2,6,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,8) = a0_Ahhh_hhhh(unitarity_s,2,1,1,1,6,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,9) = a0_Ahhh_hhhh(unitarity_s,2,1,1,2,6,9) 
scatter_matrix1(6,10) = a0_Ahhh_hhhh(unitarity_s,2,1,2,2,6,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,11) = a0_Ahhh_HmHmc(unitarity_s,2,1,1,1,6,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(6,12) = a0_Ahhh_HmHmc(unitarity_s,2,1,1,2,6,12) 
scatter_matrix1(6,13) = a0_Ahhh_HmHmc(unitarity_s,2,1,2,1,6,13) 
scatter_matrix1(6,14) = a0_Ahhh_HmHmc(unitarity_s,2,1,2,2,6,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,1) = a0_Ahhh_AhAh(unitarity_s,2,2,1,1,7,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,2) = a0_Ahhh_AhAh(unitarity_s,2,2,1,2,7,2) 
scatter_matrix1(7,3) = a0_Ahhh_AhAh(unitarity_s,2,2,2,2,7,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,4) = a0_Ahhh_Ahhh(unitarity_s,2,2,1,1,7,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,5) = a0_Ahhh_Ahhh(unitarity_s,2,2,1,2,7,5) 
scatter_matrix1(7,6) = a0_Ahhh_Ahhh(unitarity_s,2,2,2,1,7,6) 
scatter_matrix1(7,7) = a0_Ahhh_Ahhh(unitarity_s,2,2,2,2,7,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,8) = a0_Ahhh_hhhh(unitarity_s,2,2,1,1,7,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,9) = a0_Ahhh_hhhh(unitarity_s,2,2,1,2,7,9) 
scatter_matrix1(7,10) = a0_Ahhh_hhhh(unitarity_s,2,2,2,2,7,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,11) = a0_Ahhh_HmHmc(unitarity_s,2,2,1,1,7,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(7,12) = a0_Ahhh_HmHmc(unitarity_s,2,2,1,2,7,12) 
scatter_matrix1(7,13) = a0_Ahhh_HmHmc(unitarity_s,2,2,2,1,7,13) 
scatter_matrix1(7,14) = a0_Ahhh_HmHmc(unitarity_s,2,2,2,2,7,14) 
scatter_matrix1(8,1) = a0_hhhh_AhAh(unitarity_s,1,1,1,1,8,1) 
scatter_matrix1(8,2) = a0_hhhh_AhAh(unitarity_s,1,1,1,2,8,2) 
scatter_matrix1(8,3) = a0_hhhh_AhAh(unitarity_s,1,1,2,2,8,3) 
scatter_matrix1(8,4) = a0_hhhh_Ahhh(unitarity_s,1,1,1,1,8,4) 
scatter_matrix1(8,5) = a0_hhhh_Ahhh(unitarity_s,1,1,1,2,8,5) 
scatter_matrix1(8,6) = a0_hhhh_Ahhh(unitarity_s,1,1,2,1,8,6) 
scatter_matrix1(8,7) = a0_hhhh_Ahhh(unitarity_s,1,1,2,2,8,7) 
scatter_matrix1(8,8) = a0_hhhh_hhhh(unitarity_s,1,1,1,1,8,8) 
scatter_matrix1(8,9) = a0_hhhh_hhhh(unitarity_s,1,1,1,2,8,9) 
scatter_matrix1(8,10) = a0_hhhh_hhhh(unitarity_s,1,1,2,2,8,10) 
scatter_matrix1(8,11) = a0_hhhh_HmHmc(unitarity_s,1,1,1,1,8,11) 
scatter_matrix1(8,12) = a0_hhhh_HmHmc(unitarity_s,1,1,1,2,8,12) 
scatter_matrix1(8,13) = a0_hhhh_HmHmc(unitarity_s,1,1,2,1,8,13) 
scatter_matrix1(8,14) = a0_hhhh_HmHmc(unitarity_s,1,1,2,2,8,14) 
scatter_matrix1(9,1) = a0_hhhh_AhAh(unitarity_s,1,2,1,1,9,1) 
scatter_matrix1(9,2) = a0_hhhh_AhAh(unitarity_s,1,2,1,2,9,2) 
scatter_matrix1(9,3) = a0_hhhh_AhAh(unitarity_s,1,2,2,2,9,3) 
scatter_matrix1(9,4) = a0_hhhh_Ahhh(unitarity_s,1,2,1,1,9,4) 
scatter_matrix1(9,5) = a0_hhhh_Ahhh(unitarity_s,1,2,1,2,9,5) 
scatter_matrix1(9,6) = a0_hhhh_Ahhh(unitarity_s,1,2,2,1,9,6) 
scatter_matrix1(9,7) = a0_hhhh_Ahhh(unitarity_s,1,2,2,2,9,7) 
scatter_matrix1(9,8) = a0_hhhh_hhhh(unitarity_s,1,2,1,1,9,8) 
scatter_matrix1(9,9) = a0_hhhh_hhhh(unitarity_s,1,2,1,2,9,9) 
scatter_matrix1(9,10) = a0_hhhh_hhhh(unitarity_s,1,2,2,2,9,10) 
scatter_matrix1(9,11) = a0_hhhh_HmHmc(unitarity_s,1,2,1,1,9,11) 
scatter_matrix1(9,12) = a0_hhhh_HmHmc(unitarity_s,1,2,1,2,9,12) 
scatter_matrix1(9,13) = a0_hhhh_HmHmc(unitarity_s,1,2,2,1,9,13) 
scatter_matrix1(9,14) = a0_hhhh_HmHmc(unitarity_s,1,2,2,2,9,14) 
scatter_matrix1(10,1) = a0_hhhh_AhAh(unitarity_s,2,2,1,1,10,1) 
scatter_matrix1(10,2) = a0_hhhh_AhAh(unitarity_s,2,2,1,2,10,2) 
scatter_matrix1(10,3) = a0_hhhh_AhAh(unitarity_s,2,2,2,2,10,3) 
scatter_matrix1(10,4) = a0_hhhh_Ahhh(unitarity_s,2,2,1,1,10,4) 
scatter_matrix1(10,5) = a0_hhhh_Ahhh(unitarity_s,2,2,1,2,10,5) 
scatter_matrix1(10,6) = a0_hhhh_Ahhh(unitarity_s,2,2,2,1,10,6) 
scatter_matrix1(10,7) = a0_hhhh_Ahhh(unitarity_s,2,2,2,2,10,7) 
scatter_matrix1(10,8) = a0_hhhh_hhhh(unitarity_s,2,2,1,1,10,8) 
scatter_matrix1(10,9) = a0_hhhh_hhhh(unitarity_s,2,2,1,2,10,9) 
scatter_matrix1(10,10) = a0_hhhh_hhhh(unitarity_s,2,2,2,2,10,10) 
scatter_matrix1(10,11) = a0_hhhh_HmHmc(unitarity_s,2,2,1,1,10,11) 
scatter_matrix1(10,12) = a0_hhhh_HmHmc(unitarity_s,2,2,1,2,10,12) 
scatter_matrix1(10,13) = a0_hhhh_HmHmc(unitarity_s,2,2,2,1,10,13) 
scatter_matrix1(10,14) = a0_hhhh_HmHmc(unitarity_s,2,2,2,2,10,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,1) = a0_HmHmc_AhAh(unitarity_s,1,1,1,1,11,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,2) = a0_HmHmc_AhAh(unitarity_s,1,1,1,2,11,2) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,3) = a0_HmHmc_AhAh(unitarity_s,1,1,2,2,11,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,4) = a0_HmHmc_Ahhh(unitarity_s,1,1,1,1,11,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,5) = a0_HmHmc_Ahhh(unitarity_s,1,1,1,2,11,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,6) = a0_HmHmc_Ahhh(unitarity_s,1,1,2,1,11,6) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,7) = a0_HmHmc_Ahhh(unitarity_s,1,1,2,2,11,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,8) = a0_HmHmc_hhhh(unitarity_s,1,1,1,1,11,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,9) = a0_HmHmc_hhhh(unitarity_s,1,1,1,2,11,9) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,10) = a0_HmHmc_hhhh(unitarity_s,1,1,2,2,11,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,11) = a0_HmHmc_HmHmc(unitarity_s,1,1,1,1,11,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,12) = a0_HmHmc_HmHmc(unitarity_s,1,1,1,2,11,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,13) = a0_HmHmc_HmHmc(unitarity_s,1,1,2,1,11,13) 
If (IncludeGoldstoneExternal) scatter_matrix1(11,14) = a0_HmHmc_HmHmc(unitarity_s,1,1,2,2,11,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,1) = a0_HmHmc_AhAh(unitarity_s,1,2,1,1,12,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,2) = a0_HmHmc_AhAh(unitarity_s,1,2,1,2,12,2) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,3) = a0_HmHmc_AhAh(unitarity_s,1,2,2,2,12,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,4) = a0_HmHmc_Ahhh(unitarity_s,1,2,1,1,12,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,5) = a0_HmHmc_Ahhh(unitarity_s,1,2,1,2,12,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,6) = a0_HmHmc_Ahhh(unitarity_s,1,2,2,1,12,6) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,7) = a0_HmHmc_Ahhh(unitarity_s,1,2,2,2,12,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,8) = a0_HmHmc_hhhh(unitarity_s,1,2,1,1,12,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,9) = a0_HmHmc_hhhh(unitarity_s,1,2,1,2,12,9) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,10) = a0_HmHmc_hhhh(unitarity_s,1,2,2,2,12,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,11) = a0_HmHmc_HmHmc(unitarity_s,1,2,1,1,12,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,12) = a0_HmHmc_HmHmc(unitarity_s,1,2,1,2,12,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,13) = a0_HmHmc_HmHmc(unitarity_s,1,2,2,1,12,13) 
If (IncludeGoldstoneExternal) scatter_matrix1(12,14) = a0_HmHmc_HmHmc(unitarity_s,1,2,2,2,12,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,1) = a0_HmHmc_AhAh(unitarity_s,2,1,1,1,13,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,2) = a0_HmHmc_AhAh(unitarity_s,2,1,1,2,13,2) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,3) = a0_HmHmc_AhAh(unitarity_s,2,1,2,2,13,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,4) = a0_HmHmc_Ahhh(unitarity_s,2,1,1,1,13,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,5) = a0_HmHmc_Ahhh(unitarity_s,2,1,1,2,13,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,6) = a0_HmHmc_Ahhh(unitarity_s,2,1,2,1,13,6) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,7) = a0_HmHmc_Ahhh(unitarity_s,2,1,2,2,13,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,8) = a0_HmHmc_hhhh(unitarity_s,2,1,1,1,13,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,9) = a0_HmHmc_hhhh(unitarity_s,2,1,1,2,13,9) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,10) = a0_HmHmc_hhhh(unitarity_s,2,1,2,2,13,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,11) = a0_HmHmc_HmHmc(unitarity_s,2,1,1,1,13,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,12) = a0_HmHmc_HmHmc(unitarity_s,2,1,1,2,13,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,13) = a0_HmHmc_HmHmc(unitarity_s,2,1,2,1,13,13) 
If (IncludeGoldstoneExternal) scatter_matrix1(13,14) = a0_HmHmc_HmHmc(unitarity_s,2,1,2,2,13,14) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,1) = a0_HmHmc_AhAh(unitarity_s,2,2,1,1,14,1) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,2) = a0_HmHmc_AhAh(unitarity_s,2,2,1,2,14,2) 
scatter_matrix1(14,3) = a0_HmHmc_AhAh(unitarity_s,2,2,2,2,14,3) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,4) = a0_HmHmc_Ahhh(unitarity_s,2,2,1,1,14,4) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,5) = a0_HmHmc_Ahhh(unitarity_s,2,2,1,2,14,5) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,6) = a0_HmHmc_Ahhh(unitarity_s,2,2,2,1,14,6) 
scatter_matrix1(14,7) = a0_HmHmc_Ahhh(unitarity_s,2,2,2,2,14,7) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,8) = a0_HmHmc_hhhh(unitarity_s,2,2,1,1,14,8) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,9) = a0_HmHmc_hhhh(unitarity_s,2,2,1,2,14,9) 
scatter_matrix1(14,10) = a0_HmHmc_hhhh(unitarity_s,2,2,2,2,14,10) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,11) = a0_HmHmc_HmHmc(unitarity_s,2,2,1,1,14,11) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,12) = a0_HmHmc_HmHmc(unitarity_s,2,2,1,2,14,12) 
If (IncludeGoldstoneExternal) scatter_matrix1(14,13) = a0_HmHmc_HmHmc(unitarity_s,2,2,2,1,14,13) 
scatter_matrix1(14,14) = a0_HmHmc_HmHmc(unitarity_s,2,2,2,2,14,14) 


Select CASE (TUcutLevel)  
CASE (2) 
  scatter_matrix1B = scatter_matrix1
Do i=1,14 
  If (RemoveTUpoles(i).eq.1) Then
   scatter_matrix1(i,:) = 0._dp 
   scatter_matrix1(:,i) = 0._dp 
    If (AddR) scatter_matrix1(i,i) = -1111._dp ! to get a fixed order of the eigenvalues 
   scatter_matrix1B(:,i) = 0._dp 
  Else 
   scatter_matrix1B(i,:) = 0._dp 
  End If 
End Do 
CASE (1) 
If ((Abs(max_element_removed)/MaxVal(Abs(scatter_matrix1))).gt.cut_elements) Then 
 ! Write(*,*)  (Abs(max_element_removed)/MaxVal(Abs(scatter_matrix1)))  
End if 
End Select 
If (.not.pole_present) Then 
Call EigenSystem(scatter_matrix1,eigenvalues_matrix1,rot_matrix1,ierr,test)
 If ((TUcutLevel.eq.2).and.(AddR)) Then ! Calculate 'R' 
  scatter_matrix1B = MatMul(scatter_matrix1B,Conjg(Transpose(rot_matrix1))) 
   Do i=1,14 
    If (eigenvalues_matrix1 (i).lt.-1000._dp) Then
     eigenvalues_matrix1(i) = 0._dp 
    Else 
     eigenvalues_matrix1(i) = sqrt(eigenvalues_matrix1(i)**2+  sum(scatter_matrix1B(i,:)**2) )
    End If
   End Do 
 End If 
scattering_eigenvalue_trilinears=MaxVal(Abs(eigenvalues_matrix1)) 
Else 
  scattering_eigenvalue_trilinears = 0._dp 
End if 
If (scattering_eigenvalue_trilinears.gt.max_scattering_eigenvalue_trilinears) Then 
   max_scattering_eigenvalue_trilinears=scattering_eigenvalue_trilinears 
   unitarity_s_best=sqrt(unitarity_s)
End if 
 
!! 2. sub-matrix  
Pole_Present = .false. 
max_element_removed = 0._dp 
RemoveTUpoles = 0 
scatter_matrix2=0._dp 
If (IncludeGoldstoneExternal) scatter_matrix2(1,1) = a0_AhHmc_AhHm(unitarity_s,1,1,1,1,1,1) 
If (IncludeGoldstoneExternal) scatter_matrix2(1,2) = a0_AhHmc_AhHm(unitarity_s,1,1,1,2,1,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(1,3) = a0_AhHmc_AhHm(unitarity_s,1,1,2,1,1,3) 
If (IncludeGoldstoneExternal) scatter_matrix2(1,4) = a0_AhHmc_AhHm(unitarity_s,1,1,2,2,1,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(1,5) = a0_AhHmc_hhHm(unitarity_s,1,1,1,1,1,5) 
If (IncludeGoldstoneExternal) scatter_matrix2(1,6) = a0_AhHmc_hhHm(unitarity_s,1,1,1,2,1,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(1,7) = a0_AhHmc_hhHm(unitarity_s,1,1,2,1,1,7) 
If (IncludeGoldstoneExternal) scatter_matrix2(1,8) = a0_AhHmc_hhHm(unitarity_s,1,1,2,2,1,8) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,1) = a0_AhHmc_AhHm(unitarity_s,1,2,1,1,2,1) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,2) = a0_AhHmc_AhHm(unitarity_s,1,2,1,2,2,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,3) = a0_AhHmc_AhHm(unitarity_s,1,2,2,1,2,3) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,4) = a0_AhHmc_AhHm(unitarity_s,1,2,2,2,2,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,5) = a0_AhHmc_hhHm(unitarity_s,1,2,1,1,2,5) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,6) = a0_AhHmc_hhHm(unitarity_s,1,2,1,2,2,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,7) = a0_AhHmc_hhHm(unitarity_s,1,2,2,1,2,7) 
If (IncludeGoldstoneExternal) scatter_matrix2(2,8) = a0_AhHmc_hhHm(unitarity_s,1,2,2,2,2,8) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,1) = a0_AhHmc_AhHm(unitarity_s,2,1,1,1,3,1) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,2) = a0_AhHmc_AhHm(unitarity_s,2,1,1,2,3,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,3) = a0_AhHmc_AhHm(unitarity_s,2,1,2,1,3,3) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,4) = a0_AhHmc_AhHm(unitarity_s,2,1,2,2,3,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,5) = a0_AhHmc_hhHm(unitarity_s,2,1,1,1,3,5) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,6) = a0_AhHmc_hhHm(unitarity_s,2,1,1,2,3,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,7) = a0_AhHmc_hhHm(unitarity_s,2,1,2,1,3,7) 
If (IncludeGoldstoneExternal) scatter_matrix2(3,8) = a0_AhHmc_hhHm(unitarity_s,2,1,2,2,3,8) 
If (IncludeGoldstoneExternal) scatter_matrix2(4,1) = a0_AhHmc_AhHm(unitarity_s,2,2,1,1,4,1) 
If (IncludeGoldstoneExternal) scatter_matrix2(4,2) = a0_AhHmc_AhHm(unitarity_s,2,2,1,2,4,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(4,3) = a0_AhHmc_AhHm(unitarity_s,2,2,2,1,4,3) 
scatter_matrix2(4,4) = a0_AhHmc_AhHm(unitarity_s,2,2,2,2,4,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(4,5) = a0_AhHmc_hhHm(unitarity_s,2,2,1,1,4,5) 
If (IncludeGoldstoneExternal) scatter_matrix2(4,6) = a0_AhHmc_hhHm(unitarity_s,2,2,1,2,4,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(4,7) = a0_AhHmc_hhHm(unitarity_s,2,2,2,1,4,7) 
scatter_matrix2(4,8) = a0_AhHmc_hhHm(unitarity_s,2,2,2,2,4,8) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,1) = a0_hhHmc_AhHm(unitarity_s,1,1,1,1,5,1) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,2) = a0_hhHmc_AhHm(unitarity_s,1,1,1,2,5,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,3) = a0_hhHmc_AhHm(unitarity_s,1,1,2,1,5,3) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,4) = a0_hhHmc_AhHm(unitarity_s,1,1,2,2,5,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,5) = a0_hhHmc_hhHm(unitarity_s,1,1,1,1,5,5) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,6) = a0_hhHmc_hhHm(unitarity_s,1,1,1,2,5,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,7) = a0_hhHmc_hhHm(unitarity_s,1,1,2,1,5,7) 
If (IncludeGoldstoneExternal) scatter_matrix2(5,8) = a0_hhHmc_hhHm(unitarity_s,1,1,2,2,5,8) 
If (IncludeGoldstoneExternal) scatter_matrix2(6,1) = a0_hhHmc_AhHm(unitarity_s,1,2,1,1,6,1) 
scatter_matrix2(6,2) = a0_hhHmc_AhHm(unitarity_s,1,2,1,2,6,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(6,3) = a0_hhHmc_AhHm(unitarity_s,1,2,2,1,6,3) 
scatter_matrix2(6,4) = a0_hhHmc_AhHm(unitarity_s,1,2,2,2,6,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(6,5) = a0_hhHmc_hhHm(unitarity_s,1,2,1,1,6,5) 
scatter_matrix2(6,6) = a0_hhHmc_hhHm(unitarity_s,1,2,1,2,6,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(6,7) = a0_hhHmc_hhHm(unitarity_s,1,2,2,1,6,7) 
scatter_matrix2(6,8) = a0_hhHmc_hhHm(unitarity_s,1,2,2,2,6,8) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,1) = a0_hhHmc_AhHm(unitarity_s,2,1,1,1,7,1) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,2) = a0_hhHmc_AhHm(unitarity_s,2,1,1,2,7,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,3) = a0_hhHmc_AhHm(unitarity_s,2,1,2,1,7,3) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,4) = a0_hhHmc_AhHm(unitarity_s,2,1,2,2,7,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,5) = a0_hhHmc_hhHm(unitarity_s,2,1,1,1,7,5) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,6) = a0_hhHmc_hhHm(unitarity_s,2,1,1,2,7,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,7) = a0_hhHmc_hhHm(unitarity_s,2,1,2,1,7,7) 
If (IncludeGoldstoneExternal) scatter_matrix2(7,8) = a0_hhHmc_hhHm(unitarity_s,2,1,2,2,7,8) 
If (IncludeGoldstoneExternal) scatter_matrix2(8,1) = a0_hhHmc_AhHm(unitarity_s,2,2,1,1,8,1) 
scatter_matrix2(8,2) = a0_hhHmc_AhHm(unitarity_s,2,2,1,2,8,2) 
If (IncludeGoldstoneExternal) scatter_matrix2(8,3) = a0_hhHmc_AhHm(unitarity_s,2,2,2,1,8,3) 
scatter_matrix2(8,4) = a0_hhHmc_AhHm(unitarity_s,2,2,2,2,8,4) 
If (IncludeGoldstoneExternal) scatter_matrix2(8,5) = a0_hhHmc_hhHm(unitarity_s,2,2,1,1,8,5) 
scatter_matrix2(8,6) = a0_hhHmc_hhHm(unitarity_s,2,2,1,2,8,6) 
If (IncludeGoldstoneExternal) scatter_matrix2(8,7) = a0_hhHmc_hhHm(unitarity_s,2,2,2,1,8,7) 
scatter_matrix2(8,8) = a0_hhHmc_hhHm(unitarity_s,2,2,2,2,8,8) 


Select CASE (TUcutLevel)  
CASE (2) 
  scatter_matrix2B = scatter_matrix2
Do i=1,8 
  If (RemoveTUpoles(i).eq.1) Then
   scatter_matrix2(i,:) = 0._dp 
   scatter_matrix2(:,i) = 0._dp 
    If (AddR) scatter_matrix2(i,i) = -1111._dp ! to get a fixed order of the eigenvalues 
   scatter_matrix2B(:,i) = 0._dp 
  Else 
   scatter_matrix2B(i,:) = 0._dp 
  End If 
End Do 
CASE (1) 
If ((Abs(max_element_removed)/MaxVal(Abs(scatter_matrix2))).gt.cut_elements) Then 
 ! Write(*,*)  (Abs(max_element_removed)/MaxVal(Abs(scatter_matrix2)))  
End if 
End Select 
If (.not.pole_present) Then 
Call EigenSystem(scatter_matrix2,eigenvalues_matrix2,rot_matrix2,ierr,test)
 If ((TUcutLevel.eq.2).and.(AddR)) Then ! Calculate 'R' 
  scatter_matrix2B = MatMul(scatter_matrix2B,Conjg(Transpose(rot_matrix2))) 
   Do i=1,8 
    If (eigenvalues_matrix2 (i).lt.-1000._dp) Then
     eigenvalues_matrix2(i) = 0._dp 
    Else 
     eigenvalues_matrix2(i) = sqrt(eigenvalues_matrix2(i)**2+  sum(scatter_matrix2B(i,:)**2) )
    End If
   End Do 
 End If 
scattering_eigenvalue_trilinears=MaxVal(Abs(eigenvalues_matrix2)) 
Else 
  scattering_eigenvalue_trilinears = 0._dp 
End if 
If (scattering_eigenvalue_trilinears.gt.max_scattering_eigenvalue_trilinears) Then 
   max_scattering_eigenvalue_trilinears=scattering_eigenvalue_trilinears 
   unitarity_s_best=sqrt(unitarity_s)
End if 
 
!! 3. sub-matrix  
Pole_Present = .false. 
max_element_removed = 0._dp 
RemoveTUpoles = 0 
scatter_matrix3=0._dp 
If (IncludeGoldstoneExternal) scatter_matrix3(1,1) = a0_HmcHmc_HmHm(unitarity_s,1,1,1,1,1,1) 
If (IncludeGoldstoneExternal) scatter_matrix3(1,2) = a0_HmcHmc_HmHm(unitarity_s,1,1,1,2,1,2) 
If (IncludeGoldstoneExternal) scatter_matrix3(1,3) = a0_HmcHmc_HmHm(unitarity_s,1,1,2,2,1,3) 
If (IncludeGoldstoneExternal) scatter_matrix3(2,1) = a0_HmcHmc_HmHm(unitarity_s,1,2,1,1,2,1) 
If (IncludeGoldstoneExternal) scatter_matrix3(2,2) = a0_HmcHmc_HmHm(unitarity_s,1,2,1,2,2,2) 
If (IncludeGoldstoneExternal) scatter_matrix3(2,3) = a0_HmcHmc_HmHm(unitarity_s,1,2,2,2,2,3) 
If (IncludeGoldstoneExternal) scatter_matrix3(3,1) = a0_HmcHmc_HmHm(unitarity_s,2,2,1,1,3,1) 
If (IncludeGoldstoneExternal) scatter_matrix3(3,2) = a0_HmcHmc_HmHm(unitarity_s,2,2,1,2,3,2) 
scatter_matrix3(3,3) = a0_HmcHmc_HmHm(unitarity_s,2,2,2,2,3,3) 


Select CASE (TUcutLevel)  
CASE (2) 
  scatter_matrix3B = scatter_matrix3
Do i=1,3 
  If (RemoveTUpoles(i).eq.1) Then
   scatter_matrix3(i,:) = 0._dp 
   scatter_matrix3(:,i) = 0._dp 
    If (AddR) scatter_matrix3(i,i) = -1111._dp ! to get a fixed order of the eigenvalues 
   scatter_matrix3B(:,i) = 0._dp 
  Else 
   scatter_matrix3B(i,:) = 0._dp 
  End If 
End Do 
CASE (1) 
If ((Abs(max_element_removed)/MaxVal(Abs(scatter_matrix3))).gt.cut_elements) Then 
 ! Write(*,*)  (Abs(max_element_removed)/MaxVal(Abs(scatter_matrix3)))  
End if 
End Select 
If (.not.pole_present) Then 
Call EigenSystem(scatter_matrix3,eigenvalues_matrix3,rot_matrix3,ierr,test)
 If ((TUcutLevel.eq.2).and.(AddR)) Then ! Calculate 'R' 
  scatter_matrix3B = MatMul(scatter_matrix3B,Conjg(Transpose(rot_matrix3))) 
   Do i=1,3 
    If (eigenvalues_matrix3 (i).lt.-1000._dp) Then
     eigenvalues_matrix3(i) = 0._dp 
    Else 
     eigenvalues_matrix3(i) = sqrt(eigenvalues_matrix3(i)**2+  sum(scatter_matrix3B(i,:)**2) )
    End If
   End Do 
 End If 
scattering_eigenvalue_trilinears=MaxVal(Abs(eigenvalues_matrix3)) 
Else 
  scattering_eigenvalue_trilinears = 0._dp 
End if 
If (scattering_eigenvalue_trilinears.gt.max_scattering_eigenvalue_trilinears) Then 
   max_scattering_eigenvalue_trilinears=scattering_eigenvalue_trilinears 
   unitarity_s_best=sqrt(unitarity_s)
End if 
 
End do 

If (max_scattering_eigenvalue_trilinears.gt.0.5_dp) TreeUnitarityTrilinear=0._dp 
 
! Write(*,*) (max_scattering_eigenvalue_trilinears) 
 


 Contains 

Complex(dp) Function a0_AhAh_AhAh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MAh(i2)
m3 = MAh(i3)
m4 = MAh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhAhAh(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i2,iprop),cplAhAhAh(i3,i4,iprop)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i2,iprop),cplAhAhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhAh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhAh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhAh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhAh(i2,i4,iprop)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhAhhh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhAhhh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhAhhh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhAhhh(i2,i4,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhAh(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhAh(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhAh(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhAh(i2,i3,iprop)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhAh_AhAh

Complex(dp) Function a0_AhAh_Ahhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MAh(i2)
m3 = MAh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhAhhh(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i2,iprop),cplAhAhhh(i3,iprop,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i2,iprop),cplAhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i2,iprop,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i2,iprop,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i2,iprop,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i2,iprop,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhAh(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhAh(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhAh(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhAh(i2,i3,iprop)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhAhhh(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhAh_Ahhh

Complex(dp) Function a0_AhAh_hhhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MAh(i2)
m3 = Mhh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhhhhh(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i2,iprop),cplAhhhhh(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i2,iprop),cplhhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhAhhh(i2,iprop,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhAhhh(i2,iprop,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhAhhh(i2,iprop,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhAhhh(i2,iprop,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplAhhhhh(i2,i4,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i2,iprop,i3))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i2,iprop,i3))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i2,iprop,i3)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i2,iprop,i3)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhAh_hhhh

Complex(dp) Function a0_AhAh_HmHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MAh(i2)
m3 = MHm(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhHmcHm(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i2,iprop),cplAhHmcHm(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i2,iprop),cplhhHmcHm(iprop,i3,i4)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplAhHmcHm(i2,iprop,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplAhHmcHm(i2,iprop,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplAhHmcHm(i2,iprop,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplAhHmcHm(i2,iprop,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhAh_HmHmc

Complex(dp) Function a0_Ahhh_AhAh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = Mhh(i2)
m3 = MAh(i3)
m4 = MAh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhAhhh(i1,i3,i4,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i2),cplAhAhAh(i3,i4,iprop)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i2,iprop),cplAhAhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i4,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i4,iprop,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i4,iprop,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhAhhh(i4,iprop,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i4,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i4,i2,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i4,i2,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplAhhhhh(i4,i2,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhhh(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhhh(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhhh(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i4,iprop),cplAhAhhh(i3,iprop,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_Ahhh_AhAh

Complex(dp) Function a0_Ahhh_Ahhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = Mhh(i2)
m3 = MAh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhhhhh(i1,i3,i2,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i2),cplAhAhhh(i3,iprop,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i2,iprop),cplAhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhhhhh(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhhhhh(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhhhhh(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhhhhh(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhAhhh(i3,iprop,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_Ahhh_Ahhh

Complex(dp) Function a0_Ahhh_hhhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = Mhh(i2)
m3 = Mhh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhhhhh(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i2),cplAhhhhh(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i2,iprop),cplhhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhhhhh(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhhhhh(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhhhhh(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhhhhh(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhhhhh(iprop,i2,i3))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhhhhh(iprop,i2,i3))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhhhhh(iprop,i2,i3)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i4),cplAhhhhh(iprop,i2,i3)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_Ahhh_hhhh

Complex(dp) Function a0_Ahhh_HmHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = Mhh(i2)
m3 = MHm(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhHmcHm(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i2),cplAhHmcHm(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i2,iprop),cplhhHmcHm(iprop,i3,i4)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_Ahhh_HmHmc

Complex(dp) Function a0_AhHm_AhHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MHm(i2)
m3 = MAh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhHmcHm(i1,i3,i2,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i2,iprop),cplAhHmcHm(i3,iprop,i4)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhHm_AhHmc

Complex(dp) Function a0_AhHm_hhHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MHm(i2)
m3 = Mhh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhHmcHm(i1,i3,i2,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i2,iprop),cplhhHmcHm(i3,iprop,i4)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhHm_hhHmc

Complex(dp) Function a0_AhHmc_AhHm(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MHm(i2)
m3 = MAh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhHmcHm(i1,i3,i4,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i2),cplAhHmcHm(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhAh(i1,i3,iprop),cplAhHmcHm(iprop,i4,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhAhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhHmc_AhHm

Complex(dp) Function a0_AhHmc_hhHm(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MAh(i1)
m2 = MHm(i2)
m3 = Mhh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhHmcHm(i1,i3,i4,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,iprop,i2),cplhhHmcHm(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i1,iprop,i3),cplAhHmcHm(iprop,i4,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_AhHmc_hhHm

Complex(dp) Function a0_hhhh_AhAh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = Mhh(i2)
m3 = MAh(i3)
m4 = MAh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhhhhh(i3,i4,i1,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i2),cplAhAhAh(i3,i4,iprop)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i2,iprop),cplAhAhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhAhhh(i4,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhAhhh(i4,iprop,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhAhhh(i4,iprop,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhAhhh(i4,iprop,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplAhhhhh(i4,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplAhhhhh(i4,i2,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplAhhhhh(i4,i2,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplAhhhhh(i4,i2,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i4,iprop,i1),cplAhAhhh(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i4,iprop,i1),cplAhAhhh(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i4,iprop,i1),cplAhAhhh(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i4,iprop,i1),cplAhAhhh(i3,iprop,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i4,i1,iprop),cplAhhhhh(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i4,i1,iprop),cplAhhhhh(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i4,i1,iprop),cplAhhhhh(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i4,i1,iprop),cplAhhhhh(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhhh_AhAh

Complex(dp) Function a0_hhhh_Ahhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = Mhh(i2)
m3 = MAh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhhhhh(i3,i1,i2,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i2),cplAhAhhh(i3,iprop,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i2,iprop),cplAhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhhhhh(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhhhhh(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhhhhh(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhhhhh(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhhhhh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhhhhh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhhhhh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhhhhh(i2,i4,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhAhhh(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhAhhh(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhAhhh(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhAhhh(i3,iprop,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplAhhhhh(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhhh_Ahhh

Complex(dp) Function a0_hhhh_hhhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = Mhh(i2)
m3 = Mhh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplhhhhhhhh(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i2),cplAhhhhh(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i2,iprop),cplhhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhhhhh(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhhhhh(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhhhhh(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhhhhh(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhhhhh(i2,i4,iprop)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhhhhh(iprop,i2,i3))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhhhhh(iprop,i2,i3))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhhhhh(iprop,i2,i3)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i4),cplAhhhhh(iprop,i2,i3)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i4,iprop),cplhhhhhh(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhhh_hhhh

Complex(dp) Function a0_hhhh_HmHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = Mhh(i2)
m3 = MHm(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplhhhhHmcHm(i1,i2,i3,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i2),cplAhHmcHm(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i2,iprop),cplhhHmcHm(iprop,i3,i4)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i3,iprop),cplhhHmcHm(i2,iprop,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i2,i3,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhhh_HmHmc

Complex(dp) Function a0_hhHm_AhHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = MHm(i2)
m3 = MAh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhHmcHm(i3,i1,i2,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i2,iprop),cplAhHmcHm(i3,iprop,i4)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i2,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplAhHmcHm(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhHm_AhHmc

Complex(dp) Function a0_hhHm_hhHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = MHm(i2)
m3 = Mhh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplhhhhHmcHm(i1,i3,i2,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i2,iprop),cplhhHmcHm(i3,iprop,i4)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i2,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i4),cplhhHmcHm(i3,i2,iprop)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhHm_hhHmc

Complex(dp) Function a0_hhHmc_AhHm(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = MHm(i2)
m3 = MAh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhHmcHm(i3,i1,i4,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i2),cplAhHmcHm(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhAhhh(i3,iprop,i1),cplAhHmcHm(iprop,i4,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplAhhhhh(i3,i1,iprop),cplhhHmcHm(iprop,i4,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplAhHmcHm(i3,iprop,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhHmc_AhHm

Complex(dp) Function a0_hhHmc_hhHm(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = Mhh(i1)
m2 = MHm(i2)
m3 = Mhh(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplhhhhHmcHm(i1,i3,i4,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MHm(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,iprop,i2),cplhhHmcHm(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhhhhh(iprop,i1,i3),cplAhHmcHm(iprop,i4,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhhhhh(i1,i3,iprop),cplhhHmcHm(iprop,i4,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i1,i4,iprop),cplhhHmcHm(i3,iprop,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_hhHmc_hhHm

Complex(dp) Function a0_HmHm_HmcHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MHm(i1)
m2 = MHm(i2)
m3 = MHm(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplHmHmcHmcHm(i1,i2,i3,i4)


! S-Channel 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i3),cplAhHmcHm(iprop,i2,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i3),cplhhHmcHm(iprop,i2,i4))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i3),cplhhHmcHm(iprop,i2,i4))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i3),cplhhHmcHm(iprop,i2,i4)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i3),cplhhHmcHm(iprop,i2,i4)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i2,i3))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i2,i3))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i2,i3)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i2,i3)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i2,i3))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i2,i3))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i2,i3)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i2,i3)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_HmHm_HmcHmc

Complex(dp) Function a0_HmHmc_AhAh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MHm(i1)
m2 = MHm(i2)
m3 = MAh(i3)
m4 = MAh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhAhHmcHm(i3,i4,i1,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i2),cplAhAhAh(i3,i4,iprop)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i2),cplAhAhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplAhHmcHm(i4,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplAhHmcHm(i4,iprop,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplAhHmcHm(i4,iprop,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplAhHmcHm(i4,iprop,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_HmHmc_AhAh

Complex(dp) Function a0_HmHmc_Ahhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MHm(i1)
m2 = MHm(i2)
m3 = MAh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplAhhhHmcHm(i3,i4,i1,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i2),cplAhAhhh(i3,iprop,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i2),cplAhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplAhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplAhHmcHm(i3,iprop,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_HmHmc_Ahhh

Complex(dp) Function a0_HmHmc_hhhh(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MHm(i1)
m2 = MHm(i2)
m3 = Mhh(i3)
m4 = Mhh(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplhhhhHmcHm(i3,i4,i1,i2)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i2),cplAhhhhh(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i2),cplhhhhhh(i3,i4,iprop)) 
End if 
End Do 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i3,i1,iprop),cplhhHmcHm(i4,iprop,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MHm(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplhhHmcHm(i3,iprop,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MHm(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplhhHmcHm(i3,iprop,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplhhHmcHm(i3,iprop,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MHm(iProp),s,cplhhHmcHm(i4,i1,iprop),cplhhHmcHm(i3,iprop,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_HmHmc_hhhh

Complex(dp) Function a0_HmHmc_HmHmc(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MHm(i1)
m2 = MHm(i2)
m3 = MHm(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplHmHmcHmcHm(i1,i3,i2,i4)


! S-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If (Abs(1-s/MAh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i2),cplAhHmcHm(iprop,i3,i4)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If (Abs(1-s/Mhh(iProp)**2).lt.0.25_dp) Then 
 Pole_Present = .true. 
Else 
  amp = amp + Schannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i2),cplhhHmcHm(iprop,i3,i4)) 
End if 
End Do 


! T-Channel 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i3,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i3,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i3,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i1,i4),cplAhHmcHm(iprop,i3,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i3,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i3,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i3,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i1,i4),cplhhHmcHm(iprop,i3,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_HmHmc_HmHmc

Complex(dp) Function a0_HmcHmc_HmHm(s,i1,i2,i3,i4,ind1,ind2)  Result(amp)
Implicit None 
Integer, Intent(in) :: i1,i2,i3,i4,ind1,ind2 
Real(dp), Intent(in) :: s 
Integer :: iprop, istart 
Logical :: pole_s_channel=.False. 
Real(dp) :: m1,m2,m3,m4 
Complex(dp) :: amp_poles 
m1 = MHm(i1)
m2 = MHm(i2)
m3 = MHm(i3)
m4 = MHm(i4)
amp = 0._dp 
amp_poles = 0._dp 
If ((s.gt.1.25_dp*(m3+m4)**2).and.(s.gt.1.25_dp*(m1+m2)**2)) Then 


! Quartic 
amp = amp -2._dp*cplHmHmcHmcHm(i3,i4,i1,i2)


! S-Channel 


! T-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i3,i1),cplAhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i3,i1),cplAhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i3,i1),cplAhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i3,i1),cplAhHmcHm(iprop,i4,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckTpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i3,i1),cplhhHmcHm(iprop,i4,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "T",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i3,i1),cplhhHmcHm(iprop,i4,i2))) 
 Case (0) 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i3,i1),cplhhHmcHm(iprop,i4,i2)) 
End Select 
Else 
  amp = amp + Tchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i3,i1),cplhhHmcHm(iprop,i4,i2)) 
End if 
End Do 


! U-Channel 
istart=1
If (.not.IncludeGoldstoneContributions) istart=2
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,MAh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i4,i1),cplAhHmcHm(iprop,i3,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,MAh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i4,i1),cplAhHmcHm(iprop,i3,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i4,i1),cplAhHmcHm(iprop,i3,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,MAh(iProp),s,cplAhHmcHm(iprop,i4,i1),cplAhHmcHm(iprop,i3,i2)) 
End if 
End Do 
istart=1
Do iprop=istart,2
If  (((s.lt.(CheckUpole(m1**2,m2**2,m3**2,m4**2,Mhh(iProp)**2)))).and.(Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i4,i1),cplhhHmcHm(iprop,i3,i2))).gt.1.0E-10_dp)) Then 
! Write(*,*) "U",m1,m2,m3,m4,Mhh(iProp)  
Select Case (TUcutLevel) 
 Case (3) 
   Pole_Present = .True. 
 Case (2) 
  RemoveTUpoles(ind1) = 1 
  RemoveTUpoles(ind2) = 1 
 Case (1) 
  amp_poles = amp_poles + Abs(Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i4,i1),cplhhHmcHm(iprop,i3,i2))) 
 Case (0) 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i4,i1),cplhhHmcHm(iprop,i3,i2)) 
End Select 
Else 
  amp = amp + Uchannel(m1,m2,m3,m4,Mhh(iProp),s,cplhhHmcHm(iprop,i4,i1),cplhhHmcHm(iprop,i3,i2)) 
End if 
End Do 
amp = 0.5_dp*oo16pi*amp*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp=amp/sqrt(2._dp) 
If (i3.eq.i4) amp=amp/sqrt(2._dp) 
End if 
If (TUcutLevel.eq.1) Then 
 amp_poles = 0.5_dp*oo16pi*amp_poles*sqrt(sqrt(Kaehler(s,m1**2,m2**2)*Kaehler(s,m3**2,m4**2)))/s 
If (i1.eq.i2) amp_poles=amp_poles/sqrt(2._dp) 
If (i3.eq.i4) amp_poles=amp_poles/sqrt(2._dp) 
  If ((Abs(amp_poles)/Abs(amp)).gt.cut_amplitudes) Then 
   ! Write(*,*) "TU ratio", (Abs(amp_poles)/Abs(amp))  
   If ((Abs(amp)).gt.max_element_removed) max_element_removed = Abs(amp) 
   amp = 0._dp 
  End if 
End if 
End Function a0_HmcHmc_HmHm

End Subroutine ScatteringEigenvaluesWithTrilinears

Complex(dp) Function Kaehler(a,b,c) 
Implicit None 
Real(dp),Intent(in)::a,b,c
Kaehler = a**2-2._dp*a*(b+c)+(b-c)**2 
End Function Kaehler 
  
Real(dp) Function CheckTpole(m1,m2,m3,m4,mP) 
Implicit None 
Real(dp),Intent(in)::m1,m2,m3,m4,mP
Complex(dp):: res
res = (m2*m3 - m3*m4 + m2*mP + m3*mP + m4*mP - mP**2 + m1*(-m2 + m4 + mP) + Sqrt(m1**2 + (m3 - mP)**2 &
  &  - 2*m1*(m3 + mP))*Sqrt(m2**2 + (m4 - mP)**2 - 2*m2*(m4 + mP)))/(2.*mP)
If (Aimag(res).gt.1._dp) Then 
 CheckTpole = 0._dp 
Else 
 CheckTpole = Real(res,dp) 
End If 
End Function CheckTpole 
 
Real(dp) Function CheckUpole(m1,m2,m3,m4,mP) 
Implicit None 
Real(dp),Intent(in)::m1,m2,m3,m4,mP
Complex(dp) :: res
res =(m2*m4-m3*m4+m2*mP+m3*mP+m4*mP-mP**2+m1*(-m2+m3+mP)+Sqrt(m2**2+(m3-mP)**2 &
  &-2*m2*(m3+mP))*Sqrt(m1**2+(m4-mP)**2-2*m1*(m4+mP)))/(2.*mP)
If (Aimag(res).gt.1._dp) Then 
 CheckUpole = 0._dp 
Else 
 CheckUpole = Real(res,dp) 
End If 
End Function CheckUpole 
 
Complex(dp) Function Schannel(m1,m2,m3,m4,mP,s,c1,c2) 
Implicit None 
Real(dp),Intent(in)::m1,m2,m3,m4,mP,s
Complex(dp),Intent(in)::c1,c2
Schannel = 2._dp/(-mP**2+s) 
Schannel = c1*c2*Schannel 
End Function Schannel 
 
Complex(dp) Function Uchannel(m1r,m2r,m3r,m4r,mPr,s,c1,c2) 
Implicit None 
Real(dp),Intent(in)::m1r,m2r,m3r,m4r,mPr,s
Complex(dp),Intent(in)::c1,c2
Complex(dp)::m1,m2,m3,m4,mP 
m1=Cmplx(m1r,0._dp)
m2=Cmplx(m2r,0._dp)
m3=Cmplx(m3r,0._dp)
m4=Cmplx(m4r,0._dp)
mP=Cmplx(mPr,0._dp)
Uchannel = (2*s*Log(-(((m1 - m2)*(m1 + m2)*(m3 - m4)*(m3 + m4) + (m1**2 + m2**2 + m3**2 + m4**2 - 2*mP**2)*s & 
  & - s**2 + Sqrt((m1**4 + (m2**2 - s)**2 - 2*m1**2*(m2**2 + s))*(m3**4 + (m4**2 - s)**2 - 2*m3**2*(m4**2 + s))))/& 
  &((-m1 + m2)*(m1 + m2)*(m3 - m4)*(m3 + m4) - (m1**2 + m2**2 + m3**2 + m4**2 - 2*mP**2)*s + s**2 + & 
  & Sqrt((m1**4 + (m2**2 - s)**2 - 2*m1**2*(m2**2 + s))*(m3**4 + (m4**2 - s)**2 - 2*m3**2*(m4**2 + s)))))))/& 
 &Sqrt((m1**4 + (m2**2 - s)**2 - 2*m1**2*(m2**2 + s))*(m3**4 + (m4**2 - s)**2 - 2*m3**2*(m4**2 + s))) 
Uchannel = c1*c2*Uchannel 
End Function Uchannel 
  
Complex(dp) Function Tchannel(m1r,m2r,m3r,m4r,mPr,s,c1,c2) 
Implicit None 
Real(dp),Intent(in)::m1r,m2r,m3r,m4r,mPr,s
Complex(dp),Intent(in)::c1,c2
Complex(dp)::m1,m2,m3,m4,mP 
m1=Cmplx(m1r,0._dp)
m2=Cmplx(m2r,0._dp)
m3=Cmplx(m3r,0._dp)
m4=Cmplx(m4r,0._dp)
mP=Cmplx(mPr,0._dp)
Tchannel =(2*s*Log(((m1-m2)*(m1+m2)*(m3-m4)*(m3+m4)-(m1**2+m2**2+m3**2+m4**2-2*mP**2)*s+s**2& 
  & -Sqrt((m1**4+(m2**2-s)**2-2*m1**2*(m2**2+s))*(m3**4+(m4**2-s)**2-2*m3**2*(m4**2+s))))/& 
  & ((m1-m2)*(m1+m2)*(m3-m4)*(m3+m4)-(m1**2+m2**2+m3**2+m4**2-2*mP**2)*s+s**2+Sqrt((m1**4+(m2**2-s)**2& 
  & -2*m1**2*(m2**2+s))*(m3**4+(m4**2-s)**2-2*m3**2*(m4**2+s))))))/& 
  & Sqrt((m1**4+(m2**2-s)**2-2*m1**2*(m2**2+s))*(m3**4+(m4**2-s)**2-2*m3**2*(m4**2+s))) 
Tchannel = c1*c2*Tchannel 
End Function Tchannel 
  
End Module Unitarity_THDM_GEN 
