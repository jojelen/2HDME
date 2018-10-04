! -----------------------------------------------------------------------------  
! This file was automatically created by SARAH version 4.13.0 
! SARAH References: arXiv:0806.0538, 0909.2863, 1002.0840, 1207.0906, 1309.7223  
! (c) Florian Staub, 2013  
! ------------------------------------------------------------------------------  
! File created at 15:20 on 22.8.2018   
! ----------------------------------------------------------------------  
 
 
Module Tadpoles_THDM_GEN 
 
Use Model_Data_THDM_GEN 
Use TreeLevelMasses_THDM_GEN 
Use RGEs_THDM_GEN 
Use Control 
Use Settings 
Use Mathematics 

Contains 


Subroutine SolveTadpoleEquations(g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,              & 
& Lam2,eta1U,eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222,vd,vu,Tad1Loop)

Implicit None
Real(dp),Intent(inout) :: g1,g2,g3,vd,vu

Complex(dp),Intent(inout) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Complex(dp), Intent(in) :: Tad1Loop(2)

! For numerical routines 
Real(dp) :: gC(133)
logical :: broycheck 
Real(dp) :: broyx(2)

If (HighScaleModel.Eq."LOW") Then 
M112 = (-0.5_dp*Lam1*vd**3 + 0.5_dp*M12*vu - 0.75_dp*Lam6*vd**2*vu - 0.5_dp*Lam3*vd*vu**2 -  & 
&  0.5_dp*Lam4*vd*vu**2 - 0.25_dp*Lam5*vd*vu**2 - 0.25_dp*Lam7*vu**3 - 0.25_dp*vd*vu**2*Conjg(Lam5) -& 
&  0.75_dp*vd**2*vu*Conjg(Lam6) - 0.25_dp*vu**3*Conjg(Lam7) + 0.5_dp*vu*Conjg(M12) +     & 
&  1._dp*Tad1Loop(1))/vd
M222 = (0.5_dp*M12*vd - 0.25_dp*Lam6*vd**3 - 0.5_dp*Lam3*vd**2*vu - 0.5_dp*Lam4*vd**2*vu -   & 
&  0.25_dp*Lam5*vd**2*vu - 0.75_dp*Lam7*vd*vu**2 - 0.5_dp*Lam2*vu**3 - 0.25_dp*vd**2*vu*Conjg(Lam5) -& 
&  0.25_dp*vd**3*Conjg(Lam6) - 0.75_dp*vd*vu**2*Conjg(Lam7) + 0.5_dp*vd*Conjg(M12) +     & 
&  1._dp*Tad1Loop(2))/vu

 ! ----------- Check solutions for consistency  -------- 

 ! Check for NaNs 
If (Real(M112,dp).ne.Real(M112,dp)) Then 
   Write(*,*) "NaN appearing in solution of tadpole equations for M112" 
   Call TerminateProgram  
 End If 
 If (Abs(AImag(M112)).gt.1.0E-04_dp) Then 
   Write(*,*) "No real solution of tadpole equations for M112" 
   !Call TerminateProgram  
   M112 = Real(M112,dp) 
  SignOfMuChanged= .True. 
End If 
 If (Real(M222,dp).ne.Real(M222,dp)) Then 
   Write(*,*) "NaN appearing in solution of tadpole equations for M222" 
   Call TerminateProgram  
 End If 
 If (Abs(AImag(M222)).gt.1.0E-04_dp) Then 
   Write(*,*) "No real solution of tadpole equations for M222" 
   !Call TerminateProgram  
   M222 = Real(M222,dp) 
  SignOfMuChanged= .True. 
End If 
 Else 
