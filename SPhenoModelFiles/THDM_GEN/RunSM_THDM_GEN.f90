! -----------------------------------------------------------------------------  
! This file was automatically created by SARAH version 4.13.0 
! SARAH References: arXiv:0806.0538, 0909.2863, 1002.0840, 1207.0906, 1309.7223  
! (c) Florian Staub, 2013  
! ------------------------------------------------------------------------------  
! File created at 15:20 on 22.8.2018   
! ----------------------------------------------------------------------  
 
 
Module RunSM_THDM_GEN 
 
Use Control 
Use Settings 
Use LoopFunctions 
Use Mathematics 
Use StandardModel 
Use RGEs_THDM_GEN 
Use RGEs_SM_HC 
Use Model_Data_THDM_GEN 

Logical,Private,Save::OnlyDiagonal 
Contains 
 
 Subroutine RunSM_and_SUSY_RGEs(Qout,g1input,g2input,g3input,Lam6input,Lam5input,      & 
& Lam7input,Lam1input,Lam4input,Lam3input,Lam2input,eta1Uinput,eta2Uinput,               & 
& eta1Dinput,eta1Linput,eta2Dinput,eta2Linput,M12input,M112input,M222input,              & 
& vdinput,vuinput,g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U,eta2U,               & 
& eta1D,eta1L,eta2D,eta2L,M12,M112,M222,vd,vu,CKMout,sinW2_out,Alpha_out,AlphaS_out,realCKM)

Implicit None 
Real(dp),Intent(in) :: g1input,g2input,g3input,vdinput,vuinput

Complex(dp),Intent(in) :: Lam6input,Lam5input,Lam7input,Lam1input,Lam4input,Lam3input,Lam2input,eta1Uinput(3,3),& 
& eta2Uinput(3,3),eta1Dinput(3,3),eta1Linput(3,3),eta2Dinput(3,3),eta2Linput(3,3),       & 
& M12input,M112input,M222input

Real(dp),Intent(out) :: g1,g2,g3,vd,vu

Complex(dp),Intent(out) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Real(dp), Intent(in) :: Qout 
Complex(dp), Intent(out) :: CKMout(3,3) 
Real(dp), Intent(out) :: sinW2_out, Alpha_out, AlphaS_out 
Complex(dp) :: YdSM(3,3), YuSM(3,3), YeSM(3,3) 
Real(dp) :: g1SM, g2SM, g3SM, vSM 
Complex(dp) :: lambdaSM, muSM, dummy(3,3) 
Integer :: kont 
Logical :: OnlyDiagonal 
Logical :: realCKM 
Real(dp) :: deltaM = 0.000001_dp, test(3)  
Real(dp) :: scale_save, Qin, tz, dt, g1D(133), g62_SM(62) 
 
 
! Run SUSY RGEs from M_SUSY to Qin 
Qin=sqrt(getRenormalizationScale()) 
scale_save = Qin 
Call ParametersToG133(g1input,g2input,g3input,Lam6input,Lam5input,Lam7input,          & 
& Lam1input,Lam4input,Lam3input,Lam2input,eta1Uinput,eta2Uinput,eta1Dinput,              & 
& eta1Linput,eta2Dinput,eta2Linput,M12input,M112input,M222input,vdinput,vuinput,g1D)

Call GToParameters133(g1D,g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U,          & 
& eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222,vd,vu)

g1 = 1*g1 


! Run SM RGEs from MZ to Qin 
If (RunningSMparametersLowEnergy) Then 
! Run SM RGEs separately 
 
! Get values of gauge and Yukawa couplings at M_Z 
Call GetRunningSMparametersMZ(YdSM,YeSM,YuSM,g1SM,g2SM,g3SM,lambdaSM,muSM,            & 
& vSM,realCKM)

Call ParametersToG62_SM(g1SM, g2SM, g3SM, lambdaSM, YuSM, YdSM, YeSM, muSM, vSM, g62_SM) 
! Run to output scale 
tz=Log(sqrt(MZ2)/Qout) 
dt=tz/100._dp 
Call odeint(g62_SM,62,tz,0._dp,deltaM,dt,0._dp,rge62_SM,kont)

Call GtoParameters62_SM(g62_SM, g1SM, g2SM, g3SM, lambdaSM, YuSM, YdSM, YeSM, muSM, vSM) 
 