M112 = (-0.5_dp*Lam1*vd**3 + 0.5_dp*M12*vu - 0.75_dp*Lam6*vd**2*vu - 0.5_dp*Lam3*vd*vu**2 -  & 
&  0.5_dp*Lam4*vd*vu**2 - 0.25_dp*Lam5*vd*vu**2 - 0.25_dp*Lam7*vu**3 - 0.25_dp*vd*vu**2*Conjg(Lam5) -& 
&  0.75_dp*vd**2*vu*Conjg(Lam6) - 0.25_dp*vu**3*Conjg(Lam7) + 0.5_dp*vu*Conjg(M12) +     & 
&  1._dp*Tad1Loop(1))/vd
M222 = (0.5_dp*M12*vd - 0.25_dp*Lam6*vd**3 - 0.5_dp*Lam3*vd**2*vu - 0.5_dp*Lam4*vd**2*vu -   & 
&  0.25_dp*Lam5*vd**2*vu - 0.75_dp*Lam7*vd*vu**2 - 0.5_dp*Lam2*vu**3 - 0.25_dp*vd**2*vu*Conjg(Lam5) -& 
&  0.25_dp*vd**3*Conjg(Lam6) - 0.75_dp*vd*vu**2*Conjg(Lam7) + 0.5_dp*vd*Conjg(M12) +     & 
&  1._dp*Tad1Loop(2))/vu

 ! ----------- Check solutions for consistency  -------- 

 ! Check for NaNs 
If (Real(M112,dp).ne.Real(M112,dp)) Then 
   Write(*,*) "NaN appearing in solution of tadpole equations for M112" 
   Call TerminateProgram  
 End If 
 If (Abs(AImag(M112)).gt.1.0E-04_dp) Then 
   Write(*,*) "No real solution of tadpole equations for M112" 
   !Call TerminateProgram  
   M112 = Real(M112,dp) 
  SignOfMuChanged= .True. 
End If 
 If (Real(M222,dp).ne.Real(M222,dp)) Then 
   Write(*,*) "NaN appearing in solution of tadpole equations for M222" 
   Call TerminateProgram  
 End If 
 If (Abs(AImag(M222)).gt.1.0E-04_dp) Then 
   Write(*,*) "No real solution of tadpole equations for M222" 
   !Call TerminateProgram  
   M222 = Real(M222,dp) 
  SignOfMuChanged= .True. 
End If 
 End if 
End Subroutine SolveTadpoleEquations

Subroutine CalculateTadpoles(g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,             & 
& eta1U,eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222,vd,vu,Tad1Loop,TadpoleValues)

Real(dp),Intent(in) :: g1,g2,g3,vd,vu

Complex(dp),Intent(in) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Complex(dp), Intent(in) :: Tad1Loop(2)

Real(dp), Intent(out) :: TadpoleValues(2)

TadpoleValues(1) = Real(0.5_dp*Lam1*vd**3 + vd*(M112 + vu**2*(0.5_dp*Lam3 + 0.5_dp*Lam4 + 0.25_dp*Lam5 + 0.25_dp*Conjg(Lam5)))& 
&  + vd**2*vu*(0.75_dp*Lam6 + 0.75_dp*Conjg(Lam6)) + vu*(-0.5_dp*M12 + vu**2*(0.25_dp*Lam7 + 0.25_dp*Conjg(Lam7))& 
&  - 0.5_dp*Conjg(M12)) - Tad1Loop(1),dp) 
TadpoleValues(2) = Real(0.5_dp*(2._dp*M222*vu + 1._dp*Lam2*vu**3 + vd**2*vu*(Lam3 + Lam4 + 0.5_dp*Lam5 + 0.5_dp*Conjg(Lam5))& 
&  + vd**3*(0.5_dp*Lam6 + 0.5_dp*Conjg(Lam6)) + vd*(-1._dp*M12 + vu**2*(1.5_dp*Lam7 + 1.5_dp*Conjg(Lam7))& 
&  - 1._dp*Conjg(M12))) - Tad1Loop(2),dp) 
End Subroutine CalculateTadpoles 

End Module Tadpoles_THDM_GEN 
 