! Overwrite values obtained from SUSY running 
g1 = g1SM 
g2 = g2SM 
g3 = g3SM 
vd=vSM/Sqrt(1._dp+TanBeta**2) 
vu=TanBeta*vd 
! Calculate running CKM matrix 
Call FermionMass(YuSM,1._dp,test,dummy,CKMout,kont) 
 

 
 ! Output values for running SM constants 
sinW2_out = g1SM**2/(g1SM**2+g2SM**2) 
Alpha_out = sinW2_out*g2SM**2/(4._dp*Pi) 
AlphaS_out = g3SM**2/(4._dp*Pi) 
 
Else 

! Don't run SM RGEs separately 
CKMout = CKMcomplex 
sinW2_out = g1**2/(g1**2+g2**2) 
Alpha_out = sinW2_out*g2**2/(4._dp*Pi) 
AlphaS_out = g3**2/(4._dp*Pi) 
Call SetMatchingConditions(g1SM,g2SM,g3SM,YuSM,YdSM,YeSM,vSM,vd,vu,g1,g2,             & 
& g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U,eta2U,eta1D,eta1L,eta2D,eta2L,             & 
& M12,M112,M222,.False.)

End if 

Qin = SetRenormalizationScale(Qout**2) 
End Subroutine RunSM_and_SUSY_RGEs 
 
 
Subroutine GetRunningSMparametersMZ(YdSM,YeSM,YuSM,g1SM,g2SM,g3SM,lambdaSM,           & 
& muSM,vSM,realCKM)

Implicit None 
Complex(dp), Intent(out) :: YdSM(3,3), YuSM(3,3), YeSM(3,3) 
Real(dp), Intent(out) :: g1SM, g2SM, g3SM, vSM 
Complex(dp), Intent(out) :: lambdaSM, muSM 
Real(dp) :: vev2, sinW2, CosW2SinW2 
Real(dp) :: gSM2(2), gSM3(3), mtopMS, mtopMS_MZ 
Real(dp) :: dt, tz
Real(dp) :: deltaM = 0.000001_dp, test(3)  
Logical :: realCKM 
Integer :: i1,kont 
 
 
SinW2=0.22290_dp 
CosW2SinW2=(1._dp-sinW2)*sinW2 
vev2=mZ2*CosW2SinW2/(pi*Alpha_mZ) -0 
vSM = sqrt(vev2) 
 
YdSM = 0._dp 
YeSM = 0._dp 
YuSM = 0._dp 
 
Do i1=1,3 
YdSM(i1,i1) = sqrt2*mf_d_mz(i1)/vSM 
YeSM(i1,i1) = sqrt2*mf_l_mz(i1)/vSM 
YuSM(i1,i1) = sqrt2*mf_u_mz(i1)/vSM 
End do 
 

! Calculating m_top(M_Z) 
gSM2(1)=sqrt(Alpha_mZ*4*Pi) 
gSM2(2)=sqrt(AlphaS_mZ*4*Pi) 
tz=Log(sqrt(mz2)/mf_u(3)) 
dt=tz/50._dp 
Call odeint(gSM2,2,tz,0._dp,deltaM,dt,0._dp,RGEAlphaS,kont) 
 
!m_top^pole to m_top^MS(m_top) 
mtopMS=mf_u(3)*(1._dp-4._dp/3._dp*(gSM2(2)**2/4._dp/Pi )/Pi) 

!Running m_top^MS(m_top) to M_Z 
gSM3(1)=gSM2(1) 
gSM3(2)=gSM2(2) 
gSM3(3)=mtopMS 
tz=Log(sqrt(mz2)/mf_u(3)) 
dt=tz/50._dp 
Call odeint(gSM3,3,0._dp,tz,deltaM,dt,0._dp,RGEtop,kont) 
mtopMS_MZ=gSM3(3) 
YuSM(3,3) = sqrt2*mtopMS_MZ/vSM 
 

If (realCKM) Then 
 YuSM = Transpose(Matmul(Transpose(Real(CKMcomplex,dp)),Transpose(YuSM))) 
Else 
 YuSM = Transpose(Matmul(Transpose(CKMcomplex),Transpose(YuSM))) 
End if 
g1SM=sqrt(Alpha_MZ/(1-sinW2)*4._dp*Pi) 
g2SM=sqrt(Alpha_MZ/sinW2*4._dp*Pi) 
g3SM=sqrt(AlphaS_MZ*4._dp*Pi) 
 
lambdaSM = 0._dp 
muSM = 0._dp 
 
End Subroutine GetRunningSMparametersMZ 

End Module RunSM_THDM_GEN 
