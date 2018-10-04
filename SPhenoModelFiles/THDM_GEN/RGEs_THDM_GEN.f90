! -----------------------------------------------------------------------------  
! This file was automatically created by SARAH version 4.13.0 
! SARAH References: arXiv:0806.0538, 0909.2863, 1002.0840, 1207.0906, 1309.7223  
! (c) Florian Staub, 2013  
! ------------------------------------------------------------------------------  
! File created at 15:17 on 22.8.2018   
! ----------------------------------------------------------------------  
 
 
Module RGEs_THDM_GEN 
 
Use Control 
Use Settings 
Use Model_Data_THDM_GEN 
Use Mathematics 
 
Logical,Private,Save::OnlyDiagonal

Real(dp),Parameter::id3R(3,3)=& 
   & Reshape(Source=(/& 
   & 1,0,0,& 
 &0,1,0,& 
 &0,0,1& 
 &/),shape=(/3,3/)) 
Contains 


Subroutine GToParameters131(g,g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,            & 
& eta1U,eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222)

Implicit None 
Real(dp), Intent(in) :: g(131) 
Real(dp),Intent(out) :: g1,g2,g3

Complex(dp),Intent(out) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Integer i1, i2, i3, i4, SumI 
 
Iname = Iname +1 
NameOfUnit(Iname) = 'GToParameters131' 
 
g1= g(1) 
g2= g(2) 
g3= g(3) 
Lam6= Cmplx(g(4),g(5),dp) 
Lam5= Cmplx(g(6),g(7),dp) 
Lam7= Cmplx(g(8),g(9),dp) 
Lam1= Cmplx(g(10),g(11),dp) 
Lam4= Cmplx(g(12),g(13),dp) 
Lam3= Cmplx(g(14),g(15),dp) 
Lam2= Cmplx(g(16),g(17),dp) 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta1U(i1,i2) = Cmplx( g(SumI+18), g(SumI+19), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta2U(i1,i2) = Cmplx( g(SumI+36), g(SumI+37), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta1D(i1,i2) = Cmplx( g(SumI+54), g(SumI+55), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta1L(i1,i2) = Cmplx( g(SumI+72), g(SumI+73), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta2D(i1,i2) = Cmplx( g(SumI+90), g(SumI+91), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta2L(i1,i2) = Cmplx( g(SumI+108), g(SumI+109), dp) 
End Do 
 End Do 
 
M12= Cmplx(g(126),g(127),dp) 
M112= Cmplx(g(128),g(129),dp) 
M222= Cmplx(g(130),g(131),dp) 
Do i1=1,131 
If (g(i1).ne.g(i1)) Then 
 Write(*,*) "NaN appearing in ",NameOfUnit(Iname) 
 Write(*,*) "At position ", i1 
 Call TerminateProgram 
End if 
End do 
Iname = Iname - 1 
 
End Subroutine GToParameters131

Subroutine ParametersToG131(g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,              & 
& eta1U,eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222,g)

Implicit None 
Real(dp), Intent(out) :: g(131) 
Real(dp), Intent(in) :: g1,g2,g3

Complex(dp), Intent(in) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Integer i1, i2, i3, i4, SumI 
 
Iname = Iname +1 
NameOfUnit(Iname) = 'ParametersToG131' 
 
g(1) = g1  
g(2) = g2  
g(3) = g3  
g(4) = Real(Lam6,dp)  
g(5) = Aimag(Lam6)  
g(6) = Real(Lam5,dp)  
g(7) = Aimag(Lam5)  
g(8) = Real(Lam7,dp)  
g(9) = Aimag(Lam7)  
g(10) = Real(Lam1,dp)  
g(11) = Aimag(Lam1)  
g(12) = Real(Lam4,dp)  
g(13) = Aimag(Lam4)  
g(14) = Real(Lam3,dp)  
g(15) = Aimag(Lam3)  
g(16) = Real(Lam2,dp)  
g(17) = Aimag(Lam2)  
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+18) = Real(eta1U(i1,i2), dp) 
g(SumI+19) = Aimag(eta1U(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+36) = Real(eta2U(i1,i2), dp) 
g(SumI+37) = Aimag(eta2U(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+54) = Real(eta1D(i1,i2), dp) 
g(SumI+55) = Aimag(eta1D(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+72) = Real(eta1L(i1,i2), dp) 
g(SumI+73) = Aimag(eta1L(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+90) = Real(eta2D(i1,i2), dp) 
g(SumI+91) = Aimag(eta2D(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+108) = Real(eta2L(i1,i2), dp) 
g(SumI+109) = Aimag(eta2L(i1,i2)) 
End Do 
End Do 

g(126) = Real(M12,dp)  
g(127) = Aimag(M12)  
g(128) = Real(M112,dp)  
g(129) = Aimag(M112)  
g(130) = Real(M222,dp)  
g(131) = Aimag(M222)  
Iname = Iname - 1 
 
End Subroutine ParametersToG131

Subroutine rge131(len, T, GY, F) 
Implicit None 
Integer, Intent(in) :: len 
Real(dp), Intent(in) :: T, GY(len) 
Real(dp), Intent(out) :: F(len) 
Integer :: i1,i2,i3,i4 
Integer :: j1,j2,j3,j4,j5,j6,j7 
Real(dp) :: q 
Real(dp) :: g1,betag11,betag12,Dg1,g2,betag21,betag22,Dg2,g3,betag31,betag32,Dg3
Complex(dp) :: Lam6,betaLam61,betaLam62,DLam6,Lam5,betaLam51,betaLam52,               & 
& DLam5,Lam7,betaLam71,betaLam72,DLam7,Lam1,betaLam11,betaLam12,DLam1,Lam4,              & 
& betaLam41,betaLam42,DLam4,Lam3,betaLam31,betaLam32,DLam3,Lam2,betaLam21,               & 
& betaLam22,DLam2,eta1U(3,3),betaeta1U1(3,3),betaeta1U2(3,3),Deta1U(3,3),adjeta1U(3,3)   & 
& ,eta2U(3,3),betaeta2U1(3,3),betaeta2U2(3,3),Deta2U(3,3),adjeta2U(3,3),eta1D(3,3)       & 
& ,betaeta1D1(3,3),betaeta1D2(3,3),Deta1D(3,3),adjeta1D(3,3),eta1L(3,3),betaeta1L1(3,3)  & 
& ,betaeta1L2(3,3),Deta1L(3,3),adjeta1L(3,3),eta2D(3,3),betaeta2D1(3,3),betaeta2D2(3,3)  & 
& ,Deta2D(3,3),adjeta2D(3,3),eta2L(3,3),betaeta2L1(3,3),betaeta2L2(3,3),Deta2L(3,3)      & 
& ,adjeta2L(3,3),M12,betaM121,betaM122,DM12,M112,betaM1121,betaM1122,DM112,              & 
& M222,betaM2221,betaM2222,DM222
Real(dp) :: AbsLam5,AbsLam7
Complex(dp) :: eta1Dadjeta1D(3,3),eta1Dadjeta2D(3,3),eta1Ladjeta1L(3,3),eta1Ladjeta2L(3,3),          & 
& eta1Uadjeta1U(3,3),eta1Uadjeta2D(3,3),eta1Uadjeta2U(3,3),eta2Dadjeta1D(3,3),           & 
& eta2Dadjeta1U(3,3),eta2Dadjeta2D(3,3),eta2Ladjeta1L(3,3),eta2Ladjeta2L(3,3),           & 
& eta2Uadjeta1D(3,3),eta2Uadjeta1U(3,3),eta2Uadjeta2U(3,3),adjeta1Deta1D(3,3),           & 
& adjeta1Deta2D(3,3),adjeta1Leta1L(3,3),adjeta1Leta2L(3,3),adjeta1Ueta1U(3,3),           & 
& adjeta1Ueta2U(3,3),adjeta2Deta1D(3,3),adjeta2Deta2D(3,3),adjeta2Leta1L(3,3),           & 
& adjeta2Leta2L(3,3),adjeta2Ueta1U(3,3),adjeta2Ueta2U(3,3),eta1Dadjeta1Deta1D(3,3),      & 
& eta1Dadjeta1Deta2D(3,3),eta1Dadjeta1Ueta1U(3,3),eta1Dadjeta2Deta2D(3,3),               & 
& eta1Dadjeta2Ueta1U(3,3),eta1Dadjeta2Ueta2U(3,3),eta1Ladjeta1Leta1L(3,3),               & 
& eta1Ladjeta1Leta2L(3,3),eta1Ladjeta2Leta2L(3,3),eta1Uadjeta1Deta1D(3,3),               & 
& eta1Uadjeta1Ueta1U(3,3),eta1Uadjeta1Ueta2U(3,3),eta1Uadjeta2Deta1D(3,3),               & 
& eta1Uadjeta2Deta2D(3,3),eta1Uadjeta2Ueta2U(3,3),eta2Dadjeta1Deta1D(3,3),               & 
& eta2Dadjeta1Ueta1U(3,3),eta2Dadjeta1Ueta2U(3,3),eta2Dadjeta2Deta1D(3,3),               & 
& eta2Dadjeta2Deta2D(3,3),eta2Dadjeta2Ueta2U(3,3),eta2Ladjeta1Leta1L(3,3),               & 
& eta2Ladjeta2Leta1L(3,3),eta2Ladjeta2Leta2L(3,3),eta2Uadjeta1Deta1D(3,3),               & 
& eta2Uadjeta1Deta2D(3,3),eta2Uadjeta1Ueta1U(3,3),eta2Uadjeta2Deta2D(3,3),               & 
& eta2Uadjeta2Ueta1U(3,3),eta2Uadjeta2Ueta2U(3,3),adjeta1Deta1Dadjeta1D(3,3),            & 
& adjeta1Deta1Dadjeta2D(3,3),adjeta1Deta2Dadjeta1D(3,3),adjeta1Deta2Dadjeta2D(3,3),      & 
& adjeta1Leta1Ladjeta1L(3,3),adjeta1Leta1Ladjeta2L(3,3),adjeta1Leta2Ladjeta1L(3,3),      & 
& adjeta1Leta2Ladjeta2L(3,3),adjeta1Ueta1Uadjeta1U(3,3),adjeta1Ueta2Uadjeta1D(3,3),      & 
& adjeta1Ueta2Uadjeta1U(3,3),adjeta1Ueta2Uadjeta2U(3,3),adjeta2Deta1Dadjeta1D(3,3),      & 
& adjeta2Deta2Dadjeta1D(3,3),adjeta2Deta2Dadjeta1U(3,3),adjeta2Deta2Dadjeta2D(3,3),      & 
& adjeta2Leta1Ladjeta1L(3,3),adjeta2Leta2Ladjeta1L(3,3),adjeta2Leta2Ladjeta2L(3,3),      & 
& adjeta2Ueta1Uadjeta2D(3,3),adjeta2Ueta1Uadjeta2U(3,3),adjeta2Ueta2Uadjeta1D(3,3),      & 
& adjeta2Ueta2Uadjeta1U(3,3),adjeta2Ueta2Uadjeta2U(3,3),eta1Dadjeta1Deta1Dadjeta1D(3,3), & 
& eta1Dadjeta1Deta1Dadjeta2D(3,3),eta1Dadjeta1Deta2Dadjeta2D(3,3),eta1Dadjeta2Deta1Dadjeta1D(3,3),& 
& eta1Dadjeta2Deta2Dadjeta1D(3,3),eta1Dadjeta2Deta2Dadjeta2D(3,3),eta1Dadjeta2Ueta1Uadjeta2D(3,3),& 
& eta1Dadjeta2Ueta2Uadjeta1D(3,3),eta1Ladjeta1Leta1Ladjeta1L(3,3),eta1Ladjeta1Leta1Ladjeta2L(3,3),& 
& eta1Ladjeta1Leta2Ladjeta2L(3,3),eta1Ladjeta2Leta1Ladjeta1L(3,3),eta1Ladjeta2Leta2Ladjeta1L(3,3),& 
& eta1Ladjeta2Leta2Ladjeta2L(3,3),eta1Uadjeta1Ueta1Uadjeta1U(3,3),eta1Uadjeta1Ueta2Uadjeta1U(3,3),& 
& eta1Uadjeta1Ueta2Uadjeta2U(3,3),eta1Uadjeta2Deta2Dadjeta1U(3,3),eta1Uadjeta2Ueta1Uadjeta2U(3,3),& 
& eta1Uadjeta2Ueta2Uadjeta1U(3,3),eta2Dadjeta1Deta2Dadjeta1D(3,3),eta2Dadjeta1Ueta2Uadjeta1D(3,3),& 
& eta2Dadjeta2Deta2Dadjeta2D(3,3),eta2Ladjeta1Leta2Ladjeta1L(3,3),eta2Ladjeta2Leta2Ladjeta2L(3,3),& 
& eta2Uadjeta1Ueta2Uadjeta2U(3,3),eta2Uadjeta2Ueta2Uadjeta1U(3,3),eta2Uadjeta2Ueta2Uadjeta2U(3,3)

Complex(dp) :: Treta1Dadjeta1D,Treta1Dadjeta2D,Treta1Ladjeta1L,Treta1Ladjeta2L,Treta1Uadjeta1U,      & 
& Treta1Uadjeta2U,Treta2Dadjeta1D,Treta2Dadjeta2D,Treta2Ladjeta1L,Treta2Ladjeta2L,       & 
& Treta2Uadjeta1U,Treta2Uadjeta2U,Treta1Dadjeta1Deta1Dadjeta1D,Treta1Dadjeta1Deta1Dadjeta2D,& 
& Treta1Dadjeta1Deta2Dadjeta2D,Treta1Dadjeta2Deta1Dadjeta1D,Treta1Dadjeta2Deta2Dadjeta1D,& 
& Treta1Dadjeta2Deta2Dadjeta2D,Treta1Dadjeta2Ueta1Uadjeta2D,Treta1Dadjeta2Ueta2Uadjeta1D,& 
& Treta1Ladjeta1Leta1Ladjeta1L,Treta1Ladjeta1Leta1Ladjeta2L,Treta1Ladjeta1Leta2Ladjeta2L,& 
& Treta1Ladjeta2Leta1Ladjeta1L,Treta1Ladjeta2Leta2Ladjeta1L,Treta1Ladjeta2Leta2Ladjeta2L,& 
& Treta1Uadjeta1Ueta1Uadjeta1U,Treta1Uadjeta1Ueta2Uadjeta1U,Treta1Uadjeta1Ueta2Uadjeta2U,& 
& Treta1Uadjeta2Deta2Dadjeta1U,Treta1Uadjeta2Ueta1Uadjeta2U,Treta1Uadjeta2Ueta2Uadjeta1U,& 
& Treta2Dadjeta1Deta2Dadjeta1D,Treta2Dadjeta1Ueta2Uadjeta1D,Treta2Dadjeta2Deta2Dadjeta2D,& 
& Treta2Ladjeta1Leta2Ladjeta1L,Treta2Ladjeta2Leta2Ladjeta2L,Treta2Uadjeta1Ueta2Uadjeta2U,& 
& Treta2Uadjeta2Ueta2Uadjeta1U,Treta2Uadjeta2Ueta2Uadjeta2U

Real(dp) :: g1p2,g1p3,g1p4,g2p2,g2p3,g2p4,g3p2,g3p3

Complex(dp) :: Lam1p2,Lam2p2,Lam3p2,Lam4p2,CLam6p2,CLam7p2

Iname = Iname +1 
NameOfUnit(Iname) = 'rge131' 
 
OnlyDiagonal = .Not.GenerationMixing 
q = t 
 
Call GToParameters131(gy,g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U,           & 
& eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222)

AbsLam5 = Conjg(Lam5)*Lam5
AbsLam7 = Conjg(Lam7)*Lam7
Call Adjungate(eta1U,adjeta1U)
Call Adjungate(eta2U,adjeta2U)
Call Adjungate(eta1D,adjeta1D)
Call Adjungate(eta1L,adjeta1L)
Call Adjungate(eta2D,adjeta2D)
Call Adjungate(eta2L,adjeta2L)
 eta1Dadjeta1D = Matmul(eta1D,adjeta1D) 
Forall(i2=1:3)  eta1Dadjeta1D(i2,i2) =  Real(eta1Dadjeta1D(i2,i2),dp) 
 eta1Dadjeta2D = Matmul(eta1D,adjeta2D) 
 eta1Ladjeta1L = Matmul(eta1L,adjeta1L) 
Forall(i2=1:3)  eta1Ladjeta1L(i2,i2) =  Real(eta1Ladjeta1L(i2,i2),dp) 
 eta1Ladjeta2L = Matmul(eta1L,adjeta2L) 
 eta1Uadjeta1U = Matmul(eta1U,adjeta1U) 
Forall(i2=1:3)  eta1Uadjeta1U(i2,i2) =  Real(eta1Uadjeta1U(i2,i2),dp) 
 eta1Uadjeta2D = Matmul(eta1U,adjeta2D) 
 eta1Uadjeta2U = Matmul(eta1U,adjeta2U) 
 eta2Dadjeta1D = Matmul(eta2D,adjeta1D) 
 eta2Dadjeta1U = Matmul(eta2D,adjeta1U) 
 eta2Dadjeta2D = Matmul(eta2D,adjeta2D) 
Forall(i2=1:3)  eta2Dadjeta2D(i2,i2) =  Real(eta2Dadjeta2D(i2,i2),dp) 
 eta2Ladjeta1L = Matmul(eta2L,adjeta1L) 
 eta2Ladjeta2L = Matmul(eta2L,adjeta2L) 
Forall(i2=1:3)  eta2Ladjeta2L(i2,i2) =  Real(eta2Ladjeta2L(i2,i2),dp) 
 eta2Uadjeta1D = Matmul(eta2U,adjeta1D) 
 eta2Uadjeta1U = Matmul(eta2U,adjeta1U) 
 eta2Uadjeta2U = Matmul(eta2U,adjeta2U) 
Forall(i2=1:3)  eta2Uadjeta2U(i2,i2) =  Real(eta2Uadjeta2U(i2,i2),dp) 
 adjeta1Deta1D = Matmul(adjeta1D,eta1D) 
Forall(i2=1:3)  adjeta1Deta1D(i2,i2) =  Real(adjeta1Deta1D(i2,i2),dp) 
 adjeta1Deta2D = Matmul(adjeta1D,eta2D) 
 adjeta1Leta1L = Matmul(adjeta1L,eta1L) 
Forall(i2=1:3)  adjeta1Leta1L(i2,i2) =  Real(adjeta1Leta1L(i2,i2),dp) 
 adjeta1Leta2L = Matmul(adjeta1L,eta2L) 
 adjeta1Ueta1U = Matmul(adjeta1U,eta1U) 
Forall(i2=1:3)  adjeta1Ueta1U(i2,i2) =  Real(adjeta1Ueta1U(i2,i2),dp) 
 adjeta1Ueta2U = Matmul(adjeta1U,eta2U) 
 adjeta2Deta1D = Matmul(adjeta2D,eta1D) 
 adjeta2Deta2D = Matmul(adjeta2D,eta2D) 
Forall(i2=1:3)  adjeta2Deta2D(i2,i2) =  Real(adjeta2Deta2D(i2,i2),dp) 
 adjeta2Leta1L = Matmul(adjeta2L,eta1L) 
 adjeta2Leta2L = Matmul(adjeta2L,eta2L) 
Forall(i2=1:3)  adjeta2Leta2L(i2,i2) =  Real(adjeta2Leta2L(i2,i2),dp) 
 adjeta2Ueta1U = Matmul(adjeta2U,eta1U) 
 adjeta2Ueta2U = Matmul(adjeta2U,eta2U) 
Forall(i2=1:3)  adjeta2Ueta2U(i2,i2) =  Real(adjeta2Ueta2U(i2,i2),dp) 
 eta1Dadjeta1Deta1D = Matmul(eta1D,adjeta1Deta1D) 
 eta1Dadjeta1Deta2D = Matmul(eta1D,adjeta1Deta2D) 
 eta1Dadjeta1Ueta1U = Matmul(eta1D,adjeta1Ueta1U) 
 eta1Dadjeta2Deta2D = Matmul(eta1D,adjeta2Deta2D) 
 eta1Dadjeta2Ueta1U = Matmul(eta1D,adjeta2Ueta1U) 
 eta1Dadjeta2Ueta2U = Matmul(eta1D,adjeta2Ueta2U) 
 eta1Ladjeta1Leta1L = Matmul(eta1L,adjeta1Leta1L) 
 eta1Ladjeta1Leta2L = Matmul(eta1L,adjeta1Leta2L) 
 eta1Ladjeta2Leta2L = Matmul(eta1L,adjeta2Leta2L) 
 eta1Uadjeta1Deta1D = Matmul(eta1U,adjeta1Deta1D) 
 eta1Uadjeta1Ueta1U = Matmul(eta1U,adjeta1Ueta1U) 
 eta1Uadjeta1Ueta2U = Matmul(eta1U,adjeta1Ueta2U) 
 eta1Uadjeta2Deta1D = Matmul(eta1U,adjeta2Deta1D) 
 eta1Uadjeta2Deta2D = Matmul(eta1U,adjeta2Deta2D) 
 eta1Uadjeta2Ueta2U = Matmul(eta1U,adjeta2Ueta2U) 
 eta2Dadjeta1Deta1D = Matmul(eta2D,adjeta1Deta1D) 
 eta2Dadjeta1Ueta1U = Matmul(eta2D,adjeta1Ueta1U) 
 eta2Dadjeta1Ueta2U = Matmul(eta2D,adjeta1Ueta2U) 
 eta2Dadjeta2Deta1D = Matmul(eta2D,adjeta2Deta1D) 
 eta2Dadjeta2Deta2D = Matmul(eta2D,adjeta2Deta2D) 
 eta2Dadjeta2Ueta2U = Matmul(eta2D,adjeta2Ueta2U) 
 eta2Ladjeta1Leta1L = Matmul(eta2L,adjeta1Leta1L) 
 eta2Ladjeta2Leta1L = Matmul(eta2L,adjeta2Leta1L) 
 eta2Ladjeta2Leta2L = Matmul(eta2L,adjeta2Leta2L) 
 eta2Uadjeta1Deta1D = Matmul(eta2U,adjeta1Deta1D) 
 eta2Uadjeta1Deta2D = Matmul(eta2U,adjeta1Deta2D) 
 eta2Uadjeta1Ueta1U = Matmul(eta2U,adjeta1Ueta1U) 
 eta2Uadjeta2Deta2D = Matmul(eta2U,adjeta2Deta2D) 
 eta2Uadjeta2Ueta1U = Matmul(eta2U,adjeta2Ueta1U) 
 eta2Uadjeta2Ueta2U = Matmul(eta2U,adjeta2Ueta2U) 
 adjeta1Deta1Dadjeta1D = Matmul(adjeta1D,eta1Dadjeta1D) 
 adjeta1Deta1Dadjeta2D = Matmul(adjeta1D,eta1Dadjeta2D) 
 adjeta1Deta2Dadjeta1D = Matmul(adjeta1D,eta2Dadjeta1D) 
 adjeta1Deta2Dadjeta2D = Matmul(adjeta1D,eta2Dadjeta2D) 
 adjeta1Leta1Ladjeta1L = Matmul(adjeta1L,eta1Ladjeta1L) 
 adjeta1Leta1Ladjeta2L = Matmul(adjeta1L,eta1Ladjeta2L) 
 adjeta1Leta2Ladjeta1L = Matmul(adjeta1L,eta2Ladjeta1L) 
 adjeta1Leta2Ladjeta2L = Matmul(adjeta1L,eta2Ladjeta2L) 
 adjeta1Ueta1Uadjeta1U = Matmul(adjeta1U,eta1Uadjeta1U) 
 adjeta1Ueta2Uadjeta1D = Matmul(adjeta1U,eta2Uadjeta1D) 
 adjeta1Ueta2Uadjeta1U = Matmul(adjeta1U,eta2Uadjeta1U) 
 adjeta1Ueta2Uadjeta2U = Matmul(adjeta1U,eta2Uadjeta2U) 
 adjeta2Deta1Dadjeta1D = Matmul(adjeta2D,eta1Dadjeta1D) 
 adjeta2Deta2Dadjeta1D = Matmul(adjeta2D,eta2Dadjeta1D) 
 adjeta2Deta2Dadjeta1U = Matmul(adjeta2D,eta2Dadjeta1U) 
 adjeta2Deta2Dadjeta2D = Matmul(adjeta2D,eta2Dadjeta2D) 
 adjeta2Leta1Ladjeta1L = Matmul(adjeta2L,eta1Ladjeta1L) 
 adjeta2Leta2Ladjeta1L = Matmul(adjeta2L,eta2Ladjeta1L) 
 adjeta2Leta2Ladjeta2L = Matmul(adjeta2L,eta2Ladjeta2L) 
 adjeta2Ueta1Uadjeta2D = Matmul(adjeta2U,eta1Uadjeta2D) 
 adjeta2Ueta1Uadjeta2U = Matmul(adjeta2U,eta1Uadjeta2U) 
 adjeta2Ueta2Uadjeta1D = Matmul(adjeta2U,eta2Uadjeta1D) 
 adjeta2Ueta2Uadjeta1U = Matmul(adjeta2U,eta2Uadjeta1U) 
 adjeta2Ueta2Uadjeta2U = Matmul(adjeta2U,eta2Uadjeta2U) 
 eta1Dadjeta1Deta1Dadjeta1D = Matmul(eta1D,adjeta1Deta1Dadjeta1D) 
Forall(i2=1:3)  eta1Dadjeta1Deta1Dadjeta1D(i2,i2) =  Real(eta1Dadjeta1Deta1Dadjeta1D(i2,i2),dp) 
 eta1Dadjeta1Deta1Dadjeta2D = Matmul(eta1D,adjeta1Deta1Dadjeta2D) 
 eta1Dadjeta1Deta2Dadjeta2D = Matmul(eta1D,adjeta1Deta2Dadjeta2D) 
 eta1Dadjeta2Deta1Dadjeta1D = Matmul(eta1D,adjeta2Deta1Dadjeta1D) 
 eta1Dadjeta2Deta2Dadjeta1D = Matmul(eta1D,adjeta2Deta2Dadjeta1D) 
Forall(i2=1:3)  eta1Dadjeta2Deta2Dadjeta1D(i2,i2) =  Real(eta1Dadjeta2Deta2Dadjeta1D(i2,i2),dp) 
 eta1Dadjeta2Deta2Dadjeta2D = Matmul(eta1D,adjeta2Deta2Dadjeta2D) 
 eta1Dadjeta2Ueta1Uadjeta2D = Matmul(eta1D,adjeta2Ueta1Uadjeta2D) 
 eta1Dadjeta2Ueta2Uadjeta1D = Matmul(eta1D,adjeta2Ueta2Uadjeta1D) 
Forall(i2=1:3)  eta1Dadjeta2Ueta2Uadjeta1D(i2,i2) =  Real(eta1Dadjeta2Ueta2Uadjeta1D(i2,i2),dp) 
 eta1Ladjeta1Leta1Ladjeta1L = Matmul(eta1L,adjeta1Leta1Ladjeta1L) 
Forall(i2=1:3)  eta1Ladjeta1Leta1Ladjeta1L(i2,i2) =  Real(eta1Ladjeta1Leta1Ladjeta1L(i2,i2),dp) 
 eta1Ladjeta1Leta1Ladjeta2L = Matmul(eta1L,adjeta1Leta1Ladjeta2L) 
 eta1Ladjeta1Leta2Ladjeta2L = Matmul(eta1L,adjeta1Leta2Ladjeta2L) 
 eta1Ladjeta2Leta1Ladjeta1L = Matmul(eta1L,adjeta2Leta1Ladjeta1L) 
 eta1Ladjeta2Leta2Ladjeta1L = Matmul(eta1L,adjeta2Leta2Ladjeta1L) 
Forall(i2=1:3)  eta1Ladjeta2Leta2Ladjeta1L(i2,i2) =  Real(eta1Ladjeta2Leta2Ladjeta1L(i2,i2),dp) 
 eta1Ladjeta2Leta2Ladjeta2L = Matmul(eta1L,adjeta2Leta2Ladjeta2L) 
 eta1Uadjeta1Ueta1Uadjeta1U = Matmul(eta1U,adjeta1Ueta1Uadjeta1U) 
Forall(i2=1:3)  eta1Uadjeta1Ueta1Uadjeta1U(i2,i2) =  Real(eta1Uadjeta1Ueta1Uadjeta1U(i2,i2),dp) 
 eta1Uadjeta1Ueta2Uadjeta1U = Matmul(eta1U,adjeta1Ueta2Uadjeta1U) 
 eta1Uadjeta1Ueta2Uadjeta2U = Matmul(eta1U,adjeta1Ueta2Uadjeta2U) 
 eta1Uadjeta2Deta2Dadjeta1U = Matmul(eta1U,adjeta2Deta2Dadjeta1U) 
Forall(i2=1:3)  eta1Uadjeta2Deta2Dadjeta1U(i2,i2) =  Real(eta1Uadjeta2Deta2Dadjeta1U(i2,i2),dp) 
 eta1Uadjeta2Ueta1Uadjeta2U = Matmul(eta1U,adjeta2Ueta1Uadjeta2U) 
 eta1Uadjeta2Ueta2Uadjeta1U = Matmul(eta1U,adjeta2Ueta2Uadjeta1U) 
Forall(i2=1:3)  eta1Uadjeta2Ueta2Uadjeta1U(i2,i2) =  Real(eta1Uadjeta2Ueta2Uadjeta1U(i2,i2),dp) 
 eta2Dadjeta1Deta2Dadjeta1D = Matmul(eta2D,adjeta1Deta2Dadjeta1D) 
 eta2Dadjeta1Ueta2Uadjeta1D = Matmul(eta2D,adjeta1Ueta2Uadjeta1D) 
 eta2Dadjeta2Deta2Dadjeta2D = Matmul(eta2D,adjeta2Deta2Dadjeta2D) 
Forall(i2=1:3)  eta2Dadjeta2Deta2Dadjeta2D(i2,i2) =  Real(eta2Dadjeta2Deta2Dadjeta2D(i2,i2),dp) 
 eta2Ladjeta1Leta2Ladjeta1L = Matmul(eta2L,adjeta1Leta2Ladjeta1L) 
 eta2Ladjeta2Leta2Ladjeta2L = Matmul(eta2L,adjeta2Leta2Ladjeta2L) 
Forall(i2=1:3)  eta2Ladjeta2Leta2Ladjeta2L(i2,i2) =  Real(eta2Ladjeta2Leta2Ladjeta2L(i2,i2),dp) 
 eta2Uadjeta1Ueta2Uadjeta2U = Matmul(eta2U,adjeta1Ueta2Uadjeta2U) 
 eta2Uadjeta2Ueta2Uadjeta1U = Matmul(eta2U,adjeta2Ueta2Uadjeta1U) 
 eta2Uadjeta2Ueta2Uadjeta2U = Matmul(eta2U,adjeta2Ueta2Uadjeta2U) 
Forall(i2=1:3)  eta2Uadjeta2Ueta2Uadjeta2U(i2,i2) =  Real(eta2Uadjeta2Ueta2Uadjeta2U(i2,i2),dp) 
 Treta1Dadjeta1D = Real(cTrace(eta1Dadjeta1D),dp) 
 Treta1Dadjeta2D = cTrace(eta1Dadjeta2D) 
 Treta1Ladjeta1L = Real(cTrace(eta1Ladjeta1L),dp) 
 Treta1Ladjeta2L = cTrace(eta1Ladjeta2L) 
 Treta1Uadjeta1U = Real(cTrace(eta1Uadjeta1U),dp) 
 Treta1Uadjeta2U = cTrace(eta1Uadjeta2U) 
 Treta2Dadjeta1D = cTrace(eta2Dadjeta1D) 
 Treta2Dadjeta2D = Real(cTrace(eta2Dadjeta2D),dp) 
 Treta2Ladjeta1L = cTrace(eta2Ladjeta1L) 
 Treta2Ladjeta2L = Real(cTrace(eta2Ladjeta2L),dp) 
 Treta2Uadjeta1U = cTrace(eta2Uadjeta1U) 
 Treta2Uadjeta2U = Real(cTrace(eta2Uadjeta2U),dp) 
 Treta1Dadjeta1Deta1Dadjeta1D = Real(cTrace(eta1Dadjeta1Deta1Dadjeta1D),dp) 
 Treta1Dadjeta1Deta1Dadjeta2D = cTrace(eta1Dadjeta1Deta1Dadjeta2D) 
 Treta1Dadjeta1Deta2Dadjeta2D = cTrace(eta1Dadjeta1Deta2Dadjeta2D) 
 Treta1Dadjeta2Deta1Dadjeta1D = cTrace(eta1Dadjeta2Deta1Dadjeta1D) 
 Treta1Dadjeta2Deta2Dadjeta1D = Real(cTrace(eta1Dadjeta2Deta2Dadjeta1D),dp) 
 Treta1Dadjeta2Deta2Dadjeta2D = cTrace(eta1Dadjeta2Deta2Dadjeta2D) 
 Treta1Dadjeta2Ueta1Uadjeta2D = cTrace(eta1Dadjeta2Ueta1Uadjeta2D) 
 Treta1Dadjeta2Ueta2Uadjeta1D = Real(cTrace(eta1Dadjeta2Ueta2Uadjeta1D),dp) 
 Treta1Ladjeta1Leta1Ladjeta1L = Real(cTrace(eta1Ladjeta1Leta1Ladjeta1L),dp) 
 Treta1Ladjeta1Leta1Ladjeta2L = cTrace(eta1Ladjeta1Leta1Ladjeta2L) 
 Treta1Ladjeta1Leta2Ladjeta2L = cTrace(eta1Ladjeta1Leta2Ladjeta2L) 
 Treta1Ladjeta2Leta1Ladjeta1L = cTrace(eta1Ladjeta2Leta1Ladjeta1L) 
 Treta1Ladjeta2Leta2Ladjeta1L = Real(cTrace(eta1Ladjeta2Leta2Ladjeta1L),dp) 
 Treta1Ladjeta2Leta2Ladjeta2L = cTrace(eta1Ladjeta2Leta2Ladjeta2L) 
 Treta1Uadjeta1Ueta1Uadjeta1U = Real(cTrace(eta1Uadjeta1Ueta1Uadjeta1U),dp) 
 Treta1Uadjeta1Ueta2Uadjeta1U = cTrace(eta1Uadjeta1Ueta2Uadjeta1U) 
 Treta1Uadjeta1Ueta2Uadjeta2U = cTrace(eta1Uadjeta1Ueta2Uadjeta2U) 
 Treta1Uadjeta2Deta2Dadjeta1U = Real(cTrace(eta1Uadjeta2Deta2Dadjeta1U),dp) 
 Treta1Uadjeta2Ueta1Uadjeta2U = cTrace(eta1Uadjeta2Ueta1Uadjeta2U) 
 Treta1Uadjeta2Ueta2Uadjeta1U = Real(cTrace(eta1Uadjeta2Ueta2Uadjeta1U),dp) 
 Treta2Dadjeta1Deta2Dadjeta1D = cTrace(eta2Dadjeta1Deta2Dadjeta1D) 
 Treta2Dadjeta1Ueta2Uadjeta1D = cTrace(eta2Dadjeta1Ueta2Uadjeta1D) 
 Treta2Dadjeta2Deta2Dadjeta2D = Real(cTrace(eta2Dadjeta2Deta2Dadjeta2D),dp) 
 Treta2Ladjeta1Leta2Ladjeta1L = cTrace(eta2Ladjeta1Leta2Ladjeta1L) 
 Treta2Ladjeta2Leta2Ladjeta2L = Real(cTrace(eta2Ladjeta2Leta2Ladjeta2L),dp) 
 Treta2Uadjeta1Ueta2Uadjeta2U = cTrace(eta2Uadjeta1Ueta2Uadjeta2U) 
 Treta2Uadjeta2Ueta2Uadjeta1U = cTrace(eta2Uadjeta2Ueta2Uadjeta1U) 
 Treta2Uadjeta2Ueta2Uadjeta2U = Real(cTrace(eta2Uadjeta2Ueta2Uadjeta2U),dp) 
 g1p2 =g1**2 
 g1p3 =g1**3 
 g1p4 =g1**4 
 g2p2 =g2**2 
 g2p3 =g2**3 
 g2p4 =g2**4 
 g3p2 =g3**2 
 g3p3 =g3**3 
 Lam1p2 =Lam1**2 
 Lam2p2 =Lam2**2 
 Lam3p2 =Lam3**2 
 Lam4p2 =Lam4**2 
 CLam6p2 =Conjg(Lam6)**2 
 CLam7p2 =Conjg(Lam7)**2 


If (TwoLoopRGE) Then 
End If 
 
 
!-------------------- 
! g1 
!-------------------- 
 
betag11  = 7._dp*(g1p3)

 
 
If (TwoLoopRGE) Then 
betag12 = 0

 
Dg1 = oo16pi2*( betag11 + oo16pi2 * betag12 ) 

 
Else 
Dg1 = oo16pi2* betag11 
End If 
 
 
!-------------------- 
! g2 
!-------------------- 
 
betag21  = -3._dp*(g2p3)

 
 
If (TwoLoopRGE) Then 
betag22 = 0

 
Dg2 = oo16pi2*( betag21 + oo16pi2 * betag22 ) 

 
Else 
Dg2 = oo16pi2* betag21 
End If 
 
 
!-------------------- 
! g3 
!-------------------- 
 
betag31  = -7._dp*(g3p3)

 
 
If (TwoLoopRGE) Then 
betag32 = 0

 
Dg3 = oo16pi2*( betag31 + oo16pi2 * betag32 ) 

 
Else 
Dg3 = oo16pi2* betag31 
End If 
 
 
!-------------------- 
! Lam6 
!-------------------- 
 
betaLam61  = -3*g1p2*Lam6 - 9*g2p2*Lam6 + 12._dp*Lam1*Lam6 + 6*Lam3*Lam6 +            & 
&  8*Lam4*Lam6 + 6*Lam3*Lam7 + 4*Lam4*Lam7 + 9*Lam6*Treta1Dadjeta1D - 6._dp*(Treta1Dadjeta1Deta1Dadjeta2D)& 
&  + 3._dp*Lam1*Treta1Dadjeta2D + 3*Lam3*Treta1Dadjeta2D + 3*Lam4*Treta1Dadjeta2D -      & 
&  6._dp*(Treta1Dadjeta2Deta1Dadjeta1D) + 3*Lam6*Treta1Ladjeta1L - 2._dp*(Treta1Ladjeta1Leta1Ladjeta2L)& 
&  + 1._dp*Lam1*Treta1Ladjeta2L + Lam3*Treta1Ladjeta2L + Lam4*Treta1Ladjeta2L -          & 
&  2._dp*(Treta1Ladjeta2Leta1Ladjeta1L) + 9*Lam6*Treta1Uadjeta1U - 12._dp*(Treta1Uadjeta1Ueta2Uadjeta1U)& 
&  + 3*Lam6*Treta2Dadjeta2D + Lam6*Treta2Ladjeta2L + 3._dp*Lam1*Treta2Uadjeta1U +        & 
&  3*Lam3*Treta2Uadjeta1U + 3*Lam4*Treta2Uadjeta1U + 3*Lam6*Treta2Uadjeta2U +            & 
&  Conjg(Lam5)*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L +    & 
&  10*Conjg(Lam6) + 2*Conjg(Lam7))

 
 
If (TwoLoopRGE) Then 
betaLam62 = 0

 
DLam6 = oo16pi2*( betaLam61 + oo16pi2 * betaLam62 ) 

 
Else 
DLam6 = oo16pi2* betaLam61 
End If 
 
 
Call Chop(DLam6) 

!-------------------- 
! Lam5 
!-------------------- 
 
betaLam51  = 10._dp*(CLam6p2) + 10._dp*(CLam7p2) - 3*g1p2*Lam5 - 9*g2p2*Lam5 +        & 
&  2._dp*Lam1*Lam5 + 2._dp*Lam2*Lam5 + 8*Lam3*Lam5 + 12*Lam4*Lam5 + 6*Lam5*Treta1Dadjeta1D +& 
&  2*Lam5*Treta1Ladjeta1L + 6*Lam5*Treta1Uadjeta1U - 12._dp*(Treta1Uadjeta2Ueta1Uadjeta2U)& 
&  - 12._dp*(Treta2Dadjeta1Deta2Dadjeta1D) + 6*Lam5*Treta2Dadjeta2D - 4._dp*(Treta2Ladjeta1Leta2Ladjeta1L)& 
&  + 2*Lam5*Treta2Ladjeta2L + 6*Lam5*Treta2Uadjeta2U + 2*(3._dp*(Treta1Uadjeta2U)        & 
&  + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L)*Conjg(Lam7) + 2*Conjg(Lam6)              & 
& *(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L + 2*Conjg(Lam7))

 
 
If (TwoLoopRGE) Then 
betaLam52 = 0

 
DLam5 = oo16pi2*( betaLam51 + oo16pi2 * betaLam52 ) 

 
Else 
DLam5 = oo16pi2* betaLam51 
End If 
 
 
Call Chop(DLam5) 

!-------------------- 
! Lam7 
!-------------------- 
 
betaLam71  = 6*Lam3*Lam6 + 4*Lam4*Lam6 - 3*g1p2*Lam7 - 9*g2p2*Lam7 + 12._dp*Lam2*Lam7 +& 
&  6*Lam3*Lam7 + 8*Lam4*Lam7 + 3*Lam7*Treta1Dadjeta1D + 3._dp*Lam2*Treta1Dadjeta2D +     & 
&  3*Lam3*Treta1Dadjeta2D + 3*Lam4*Treta1Dadjeta2D - 12._dp*(Treta1Dadjeta2Deta2Dadjeta2D)& 
&  + Lam7*Treta1Ladjeta1L + 1._dp*Lam2*Treta1Ladjeta2L + Lam3*Treta1Ladjeta2L +          & 
&  Lam4*Treta1Ladjeta2L - 4._dp*(Treta1Ladjeta2Leta2Ladjeta2L) + 3*Lam7*Treta1Uadjeta1U +& 
&  9*Lam7*Treta2Dadjeta2D + 0.0000000000000000555112_dp*Lam2*Treta2Ladjeta1L +           & 
&  3*Lam7*Treta2Ladjeta2L + 3._dp*Lam2*Treta2Uadjeta1U + 3*Lam3*Treta2Uadjeta1U +        & 
&  3*Lam4*Treta2Uadjeta1U - 6._dp*(Treta2Uadjeta1Ueta2Uadjeta2U) + 9*Lam7*Treta2Uadjeta2U -& 
&  6._dp*(Treta2Uadjeta2Ueta2Uadjeta1U) + Conjg(Lam5)*(3._dp*(Treta1Uadjeta2U)           & 
&  + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L + 2*Conjg(Lam6) + 10*Conjg(Lam7))

 
 
If (TwoLoopRGE) Then 
betaLam72 = 0

 
DLam7 = oo16pi2*( betaLam71 + oo16pi2 * betaLam72 ) 

 
Else 
DLam7 = oo16pi2* betaLam71 
End If 
 
 
Call Chop(DLam7) 

!-------------------- 
! Lam1 
!-------------------- 
 
betaLam11  = 2._dp*AbsLam5 + 0.75_dp*g1p4 + 1.5_dp*g1p2*g2p2 + 2.25_dp*g2p4 +         & 
&  12._dp*Lam1p2 + 4._dp*Lam3p2 + 2._dp*Lam4p2 - 3._dp*g1p2*Lam1 - 9._dp*g2p2*Lam1 +     & 
&  4._dp*Lam3*Lam4 + 12._dp*Lam1*Treta1Dadjeta1D - 12._dp*Treta1Dadjeta1Deta1Dadjeta1D + & 
&  4._dp*Lam1*Treta1Ladjeta1L - 4._dp*Treta1Ladjeta1Leta1Ladjeta1L + 12._dp*Lam1*Treta1Uadjeta1U -& 
&  12._dp*Treta1Uadjeta1Ueta1Uadjeta1U + 6._dp*Lam6*Treta1Uadjeta2U + 6._dp*Lam6*Treta2Dadjeta1D +& 
&  2._dp*Lam6*Treta2Ladjeta1L + (24._dp*Lam6 + 6._dp*Treta1Dadjeta2D + 2._dp*Treta1Ladjeta2L +& 
&  6._dp*Treta2Uadjeta1U)*Conjg(Lam6)

 
 
If (TwoLoopRGE) Then 
betaLam12 = 0

 
DLam1 = oo16pi2*( betaLam11 + oo16pi2 * betaLam12 ) 

 
Else 
DLam1 = oo16pi2* betaLam11 
End If 
 
 
Call Chop(DLam1) 

!-------------------- 
! Lam4 
!-------------------- 
 
betaLam41  = 8._dp*(AbsLam5) + 10._dp*(AbsLam7) + 3*g1p2*g2p2 + 4._dp*(Lam4p2)        & 
&  - 3*g1p2*Lam4 - 9*g2p2*Lam4 + 2._dp*Lam1*Lam4 + 2._dp*Lam2*Lam4 + 8*Lam3*Lam4 +       & 
&  6*Lam4*Treta1Dadjeta1D - 12._dp*(Treta1Dadjeta2Deta2Dadjeta1D) - 12._dp*(Treta1Dadjeta2Ueta1Uadjeta2D)& 
&  + 12._dp*(Treta1Dadjeta2Ueta2Uadjeta1D) + 2*Lam4*Treta1Ladjeta1L - 4._dp*(Treta1Ladjeta2Leta2Ladjeta1L)& 
&  + 6*Lam4*Treta1Uadjeta1U + 12._dp*(Treta1Uadjeta2Deta2Dadjeta1U) + 3*Lam6*Treta1Uadjeta2U +& 
&  3*Lam7*Treta1Uadjeta2U - 12._dp*(Treta1Uadjeta2Ueta2Uadjeta1U) + 3*Lam6*Treta2Dadjeta1D +& 
&  3*Lam7*Treta2Dadjeta1D - 12._dp*(Treta2Dadjeta1Ueta2Uadjeta1D) + 6*Lam4*Treta2Dadjeta2D +& 
&  Lam6*Treta2Ladjeta1L + Lam7*Treta2Ladjeta1L + 2*Lam4*Treta2Ladjeta2L + 6*Lam4*Treta2Uadjeta2U +& 
&  (10._dp*(Lam6) + 2._dp*(Lam7) + 3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L +           & 
&  3._dp*(Treta2Uadjeta1U))*Conjg(Lam6) + 2*Lam6*Conjg(Lam7) + 3*Treta1Dadjeta2D*Conjg(Lam7)& 
&  + Treta1Ladjeta2L*Conjg(Lam7) + 3*Treta2Uadjeta1U*Conjg(Lam7)

 
 
If (TwoLoopRGE) Then 
betaLam42 = 0

 
DLam4 = oo16pi2*( betaLam41 + oo16pi2 * betaLam42 ) 

 
Else 
DLam4 = oo16pi2* betaLam41 
End If 
 
 
Call Chop(DLam4) 

!-------------------- 
! Lam3 
!-------------------- 
 
betaLam31  = 2._dp*(AbsLam5) + 4._dp*(AbsLam7) + 3._dp*(g1p4)/4._dp - (3*g1p2*g2p2)   & 
& /2._dp + 9._dp*(g2p4)/4._dp + 4._dp*(Lam3p2) + 2._dp*(Lam4p2) - 3*g1p2*Lam3 -          & 
&  9*g2p2*Lam3 + 6._dp*Lam1*Lam3 + 6._dp*Lam2*Lam3 + 2._dp*Lam1*Lam4 + 2._dp*Lam2*Lam4 + & 
&  6*Lam3*Treta1Dadjeta1D - 12._dp*(Treta1Dadjeta1Deta2Dadjeta2D) + 12._dp*(Treta1Dadjeta2Ueta1Uadjeta2D)& 
&  - 12._dp*(Treta1Dadjeta2Ueta2Uadjeta1D) + 2*Lam3*Treta1Ladjeta1L - 4._dp*(Treta1Ladjeta1Leta2Ladjeta2L)& 
&  + 6*Lam3*Treta1Uadjeta1U - 12._dp*(Treta1Uadjeta1Ueta2Uadjeta2U) - 12._dp*(Treta1Uadjeta2Deta2Dadjeta1U)& 
&  + 3*Lam6*Treta1Uadjeta2U + 3*Lam7*Treta1Uadjeta2U + 3*Lam6*Treta2Dadjeta1D +          & 
&  3*Lam7*Treta2Dadjeta1D + 12._dp*(Treta2Dadjeta1Ueta2Uadjeta1D) + 6*Lam3*Treta2Dadjeta2D +& 
&  Lam6*Treta2Ladjeta1L + Lam7*Treta2Ladjeta1L + 2*Lam3*Treta2Ladjeta2L + 6*Lam3*Treta2Uadjeta2U +& 
&  (4._dp*(Lam6) + 8._dp*(Lam7) + 3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L +            & 
&  3._dp*(Treta2Uadjeta1U))*Conjg(Lam6) + 8*Lam6*Conjg(Lam7) + 3*Treta1Dadjeta2D*Conjg(Lam7)& 
&  + Treta1Ladjeta2L*Conjg(Lam7) + 3*Treta2Uadjeta1U*Conjg(Lam7)

 
 
If (TwoLoopRGE) Then 
betaLam32 = 0

 
DLam3 = oo16pi2*( betaLam31 + oo16pi2 * betaLam32 ) 

 
Else 
DLam3 = oo16pi2* betaLam31 
End If 
 
 
Call Chop(DLam3) 

!-------------------- 
! Lam2 
!-------------------- 
 
betaLam21  = 2._dp*AbsLam5 + 0.75_dp*g1p4 + 1.5_dp*g1p2*g2p2 + 2.25_dp*g2p4 +         & 
&  12._dp*Lam2p2 + 4._dp*Lam3p2 + 2._dp*Lam4p2 - 3._dp*g1p2*Lam2 - 9._dp*g2p2*Lam2 +     & 
&  4._dp*Lam3*Lam4 + 6._dp*Lam7*Treta1Uadjeta2U + 6._dp*Lam7*Treta2Dadjeta1D +           & 
&  12._dp*Lam2*Treta2Dadjeta2D - 12._dp*Treta2Dadjeta2Deta2Dadjeta2D + 2._dp*Lam7*Treta2Ladjeta1L +& 
&  4._dp*Lam2*Treta2Ladjeta2L - 4._dp*Treta2Ladjeta2Leta2Ladjeta2L + 12._dp*Lam2*Treta2Uadjeta2U -& 
&  12._dp*Treta2Uadjeta2Ueta2Uadjeta2U + (24._dp*Lam7 + 6._dp*Treta1Dadjeta2D +          & 
&  2._dp*Treta1Ladjeta2L + 6._dp*Treta2Uadjeta1U)*Conjg(Lam7)

 
 
If (TwoLoopRGE) Then 
betaLam22 = 0

 
DLam2 = oo16pi2*( betaLam21 + oo16pi2 * betaLam22 ) 

 
Else 
DLam2 = oo16pi2* betaLam21 
End If 
 
 
Call Chop(DLam2) 

!-------------------- 
! eta1U 
!-------------------- 
 
betaeta1U1  = eta1U*(-17._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)        & 
&  + 3._dp*(Treta1Dadjeta1D) + Treta1Ladjeta1L + 3._dp*(Treta1Uadjeta1U)) +              & 
&  (-3._dp*(eta1Uadjeta1Deta1D) + 3._dp*(eta1Uadjeta1Ueta1U) + eta1Uadjeta2Deta2D +      & 
&  eta1Uadjeta2Ueta2U - 4._dp*(eta2Uadjeta1Deta2D) + 2._dp*(eta2Uadjeta2Ueta1U)          & 
&  + 2*eta2U*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L))/2._dp

 
 
If (TwoLoopRGE) Then 
betaeta1U2 = 0

 
Deta1U = oo16pi2*( betaeta1U1 + oo16pi2 * betaeta1U2 ) 

 
Else 
Deta1U = oo16pi2* betaeta1U1 
End If 
 
 
Call Chop(Deta1U) 

!-------------------- 
! eta2U 
!-------------------- 
 
betaeta2U1  = (2._dp*(eta1Uadjeta1Ueta2U) - 4._dp*(eta1Uadjeta2Deta1D) +              & 
&  eta2Uadjeta1Deta1D + eta2Uadjeta1Ueta1U - 3._dp*(eta2Uadjeta2Deta2D) + 3._dp*(eta2Uadjeta2Ueta2U)& 
&  + 2*eta1U*(3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L + 3._dp*(Treta2Uadjeta1U)))      & 
& /2._dp + eta2U*(-17._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)              & 
&  + 3._dp*(Treta2Dadjeta2D) + Treta2Ladjeta2L + 3._dp*(Treta2Uadjeta2U))

 
 
If (TwoLoopRGE) Then 
betaeta2U2 = 0

 
Deta2U = oo16pi2*( betaeta2U1 + oo16pi2 * betaeta2U2 ) 

 
Else 
Deta2U = oo16pi2* betaeta2U1 
End If 
 
 
Call Chop(Deta2U) 

!-------------------- 
! eta1D 
!-------------------- 
 
betaeta1D1  = eta1D*(-5._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)         & 
&  + 3._dp*(Treta1Dadjeta1D) + Treta1Ladjeta1L + 3._dp*(Treta1Uadjeta1U)) +              & 
&  (3._dp*(eta1Dadjeta1Deta1D) - 3._dp*(eta1Dadjeta1Ueta1U) + eta1Dadjeta2Deta2D +       & 
&  eta1Dadjeta2Ueta2U - 4._dp*(eta2Dadjeta1Ueta2U) + 2._dp*(eta2Dadjeta2Deta1D)          & 
&  + 2*eta2D*(3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L + 3._dp*(Treta2Uadjeta1U)))/2._dp

 
 
If (TwoLoopRGE) Then 
betaeta1D2 = 0

 
Deta1D = oo16pi2*( betaeta1D1 + oo16pi2 * betaeta1D2 ) 

 
Else 
Deta1D = oo16pi2* betaeta1D1 
End If 
 
 
Call Chop(Deta1D) 

!-------------------- 
! eta1L 
!-------------------- 
 
betaeta1L1  = (3._dp*(eta1Ladjeta1Leta1L) + eta1Ladjeta2Leta2L + 2._dp*(eta2Ladjeta2Leta1L))& 
& /2._dp + eta1L*(-15._dp*(g1p2)/4._dp - 9._dp*(g2p2)/4._dp + 3._dp*(Treta1Dadjeta1D)    & 
&  + Treta1Ladjeta1L + 3._dp*(Treta1Uadjeta1U)) + eta2L*(3._dp*(Treta1Dadjeta2D)         & 
&  + Treta1Ladjeta2L + 3._dp*(Treta2Uadjeta1U))

 
 
If (TwoLoopRGE) Then 
betaeta1L2 = 0

 
Deta1L = oo16pi2*( betaeta1L1 + oo16pi2 * betaeta1L2 ) 

 
Else 
Deta1L = oo16pi2* betaeta1L1 
End If 
 
 
Call Chop(Deta1L) 

!-------------------- 
! eta2D 
!-------------------- 
 
betaeta2D1  = (2._dp*(eta1Dadjeta1Deta2D) - 4._dp*(eta1Dadjeta2Ueta1U) +              & 
&  eta2Dadjeta1Deta1D + eta2Dadjeta1Ueta1U + 3._dp*(eta2Dadjeta2Deta2D) - 3._dp*(eta2Dadjeta2Ueta2U)& 
&  + 2*eta1D*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L))      & 
& /2._dp + eta2D*(-5._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)               & 
&  + 3._dp*(Treta2Dadjeta2D) + Treta2Ladjeta2L + 3._dp*(Treta2Uadjeta2U))

 
 
If (TwoLoopRGE) Then 
betaeta2D2 = 0

 
Deta2D = oo16pi2*( betaeta2D1 + oo16pi2 * betaeta2D2 ) 

 
Else 
Deta2D = oo16pi2* betaeta2D1 
End If 
 
 
Call Chop(Deta2D) 

!-------------------- 
! eta2L 
!-------------------- 
 
betaeta2L1  = (2._dp*(eta1Ladjeta1Leta2L) + eta2Ladjeta1Leta1L + 3._dp*(eta2Ladjeta2Leta2L))& 
& /2._dp + eta1L*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L)   & 
&  + eta2L*(-15._dp*(g1p2)/4._dp - 9._dp*(g2p2)/4._dp + 3._dp*(Treta2Dadjeta2D)          & 
&  + Treta2Ladjeta2L + 3._dp*(Treta2Uadjeta2U))

 
 
If (TwoLoopRGE) Then 
betaeta2L2 = 0

 
Deta2L = oo16pi2*( betaeta2L1 + oo16pi2 * betaeta2L2 ) 

 
Else 
Deta2L = oo16pi2* betaeta2L1 
End If 
 
 
Call Chop(Deta2L) 

!-------------------- 
! M12 
!-------------------- 
 
betaM121  = -6*Lam6*M112 - (3*g1p2*M12)/2._dp - (9*g2p2*M12)/2._dp + 2*Lam3*M12 +     & 
&  4*Lam4*M12 - 6*Lam7*M222 + 3*M12*Treta1Dadjeta1D + M12*Treta1Ladjeta1L +              & 
&  3*M12*Treta1Uadjeta1U + 3*M12*Treta2Dadjeta2D + M12*Treta2Ladjeta2L + 3*M12*Treta2Uadjeta2U +& 
&  6*Conjg(Lam5)*Conjg(M12)

 
 
If (TwoLoopRGE) Then 
betaM122 = 0

 
DM12 = oo16pi2*( betaM121 + oo16pi2 * betaM122 ) 

 
Else 
DM12 = oo16pi2* betaM121 
End If 
 
 
Call Chop(DM12) 

!-------------------- 
! M112 
!-------------------- 
 
betaM1121  = (-3*g1p2*M112)/2._dp - (9*g2p2*M112)/2._dp + 6._dp*Lam1*M112 +           & 
&  4*Lam3*M222 + 2*Lam4*M222 + 6*M112*Treta1Dadjeta1D + 2*M112*Treta1Ladjeta1L +         & 
&  6*M112*Treta1Uadjeta1U - 6*M12*Conjg(Lam6) - 6*Lam6*Conjg(M12)

 
 
If (TwoLoopRGE) Then 
betaM1122 = 0

 
DM112 = oo16pi2*( betaM1121 + oo16pi2 * betaM1122 ) 

 
Else 
DM112 = oo16pi2* betaM1121 
End If 
 
 
Call Chop(DM112) 

!-------------------- 
! M222 
!-------------------- 
 
betaM2221  = 4*Lam3*M112 + 2*Lam4*M112 - (3*g1p2*M222)/2._dp - (9*g2p2*M222)          & 
& /2._dp + 6._dp*Lam2*M222 + 6*M222*Treta2Dadjeta2D + 2*M222*Treta2Ladjeta2L +           & 
&  6*M222*Treta2Uadjeta2U - 6*M12*Conjg(Lam7) - 6*Lam7*Conjg(M12)

 
 
If (TwoLoopRGE) Then 
betaM2222 = 0

 
DM222 = oo16pi2*( betaM2221 + oo16pi2 * betaM2222 ) 

 
Else 
DM222 = oo16pi2* betaM2221 
End If 
 
 
Call Chop(DM222) 

Call ParametersToG131(Dg1,Dg2,Dg3,DLam6,DLam5,DLam7,DLam1,DLam4,DLam3,DLam2,          & 
& Deta1U,Deta2U,Deta1D,Deta1L,Deta2D,Deta2L,DM12,DM112,DM222,f)

Iname = Iname - 1 
 
End Subroutine rge131  

Subroutine GToParameters133(g,g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,            & 
& eta1U,eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222,vd,vu)

Implicit None 
Real(dp), Intent(in) :: g(133) 
Real(dp),Intent(out) :: g1,g2,g3,vd,vu

Complex(dp),Intent(out) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Integer i1, i2, i3, i4, SumI 
 
Iname = Iname +1 
NameOfUnit(Iname) = 'GToParameters133' 
 
g1= g(1) 
g2= g(2) 
g3= g(3) 
Lam6= Cmplx(g(4),g(5),dp) 
Lam5= Cmplx(g(6),g(7),dp) 
Lam7= Cmplx(g(8),g(9),dp) 
Lam1= Cmplx(g(10),g(11),dp) 
Lam4= Cmplx(g(12),g(13),dp) 
Lam3= Cmplx(g(14),g(15),dp) 
Lam2= Cmplx(g(16),g(17),dp) 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta1U(i1,i2) = Cmplx( g(SumI+18), g(SumI+19), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta2U(i1,i2) = Cmplx( g(SumI+36), g(SumI+37), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta1D(i1,i2) = Cmplx( g(SumI+54), g(SumI+55), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta1L(i1,i2) = Cmplx( g(SumI+72), g(SumI+73), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta2D(i1,i2) = Cmplx( g(SumI+90), g(SumI+91), dp) 
End Do 
 End Do 
 
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
eta2L(i1,i2) = Cmplx( g(SumI+108), g(SumI+109), dp) 
End Do 
 End Do 
 
M12= Cmplx(g(126),g(127),dp) 
M112= Cmplx(g(128),g(129),dp) 
M222= Cmplx(g(130),g(131),dp) 
vd= g(132) 
vu= g(133) 
Do i1=1,133 
If (g(i1).ne.g(i1)) Then 
 Write(*,*) "NaN appearing in ",NameOfUnit(Iname) 
 Write(*,*) "At position ", i1 
 Call TerminateProgram 
End if 
End do 
Iname = Iname - 1 
 
End Subroutine GToParameters133

Subroutine ParametersToG133(g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,              & 
& eta1U,eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222,vd,vu,g)

Implicit None 
Real(dp), Intent(out) :: g(133) 
Real(dp), Intent(in) :: g1,g2,g3,vd,vu

Complex(dp), Intent(in) :: Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U(3,3),eta2U(3,3),eta1D(3,3),eta1L(3,3),       & 
& eta2D(3,3),eta2L(3,3),M12,M112,M222

Integer i1, i2, i3, i4, SumI 
 
Iname = Iname +1 
NameOfUnit(Iname) = 'ParametersToG133' 
 
g(1) = g1  
g(2) = g2  
g(3) = g3  
g(4) = Real(Lam6,dp)  
g(5) = Aimag(Lam6)  
g(6) = Real(Lam5,dp)  
g(7) = Aimag(Lam5)  
g(8) = Real(Lam7,dp)  
g(9) = Aimag(Lam7)  
g(10) = Real(Lam1,dp)  
g(11) = Aimag(Lam1)  
g(12) = Real(Lam4,dp)  
g(13) = Aimag(Lam4)  
g(14) = Real(Lam3,dp)  
g(15) = Aimag(Lam3)  
g(16) = Real(Lam2,dp)  
g(17) = Aimag(Lam2)  
Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+18) = Real(eta1U(i1,i2), dp) 
g(SumI+19) = Aimag(eta1U(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+36) = Real(eta2U(i1,i2), dp) 
g(SumI+37) = Aimag(eta2U(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+54) = Real(eta1D(i1,i2), dp) 
g(SumI+55) = Aimag(eta1D(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+72) = Real(eta1L(i1,i2), dp) 
g(SumI+73) = Aimag(eta1L(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+90) = Real(eta2D(i1,i2), dp) 
g(SumI+91) = Aimag(eta2D(i1,i2)) 
End Do 
End Do 

Do i1 = 1,3
Do i2 = 1,3
SumI = (i2-1) + (i1-1)*3
SumI = SumI*2 
g(SumI+108) = Real(eta2L(i1,i2), dp) 
g(SumI+109) = Aimag(eta2L(i1,i2)) 
End Do 
End Do 

g(126) = Real(M12,dp)  
g(127) = Aimag(M12)  
g(128) = Real(M112,dp)  
g(129) = Aimag(M112)  
g(130) = Real(M222,dp)  
g(131) = Aimag(M222)  
g(132) = vd  
g(133) = vu  
Iname = Iname - 1 
 
End Subroutine ParametersToG133

Subroutine rge133(len, T, GY, F) 
Implicit None 
Integer, Intent(in) :: len 
Real(dp), Intent(in) :: T, GY(len) 
Real(dp), Intent(out) :: F(len) 
Integer :: i1,i2,i3,i4 
Integer :: j1,j2,j3,j4,j5,j6,j7 
Real(dp) :: q 
Real(dp) :: g1,betag11,betag12,Dg1,g2,betag21,betag22,Dg2,g3,betag31,betag32,         & 
& Dg3,vd,betavd1,betavd2,Dvd,vu,betavu1,betavu2,Dvu
Complex(dp) :: Lam6,betaLam61,betaLam62,DLam6,Lam5,betaLam51,betaLam52,               & 
& DLam5,Lam7,betaLam71,betaLam72,DLam7,Lam1,betaLam11,betaLam12,DLam1,Lam4,              & 
& betaLam41,betaLam42,DLam4,Lam3,betaLam31,betaLam32,DLam3,Lam2,betaLam21,               & 
& betaLam22,DLam2,eta1U(3,3),betaeta1U1(3,3),betaeta1U2(3,3),Deta1U(3,3),adjeta1U(3,3)   & 
& ,eta2U(3,3),betaeta2U1(3,3),betaeta2U2(3,3),Deta2U(3,3),adjeta2U(3,3),eta1D(3,3)       & 
& ,betaeta1D1(3,3),betaeta1D2(3,3),Deta1D(3,3),adjeta1D(3,3),eta1L(3,3),betaeta1L1(3,3)  & 
& ,betaeta1L2(3,3),Deta1L(3,3),adjeta1L(3,3),eta2D(3,3),betaeta2D1(3,3),betaeta2D2(3,3)  & 
& ,Deta2D(3,3),adjeta2D(3,3),eta2L(3,3),betaeta2L1(3,3),betaeta2L2(3,3),Deta2L(3,3)      & 
& ,adjeta2L(3,3),M12,betaM121,betaM122,DM12,M112,betaM1121,betaM1122,DM112,              & 
& M222,betaM2221,betaM2222,DM222
Real(dp) :: AbsLam5,AbsLam7
Complex(dp) :: eta1Dadjeta1D(3,3),eta1Dadjeta2D(3,3),eta1Ladjeta1L(3,3),eta1Ladjeta2L(3,3),          & 
& eta1Uadjeta1U(3,3),eta1Uadjeta2D(3,3),eta1Uadjeta2U(3,3),eta2Dadjeta1D(3,3),           & 
& eta2Dadjeta1U(3,3),eta2Dadjeta2D(3,3),eta2Ladjeta1L(3,3),eta2Ladjeta2L(3,3),           & 
& eta2Uadjeta1D(3,3),eta2Uadjeta1U(3,3),eta2Uadjeta2U(3,3),adjeta1Deta1D(3,3),           & 
& adjeta1Deta2D(3,3),adjeta1Leta1L(3,3),adjeta1Leta2L(3,3),adjeta1Ueta1U(3,3),           & 
& adjeta1Ueta2U(3,3),adjeta2Deta1D(3,3),adjeta2Deta2D(3,3),adjeta2Leta1L(3,3),           & 
& adjeta2Leta2L(3,3),adjeta2Ueta1U(3,3),adjeta2Ueta2U(3,3),eta1Dadjeta1Deta1D(3,3),      & 
& eta1Dadjeta1Deta2D(3,3),eta1Dadjeta1Ueta1U(3,3),eta1Dadjeta2Deta2D(3,3),               & 
& eta1Dadjeta2Ueta1U(3,3),eta1Dadjeta2Ueta2U(3,3),eta1Ladjeta1Leta1L(3,3),               & 
& eta1Ladjeta1Leta2L(3,3),eta1Ladjeta2Leta2L(3,3),eta1Uadjeta1Deta1D(3,3),               & 
& eta1Uadjeta1Ueta1U(3,3),eta1Uadjeta1Ueta2U(3,3),eta1Uadjeta2Deta1D(3,3),               & 
& eta1Uadjeta2Deta2D(3,3),eta1Uadjeta2Ueta2U(3,3),eta2Dadjeta1Deta1D(3,3),               & 
& eta2Dadjeta1Ueta1U(3,3),eta2Dadjeta1Ueta2U(3,3),eta2Dadjeta2Deta1D(3,3),               & 
& eta2Dadjeta2Deta2D(3,3),eta2Dadjeta2Ueta2U(3,3),eta2Ladjeta1Leta1L(3,3),               & 
& eta2Ladjeta2Leta1L(3,3),eta2Ladjeta2Leta2L(3,3),eta2Uadjeta1Deta1D(3,3),               & 
& eta2Uadjeta1Deta2D(3,3),eta2Uadjeta1Ueta1U(3,3),eta2Uadjeta2Deta2D(3,3),               & 
& eta2Uadjeta2Ueta1U(3,3),eta2Uadjeta2Ueta2U(3,3),adjeta1Deta1Dadjeta1D(3,3),            & 
& adjeta1Deta1Dadjeta2D(3,3),adjeta1Deta2Dadjeta1D(3,3),adjeta1Deta2Dadjeta2D(3,3),      & 
& adjeta1Leta1Ladjeta1L(3,3),adjeta1Leta1Ladjeta2L(3,3),adjeta1Leta2Ladjeta1L(3,3),      & 
& adjeta1Leta2Ladjeta2L(3,3),adjeta1Ueta1Uadjeta1U(3,3),adjeta1Ueta2Uadjeta1D(3,3),      & 
& adjeta1Ueta2Uadjeta1U(3,3),adjeta1Ueta2Uadjeta2U(3,3),adjeta2Deta1Dadjeta1D(3,3),      & 
& adjeta2Deta2Dadjeta1D(3,3),adjeta2Deta2Dadjeta1U(3,3),adjeta2Deta2Dadjeta2D(3,3),      & 
& adjeta2Leta1Ladjeta1L(3,3),adjeta2Leta2Ladjeta1L(3,3),adjeta2Leta2Ladjeta2L(3,3),      & 
& adjeta2Ueta1Uadjeta2D(3,3),adjeta2Ueta1Uadjeta2U(3,3),adjeta2Ueta2Uadjeta1D(3,3),      & 
& adjeta2Ueta2Uadjeta1U(3,3),adjeta2Ueta2Uadjeta2U(3,3),eta1Dadjeta1Deta1Dadjeta1D(3,3), & 
& eta1Dadjeta1Deta1Dadjeta2D(3,3),eta1Dadjeta1Deta2Dadjeta2D(3,3),eta1Dadjeta2Deta1Dadjeta1D(3,3),& 
& eta1Dadjeta2Deta2Dadjeta1D(3,3),eta1Dadjeta2Deta2Dadjeta2D(3,3),eta1Dadjeta2Ueta1Uadjeta2D(3,3),& 
& eta1Dadjeta2Ueta2Uadjeta1D(3,3),eta1Ladjeta1Leta1Ladjeta1L(3,3),eta1Ladjeta1Leta1Ladjeta2L(3,3),& 
& eta1Ladjeta1Leta2Ladjeta2L(3,3),eta1Ladjeta2Leta1Ladjeta1L(3,3),eta1Ladjeta2Leta2Ladjeta1L(3,3),& 
& eta1Ladjeta2Leta2Ladjeta2L(3,3),eta1Uadjeta1Ueta1Uadjeta1U(3,3),eta1Uadjeta1Ueta2Uadjeta1U(3,3),& 
& eta1Uadjeta1Ueta2Uadjeta2U(3,3),eta1Uadjeta2Deta2Dadjeta1U(3,3),eta1Uadjeta2Ueta1Uadjeta2U(3,3),& 
& eta1Uadjeta2Ueta2Uadjeta1U(3,3),eta2Dadjeta1Deta2Dadjeta1D(3,3),eta2Dadjeta1Ueta2Uadjeta1D(3,3),& 
& eta2Dadjeta2Deta2Dadjeta2D(3,3),eta2Ladjeta1Leta2Ladjeta1L(3,3),eta2Ladjeta2Leta2Ladjeta2L(3,3),& 
& eta2Uadjeta1Ueta2Uadjeta2U(3,3),eta2Uadjeta2Ueta2Uadjeta1U(3,3),eta2Uadjeta2Ueta2Uadjeta2U(3,3)

Complex(dp) :: Treta1Dadjeta1D,Treta1Dadjeta2D,Treta1Ladjeta1L,Treta1Ladjeta2L,Treta1Uadjeta1U,      & 
& Treta1Uadjeta2U,Treta2Dadjeta1D,Treta2Dadjeta2D,Treta2Ladjeta1L,Treta2Ladjeta2L,       & 
& Treta2Uadjeta1U,Treta2Uadjeta2U,Treta1Dadjeta1Deta1Dadjeta1D,Treta1Dadjeta1Deta1Dadjeta2D,& 
& Treta1Dadjeta1Deta2Dadjeta2D,Treta1Dadjeta2Deta1Dadjeta1D,Treta1Dadjeta2Deta2Dadjeta1D,& 
& Treta1Dadjeta2Deta2Dadjeta2D,Treta1Dadjeta2Ueta1Uadjeta2D,Treta1Dadjeta2Ueta2Uadjeta1D,& 
& Treta1Ladjeta1Leta1Ladjeta1L,Treta1Ladjeta1Leta1Ladjeta2L,Treta1Ladjeta1Leta2Ladjeta2L,& 
& Treta1Ladjeta2Leta1Ladjeta1L,Treta1Ladjeta2Leta2Ladjeta1L,Treta1Ladjeta2Leta2Ladjeta2L,& 
& Treta1Uadjeta1Ueta1Uadjeta1U,Treta1Uadjeta1Ueta2Uadjeta1U,Treta1Uadjeta1Ueta2Uadjeta2U,& 
& Treta1Uadjeta2Deta2Dadjeta1U,Treta1Uadjeta2Ueta1Uadjeta2U,Treta1Uadjeta2Ueta2Uadjeta1U,& 
& Treta2Dadjeta1Deta2Dadjeta1D,Treta2Dadjeta1Ueta2Uadjeta1D,Treta2Dadjeta2Deta2Dadjeta2D,& 
& Treta2Ladjeta1Leta2Ladjeta1L,Treta2Ladjeta2Leta2Ladjeta2L,Treta2Uadjeta1Ueta2Uadjeta2U,& 
& Treta2Uadjeta2Ueta2Uadjeta1U,Treta2Uadjeta2Ueta2Uadjeta2U

Real(dp) :: g1p2,g1p3,g1p4,g2p2,g2p3,g2p4,g3p2,g3p3

Complex(dp) :: Lam1p2,Lam2p2,Lam3p2,Lam4p2,CLam6p2,CLam7p2

Iname = Iname +1 
NameOfUnit(Iname) = 'rge133' 
 
OnlyDiagonal = .Not.GenerationMixing 
q = t 
 
Call GToParameters133(gy,g1,g2,g3,Lam6,Lam5,Lam7,Lam1,Lam4,Lam3,Lam2,eta1U,           & 
& eta2U,eta1D,eta1L,eta2D,eta2L,M12,M112,M222,vd,vu)

AbsLam5 = Conjg(Lam5)*Lam5
AbsLam7 = Conjg(Lam7)*Lam7
Call Adjungate(eta1U,adjeta1U)
Call Adjungate(eta2U,adjeta2U)
Call Adjungate(eta1D,adjeta1D)
Call Adjungate(eta1L,adjeta1L)
Call Adjungate(eta2D,adjeta2D)
Call Adjungate(eta2L,adjeta2L)
 eta1Dadjeta1D = Matmul(eta1D,adjeta1D) 
Forall(i2=1:3)  eta1Dadjeta1D(i2,i2) =  Real(eta1Dadjeta1D(i2,i2),dp) 
 eta1Dadjeta2D = Matmul(eta1D,adjeta2D) 
 eta1Ladjeta1L = Matmul(eta1L,adjeta1L) 
Forall(i2=1:3)  eta1Ladjeta1L(i2,i2) =  Real(eta1Ladjeta1L(i2,i2),dp) 
 eta1Ladjeta2L = Matmul(eta1L,adjeta2L) 
 eta1Uadjeta1U = Matmul(eta1U,adjeta1U) 
Forall(i2=1:3)  eta1Uadjeta1U(i2,i2) =  Real(eta1Uadjeta1U(i2,i2),dp) 
 eta1Uadjeta2D = Matmul(eta1U,adjeta2D) 
 eta1Uadjeta2U = Matmul(eta1U,adjeta2U) 
 eta2Dadjeta1D = Matmul(eta2D,adjeta1D) 
 eta2Dadjeta1U = Matmul(eta2D,adjeta1U) 
 eta2Dadjeta2D = Matmul(eta2D,adjeta2D) 
Forall(i2=1:3)  eta2Dadjeta2D(i2,i2) =  Real(eta2Dadjeta2D(i2,i2),dp) 
 eta2Ladjeta1L = Matmul(eta2L,adjeta1L) 
 eta2Ladjeta2L = Matmul(eta2L,adjeta2L) 
Forall(i2=1:3)  eta2Ladjeta2L(i2,i2) =  Real(eta2Ladjeta2L(i2,i2),dp) 
 eta2Uadjeta1D = Matmul(eta2U,adjeta1D) 
 eta2Uadjeta1U = Matmul(eta2U,adjeta1U) 
 eta2Uadjeta2U = Matmul(eta2U,adjeta2U) 
Forall(i2=1:3)  eta2Uadjeta2U(i2,i2) =  Real(eta2Uadjeta2U(i2,i2),dp) 
 adjeta1Deta1D = Matmul(adjeta1D,eta1D) 
Forall(i2=1:3)  adjeta1Deta1D(i2,i2) =  Real(adjeta1Deta1D(i2,i2),dp) 
 adjeta1Deta2D = Matmul(adjeta1D,eta2D) 
 adjeta1Leta1L = Matmul(adjeta1L,eta1L) 
Forall(i2=1:3)  adjeta1Leta1L(i2,i2) =  Real(adjeta1Leta1L(i2,i2),dp) 
 adjeta1Leta2L = Matmul(adjeta1L,eta2L) 
 adjeta1Ueta1U = Matmul(adjeta1U,eta1U) 
Forall(i2=1:3)  adjeta1Ueta1U(i2,i2) =  Real(adjeta1Ueta1U(i2,i2),dp) 
 adjeta1Ueta2U = Matmul(adjeta1U,eta2U) 
 adjeta2Deta1D = Matmul(adjeta2D,eta1D) 
 adjeta2Deta2D = Matmul(adjeta2D,eta2D) 
Forall(i2=1:3)  adjeta2Deta2D(i2,i2) =  Real(adjeta2Deta2D(i2,i2),dp) 
 adjeta2Leta1L = Matmul(adjeta2L,eta1L) 
 adjeta2Leta2L = Matmul(adjeta2L,eta2L) 
Forall(i2=1:3)  adjeta2Leta2L(i2,i2) =  Real(adjeta2Leta2L(i2,i2),dp) 
 adjeta2Ueta1U = Matmul(adjeta2U,eta1U) 
 adjeta2Ueta2U = Matmul(adjeta2U,eta2U) 
Forall(i2=1:3)  adjeta2Ueta2U(i2,i2) =  Real(adjeta2Ueta2U(i2,i2),dp) 
 eta1Dadjeta1Deta1D = Matmul(eta1D,adjeta1Deta1D) 
 eta1Dadjeta1Deta2D = Matmul(eta1D,adjeta1Deta2D) 
 eta1Dadjeta1Ueta1U = Matmul(eta1D,adjeta1Ueta1U) 
 eta1Dadjeta2Deta2D = Matmul(eta1D,adjeta2Deta2D) 
 eta1Dadjeta2Ueta1U = Matmul(eta1D,adjeta2Ueta1U) 
 eta1Dadjeta2Ueta2U = Matmul(eta1D,adjeta2Ueta2U) 
 eta1Ladjeta1Leta1L = Matmul(eta1L,adjeta1Leta1L) 
 eta1Ladjeta1Leta2L = Matmul(eta1L,adjeta1Leta2L) 
 eta1Ladjeta2Leta2L = Matmul(eta1L,adjeta2Leta2L) 
 eta1Uadjeta1Deta1D = Matmul(eta1U,adjeta1Deta1D) 
 eta1Uadjeta1Ueta1U = Matmul(eta1U,adjeta1Ueta1U) 
 eta1Uadjeta1Ueta2U = Matmul(eta1U,adjeta1Ueta2U) 
 eta1Uadjeta2Deta1D = Matmul(eta1U,adjeta2Deta1D) 
 eta1Uadjeta2Deta2D = Matmul(eta1U,adjeta2Deta2D) 
 eta1Uadjeta2Ueta2U = Matmul(eta1U,adjeta2Ueta2U) 
 eta2Dadjeta1Deta1D = Matmul(eta2D,adjeta1Deta1D) 
 eta2Dadjeta1Ueta1U = Matmul(eta2D,adjeta1Ueta1U) 
 eta2Dadjeta1Ueta2U = Matmul(eta2D,adjeta1Ueta2U) 
 eta2Dadjeta2Deta1D = Matmul(eta2D,adjeta2Deta1D) 
 eta2Dadjeta2Deta2D = Matmul(eta2D,adjeta2Deta2D) 
 eta2Dadjeta2Ueta2U = Matmul(eta2D,adjeta2Ueta2U) 
 eta2Ladjeta1Leta1L = Matmul(eta2L,adjeta1Leta1L) 
 eta2Ladjeta2Leta1L = Matmul(eta2L,adjeta2Leta1L) 
 eta2Ladjeta2Leta2L = Matmul(eta2L,adjeta2Leta2L) 
 eta2Uadjeta1Deta1D = Matmul(eta2U,adjeta1Deta1D) 
 eta2Uadjeta1Deta2D = Matmul(eta2U,adjeta1Deta2D) 
 eta2Uadjeta1Ueta1U = Matmul(eta2U,adjeta1Ueta1U) 
 eta2Uadjeta2Deta2D = Matmul(eta2U,adjeta2Deta2D) 
 eta2Uadjeta2Ueta1U = Matmul(eta2U,adjeta2Ueta1U) 
 eta2Uadjeta2Ueta2U = Matmul(eta2U,adjeta2Ueta2U) 
 adjeta1Deta1Dadjeta1D = Matmul(adjeta1D,eta1Dadjeta1D) 
 adjeta1Deta1Dadjeta2D = Matmul(adjeta1D,eta1Dadjeta2D) 
 adjeta1Deta2Dadjeta1D = Matmul(adjeta1D,eta2Dadjeta1D) 
 adjeta1Deta2Dadjeta2D = Matmul(adjeta1D,eta2Dadjeta2D) 
 adjeta1Leta1Ladjeta1L = Matmul(adjeta1L,eta1Ladjeta1L) 
 adjeta1Leta1Ladjeta2L = Matmul(adjeta1L,eta1Ladjeta2L) 
 adjeta1Leta2Ladjeta1L = Matmul(adjeta1L,eta2Ladjeta1L) 
 adjeta1Leta2Ladjeta2L = Matmul(adjeta1L,eta2Ladjeta2L) 
 adjeta1Ueta1Uadjeta1U = Matmul(adjeta1U,eta1Uadjeta1U) 
 adjeta1Ueta2Uadjeta1D = Matmul(adjeta1U,eta2Uadjeta1D) 
 adjeta1Ueta2Uadjeta1U = Matmul(adjeta1U,eta2Uadjeta1U) 
 adjeta1Ueta2Uadjeta2U = Matmul(adjeta1U,eta2Uadjeta2U) 
 adjeta2Deta1Dadjeta1D = Matmul(adjeta2D,eta1Dadjeta1D) 
 adjeta2Deta2Dadjeta1D = Matmul(adjeta2D,eta2Dadjeta1D) 
 adjeta2Deta2Dadjeta1U = Matmul(adjeta2D,eta2Dadjeta1U) 
 adjeta2Deta2Dadjeta2D = Matmul(adjeta2D,eta2Dadjeta2D) 
 adjeta2Leta1Ladjeta1L = Matmul(adjeta2L,eta1Ladjeta1L) 
 adjeta2Leta2Ladjeta1L = Matmul(adjeta2L,eta2Ladjeta1L) 
 adjeta2Leta2Ladjeta2L = Matmul(adjeta2L,eta2Ladjeta2L) 
 adjeta2Ueta1Uadjeta2D = Matmul(adjeta2U,eta1Uadjeta2D) 
 adjeta2Ueta1Uadjeta2U = Matmul(adjeta2U,eta1Uadjeta2U) 
 adjeta2Ueta2Uadjeta1D = Matmul(adjeta2U,eta2Uadjeta1D) 
 adjeta2Ueta2Uadjeta1U = Matmul(adjeta2U,eta2Uadjeta1U) 
 adjeta2Ueta2Uadjeta2U = Matmul(adjeta2U,eta2Uadjeta2U) 
 eta1Dadjeta1Deta1Dadjeta1D = Matmul(eta1D,adjeta1Deta1Dadjeta1D) 
Forall(i2=1:3)  eta1Dadjeta1Deta1Dadjeta1D(i2,i2) =  Real(eta1Dadjeta1Deta1Dadjeta1D(i2,i2),dp) 
 eta1Dadjeta1Deta1Dadjeta2D = Matmul(eta1D,adjeta1Deta1Dadjeta2D) 
 eta1Dadjeta1Deta2Dadjeta2D = Matmul(eta1D,adjeta1Deta2Dadjeta2D) 
 eta1Dadjeta2Deta1Dadjeta1D = Matmul(eta1D,adjeta2Deta1Dadjeta1D) 
 eta1Dadjeta2Deta2Dadjeta1D = Matmul(eta1D,adjeta2Deta2Dadjeta1D) 
Forall(i2=1:3)  eta1Dadjeta2Deta2Dadjeta1D(i2,i2) =  Real(eta1Dadjeta2Deta2Dadjeta1D(i2,i2),dp) 
 eta1Dadjeta2Deta2Dadjeta2D = Matmul(eta1D,adjeta2Deta2Dadjeta2D) 
 eta1Dadjeta2Ueta1Uadjeta2D = Matmul(eta1D,adjeta2Ueta1Uadjeta2D) 
 eta1Dadjeta2Ueta2Uadjeta1D = Matmul(eta1D,adjeta2Ueta2Uadjeta1D) 
Forall(i2=1:3)  eta1Dadjeta2Ueta2Uadjeta1D(i2,i2) =  Real(eta1Dadjeta2Ueta2Uadjeta1D(i2,i2),dp) 
 eta1Ladjeta1Leta1Ladjeta1L = Matmul(eta1L,adjeta1Leta1Ladjeta1L) 
Forall(i2=1:3)  eta1Ladjeta1Leta1Ladjeta1L(i2,i2) =  Real(eta1Ladjeta1Leta1Ladjeta1L(i2,i2),dp) 
 eta1Ladjeta1Leta1Ladjeta2L = Matmul(eta1L,adjeta1Leta1Ladjeta2L) 
 eta1Ladjeta1Leta2Ladjeta2L = Matmul(eta1L,adjeta1Leta2Ladjeta2L) 
 eta1Ladjeta2Leta1Ladjeta1L = Matmul(eta1L,adjeta2Leta1Ladjeta1L) 
 eta1Ladjeta2Leta2Ladjeta1L = Matmul(eta1L,adjeta2Leta2Ladjeta1L) 
Forall(i2=1:3)  eta1Ladjeta2Leta2Ladjeta1L(i2,i2) =  Real(eta1Ladjeta2Leta2Ladjeta1L(i2,i2),dp) 
 eta1Ladjeta2Leta2Ladjeta2L = Matmul(eta1L,adjeta2Leta2Ladjeta2L) 
 eta1Uadjeta1Ueta1Uadjeta1U = Matmul(eta1U,adjeta1Ueta1Uadjeta1U) 
Forall(i2=1:3)  eta1Uadjeta1Ueta1Uadjeta1U(i2,i2) =  Real(eta1Uadjeta1Ueta1Uadjeta1U(i2,i2),dp) 
 eta1Uadjeta1Ueta2Uadjeta1U = Matmul(eta1U,adjeta1Ueta2Uadjeta1U) 
 eta1Uadjeta1Ueta2Uadjeta2U = Matmul(eta1U,adjeta1Ueta2Uadjeta2U) 
 eta1Uadjeta2Deta2Dadjeta1U = Matmul(eta1U,adjeta2Deta2Dadjeta1U) 
Forall(i2=1:3)  eta1Uadjeta2Deta2Dadjeta1U(i2,i2) =  Real(eta1Uadjeta2Deta2Dadjeta1U(i2,i2),dp) 
 eta1Uadjeta2Ueta1Uadjeta2U = Matmul(eta1U,adjeta2Ueta1Uadjeta2U) 
 eta1Uadjeta2Ueta2Uadjeta1U = Matmul(eta1U,adjeta2Ueta2Uadjeta1U) 
Forall(i2=1:3)  eta1Uadjeta2Ueta2Uadjeta1U(i2,i2) =  Real(eta1Uadjeta2Ueta2Uadjeta1U(i2,i2),dp) 
 eta2Dadjeta1Deta2Dadjeta1D = Matmul(eta2D,adjeta1Deta2Dadjeta1D) 
 eta2Dadjeta1Ueta2Uadjeta1D = Matmul(eta2D,adjeta1Ueta2Uadjeta1D) 
 eta2Dadjeta2Deta2Dadjeta2D = Matmul(eta2D,adjeta2Deta2Dadjeta2D) 
Forall(i2=1:3)  eta2Dadjeta2Deta2Dadjeta2D(i2,i2) =  Real(eta2Dadjeta2Deta2Dadjeta2D(i2,i2),dp) 
 eta2Ladjeta1Leta2Ladjeta1L = Matmul(eta2L,adjeta1Leta2Ladjeta1L) 
 eta2Ladjeta2Leta2Ladjeta2L = Matmul(eta2L,adjeta2Leta2Ladjeta2L) 
Forall(i2=1:3)  eta2Ladjeta2Leta2Ladjeta2L(i2,i2) =  Real(eta2Ladjeta2Leta2Ladjeta2L(i2,i2),dp) 
 eta2Uadjeta1Ueta2Uadjeta2U = Matmul(eta2U,adjeta1Ueta2Uadjeta2U) 
 eta2Uadjeta2Ueta2Uadjeta1U = Matmul(eta2U,adjeta2Ueta2Uadjeta1U) 
 eta2Uadjeta2Ueta2Uadjeta2U = Matmul(eta2U,adjeta2Ueta2Uadjeta2U) 
Forall(i2=1:3)  eta2Uadjeta2Ueta2Uadjeta2U(i2,i2) =  Real(eta2Uadjeta2Ueta2Uadjeta2U(i2,i2),dp) 
 Treta1Dadjeta1D = Real(cTrace(eta1Dadjeta1D),dp) 
 Treta1Dadjeta2D = cTrace(eta1Dadjeta2D) 
 Treta1Ladjeta1L = Real(cTrace(eta1Ladjeta1L),dp) 
 Treta1Ladjeta2L = cTrace(eta1Ladjeta2L) 
 Treta1Uadjeta1U = Real(cTrace(eta1Uadjeta1U),dp) 
 Treta1Uadjeta2U = cTrace(eta1Uadjeta2U) 
 Treta2Dadjeta1D = cTrace(eta2Dadjeta1D) 
 Treta2Dadjeta2D = Real(cTrace(eta2Dadjeta2D),dp) 
 Treta2Ladjeta1L = cTrace(eta2Ladjeta1L) 
 Treta2Ladjeta2L = Real(cTrace(eta2Ladjeta2L),dp) 
 Treta2Uadjeta1U = cTrace(eta2Uadjeta1U) 
 Treta2Uadjeta2U = Real(cTrace(eta2Uadjeta2U),dp) 
 Treta1Dadjeta1Deta1Dadjeta1D = Real(cTrace(eta1Dadjeta1Deta1Dadjeta1D),dp) 
 Treta1Dadjeta1Deta1Dadjeta2D = cTrace(eta1Dadjeta1Deta1Dadjeta2D) 
 Treta1Dadjeta1Deta2Dadjeta2D = cTrace(eta1Dadjeta1Deta2Dadjeta2D) 
 Treta1Dadjeta2Deta1Dadjeta1D = cTrace(eta1Dadjeta2Deta1Dadjeta1D) 
 Treta1Dadjeta2Deta2Dadjeta1D = Real(cTrace(eta1Dadjeta2Deta2Dadjeta1D),dp) 
 Treta1Dadjeta2Deta2Dadjeta2D = cTrace(eta1Dadjeta2Deta2Dadjeta2D) 
 Treta1Dadjeta2Ueta1Uadjeta2D = cTrace(eta1Dadjeta2Ueta1Uadjeta2D) 
 Treta1Dadjeta2Ueta2Uadjeta1D = Real(cTrace(eta1Dadjeta2Ueta2Uadjeta1D),dp) 
 Treta1Ladjeta1Leta1Ladjeta1L = Real(cTrace(eta1Ladjeta1Leta1Ladjeta1L),dp) 
 Treta1Ladjeta1Leta1Ladjeta2L = cTrace(eta1Ladjeta1Leta1Ladjeta2L) 
 Treta1Ladjeta1Leta2Ladjeta2L = cTrace(eta1Ladjeta1Leta2Ladjeta2L) 
 Treta1Ladjeta2Leta1Ladjeta1L = cTrace(eta1Ladjeta2Leta1Ladjeta1L) 
 Treta1Ladjeta2Leta2Ladjeta1L = Real(cTrace(eta1Ladjeta2Leta2Ladjeta1L),dp) 
 Treta1Ladjeta2Leta2Ladjeta2L = cTrace(eta1Ladjeta2Leta2Ladjeta2L) 
 Treta1Uadjeta1Ueta1Uadjeta1U = Real(cTrace(eta1Uadjeta1Ueta1Uadjeta1U),dp) 
 Treta1Uadjeta1Ueta2Uadjeta1U = cTrace(eta1Uadjeta1Ueta2Uadjeta1U) 
 Treta1Uadjeta1Ueta2Uadjeta2U = cTrace(eta1Uadjeta1Ueta2Uadjeta2U) 
 Treta1Uadjeta2Deta2Dadjeta1U = Real(cTrace(eta1Uadjeta2Deta2Dadjeta1U),dp) 
 Treta1Uadjeta2Ueta1Uadjeta2U = cTrace(eta1Uadjeta2Ueta1Uadjeta2U) 
 Treta1Uadjeta2Ueta2Uadjeta1U = Real(cTrace(eta1Uadjeta2Ueta2Uadjeta1U),dp) 
 Treta2Dadjeta1Deta2Dadjeta1D = cTrace(eta2Dadjeta1Deta2Dadjeta1D) 
 Treta2Dadjeta1Ueta2Uadjeta1D = cTrace(eta2Dadjeta1Ueta2Uadjeta1D) 
 Treta2Dadjeta2Deta2Dadjeta2D = Real(cTrace(eta2Dadjeta2Deta2Dadjeta2D),dp) 
 Treta2Ladjeta1Leta2Ladjeta1L = cTrace(eta2Ladjeta1Leta2Ladjeta1L) 
 Treta2Ladjeta2Leta2Ladjeta2L = Real(cTrace(eta2Ladjeta2Leta2Ladjeta2L),dp) 
 Treta2Uadjeta1Ueta2Uadjeta2U = cTrace(eta2Uadjeta1Ueta2Uadjeta2U) 
 Treta2Uadjeta2Ueta2Uadjeta1U = cTrace(eta2Uadjeta2Ueta2Uadjeta1U) 
 Treta2Uadjeta2Ueta2Uadjeta2U = Real(cTrace(eta2Uadjeta2Ueta2Uadjeta2U),dp) 
 g1p2 =g1**2 
 g1p3 =g1**3 
 g1p4 =g1**4 
 g2p2 =g2**2 
 g2p3 =g2**3 
 g2p4 =g2**4 
 g3p2 =g3**2 
 g3p3 =g3**3 
 Lam1p2 =Lam1**2 
 Lam2p2 =Lam2**2 
 Lam3p2 =Lam3**2 
 Lam4p2 =Lam4**2 
 CLam6p2 =Conjg(Lam6)**2 
 CLam7p2 =Conjg(Lam7)**2 


If (TwoLoopRGE) Then 
End If 
 
 
!-------------------- 
! g1 
!-------------------- 
 
betag11  = 7._dp*(g1p3)

 
 
If (TwoLoopRGE) Then 
betag12 = 0

 
Dg1 = oo16pi2*( betag11 + oo16pi2 * betag12 ) 

 
Else 
Dg1 = oo16pi2* betag11 
End If 
 
 
!-------------------- 
! g2 
!-------------------- 
 
betag21  = -3._dp*(g2p3)

 
 
If (TwoLoopRGE) Then 
betag22 = 0

 
Dg2 = oo16pi2*( betag21 + oo16pi2 * betag22 ) 

 
Else 
Dg2 = oo16pi2* betag21 
End If 
 
 
!-------------------- 
! g3 
!-------------------- 
 
betag31  = -7._dp*(g3p3)

 
 
If (TwoLoopRGE) Then 
betag32 = 0

 
Dg3 = oo16pi2*( betag31 + oo16pi2 * betag32 ) 

 
Else 
Dg3 = oo16pi2* betag31 
End If 
 
 
!-------------------- 
! Lam6 
!-------------------- 
 
betaLam61  = -3*g1p2*Lam6 - 9*g2p2*Lam6 + 12._dp*Lam1*Lam6 + 6*Lam3*Lam6 +            & 
&  8*Lam4*Lam6 + 6*Lam3*Lam7 + 4*Lam4*Lam7 + 9*Lam6*Treta1Dadjeta1D - 6._dp*(Treta1Dadjeta1Deta1Dadjeta2D)& 
&  + 3._dp*Lam1*Treta1Dadjeta2D + 3*Lam3*Treta1Dadjeta2D + 3*Lam4*Treta1Dadjeta2D -      & 
&  6._dp*(Treta1Dadjeta2Deta1Dadjeta1D) + 3*Lam6*Treta1Ladjeta1L - 2._dp*(Treta1Ladjeta1Leta1Ladjeta2L)& 
&  + 1._dp*Lam1*Treta1Ladjeta2L + Lam3*Treta1Ladjeta2L + Lam4*Treta1Ladjeta2L -          & 
&  2._dp*(Treta1Ladjeta2Leta1Ladjeta1L) + 9*Lam6*Treta1Uadjeta1U - 12._dp*(Treta1Uadjeta1Ueta2Uadjeta1U)& 
&  + 3*Lam6*Treta2Dadjeta2D + Lam6*Treta2Ladjeta2L + 3._dp*Lam1*Treta2Uadjeta1U +        & 
&  3*Lam3*Treta2Uadjeta1U + 3*Lam4*Treta2Uadjeta1U + 3*Lam6*Treta2Uadjeta2U +            & 
&  Conjg(Lam5)*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L +    & 
&  10*Conjg(Lam6) + 2*Conjg(Lam7))

 
 
If (TwoLoopRGE) Then 
betaLam62 = 0

 
DLam6 = oo16pi2*( betaLam61 + oo16pi2 * betaLam62 ) 

 
Else 
DLam6 = oo16pi2* betaLam61 
End If 
 
 
Call Chop(DLam6) 

!-------------------- 
! Lam5 
!-------------------- 
 
betaLam51  = 10._dp*(CLam6p2) + 10._dp*(CLam7p2) - 3*g1p2*Lam5 - 9*g2p2*Lam5 +        & 
&  2._dp*Lam1*Lam5 + 2._dp*Lam2*Lam5 + 8*Lam3*Lam5 + 12*Lam4*Lam5 + 6*Lam5*Treta1Dadjeta1D +& 
&  2*Lam5*Treta1Ladjeta1L + 6*Lam5*Treta1Uadjeta1U - 12._dp*(Treta1Uadjeta2Ueta1Uadjeta2U)& 
&  - 12._dp*(Treta2Dadjeta1Deta2Dadjeta1D) + 6*Lam5*Treta2Dadjeta2D - 4._dp*(Treta2Ladjeta1Leta2Ladjeta1L)& 
&  + 2*Lam5*Treta2Ladjeta2L + 6*Lam5*Treta2Uadjeta2U + 2*(3._dp*(Treta1Uadjeta2U)        & 
&  + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L)*Conjg(Lam7) + 2*Conjg(Lam6)              & 
& *(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L + 2*Conjg(Lam7))

 
 
If (TwoLoopRGE) Then 
betaLam52 = 0

 
DLam5 = oo16pi2*( betaLam51 + oo16pi2 * betaLam52 ) 

 
Else 
DLam5 = oo16pi2* betaLam51 
End If 
 
 
Call Chop(DLam5) 

!-------------------- 
! Lam7 
!-------------------- 
 
betaLam71  = 6*Lam3*Lam6 + 4*Lam4*Lam6 - 3*g1p2*Lam7 - 9*g2p2*Lam7 + 12._dp*Lam2*Lam7 +& 
&  6*Lam3*Lam7 + 8*Lam4*Lam7 + 3*Lam7*Treta1Dadjeta1D + 3._dp*Lam2*Treta1Dadjeta2D +     & 
&  3*Lam3*Treta1Dadjeta2D + 3*Lam4*Treta1Dadjeta2D - 12._dp*(Treta1Dadjeta2Deta2Dadjeta2D)& 
&  + Lam7*Treta1Ladjeta1L + 1._dp*Lam2*Treta1Ladjeta2L + Lam3*Treta1Ladjeta2L +          & 
&  Lam4*Treta1Ladjeta2L - 4._dp*(Treta1Ladjeta2Leta2Ladjeta2L) + 3*Lam7*Treta1Uadjeta1U +& 
&  9*Lam7*Treta2Dadjeta2D + 0.0000000000000000555112_dp*Lam2*Treta2Ladjeta1L +           & 
&  3*Lam7*Treta2Ladjeta2L + 3._dp*Lam2*Treta2Uadjeta1U + 3*Lam3*Treta2Uadjeta1U +        & 
&  3*Lam4*Treta2Uadjeta1U - 6._dp*(Treta2Uadjeta1Ueta2Uadjeta2U) + 9*Lam7*Treta2Uadjeta2U -& 
&  6._dp*(Treta2Uadjeta2Ueta2Uadjeta1U) + Conjg(Lam5)*(3._dp*(Treta1Uadjeta2U)           & 
&  + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L + 2*Conjg(Lam6) + 10*Conjg(Lam7))

 
 
If (TwoLoopRGE) Then 
betaLam72 = 0

 
DLam7 = oo16pi2*( betaLam71 + oo16pi2 * betaLam72 ) 

 
Else 
DLam7 = oo16pi2* betaLam71 
End If 
 
 
Call Chop(DLam7) 

!-------------------- 
! Lam1 
!-------------------- 
 
betaLam11  = 2._dp*AbsLam5 + 0.75_dp*g1p4 + 1.5_dp*g1p2*g2p2 + 2.25_dp*g2p4 +         & 
&  12._dp*Lam1p2 + 4._dp*Lam3p2 + 2._dp*Lam4p2 - 3._dp*g1p2*Lam1 - 9._dp*g2p2*Lam1 +     & 
&  4._dp*Lam3*Lam4 + 12._dp*Lam1*Treta1Dadjeta1D - 12._dp*Treta1Dadjeta1Deta1Dadjeta1D + & 
&  4._dp*Lam1*Treta1Ladjeta1L - 4._dp*Treta1Ladjeta1Leta1Ladjeta1L + 12._dp*Lam1*Treta1Uadjeta1U -& 
&  12._dp*Treta1Uadjeta1Ueta1Uadjeta1U + 6._dp*Lam6*Treta1Uadjeta2U + 6._dp*Lam6*Treta2Dadjeta1D +& 
&  2._dp*Lam6*Treta2Ladjeta1L + (24._dp*Lam6 + 6._dp*Treta1Dadjeta2D + 2._dp*Treta1Ladjeta2L +& 
&  6._dp*Treta2Uadjeta1U)*Conjg(Lam6)

 
 
If (TwoLoopRGE) Then 
betaLam12 = 0

 
DLam1 = oo16pi2*( betaLam11 + oo16pi2 * betaLam12 ) 

 
Else 
DLam1 = oo16pi2* betaLam11 
End If 
 
 
Call Chop(DLam1) 

!-------------------- 
! Lam4 
!-------------------- 
 
betaLam41  = 8._dp*(AbsLam5) + 10._dp*(AbsLam7) + 3*g1p2*g2p2 + 4._dp*(Lam4p2)        & 
&  - 3*g1p2*Lam4 - 9*g2p2*Lam4 + 2._dp*Lam1*Lam4 + 2._dp*Lam2*Lam4 + 8*Lam3*Lam4 +       & 
&  6*Lam4*Treta1Dadjeta1D - 12._dp*(Treta1Dadjeta2Deta2Dadjeta1D) - 12._dp*(Treta1Dadjeta2Ueta1Uadjeta2D)& 
&  + 12._dp*(Treta1Dadjeta2Ueta2Uadjeta1D) + 2*Lam4*Treta1Ladjeta1L - 4._dp*(Treta1Ladjeta2Leta2Ladjeta1L)& 
&  + 6*Lam4*Treta1Uadjeta1U + 12._dp*(Treta1Uadjeta2Deta2Dadjeta1U) + 3*Lam6*Treta1Uadjeta2U +& 
&  3*Lam7*Treta1Uadjeta2U - 12._dp*(Treta1Uadjeta2Ueta2Uadjeta1U) + 3*Lam6*Treta2Dadjeta1D +& 
&  3*Lam7*Treta2Dadjeta1D - 12._dp*(Treta2Dadjeta1Ueta2Uadjeta1D) + 6*Lam4*Treta2Dadjeta2D +& 
&  Lam6*Treta2Ladjeta1L + Lam7*Treta2Ladjeta1L + 2*Lam4*Treta2Ladjeta2L + 6*Lam4*Treta2Uadjeta2U +& 
&  (10._dp*(Lam6) + 2._dp*(Lam7) + 3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L +           & 
&  3._dp*(Treta2Uadjeta1U))*Conjg(Lam6) + 2*Lam6*Conjg(Lam7) + 3*Treta1Dadjeta2D*Conjg(Lam7)& 
&  + Treta1Ladjeta2L*Conjg(Lam7) + 3*Treta2Uadjeta1U*Conjg(Lam7)

 
 
If (TwoLoopRGE) Then 
betaLam42 = 0

 
DLam4 = oo16pi2*( betaLam41 + oo16pi2 * betaLam42 ) 

 
Else 
DLam4 = oo16pi2* betaLam41 
End If 
 
 
Call Chop(DLam4) 

!-------------------- 
! Lam3 
!-------------------- 
 
betaLam31  = 2._dp*(AbsLam5) + 4._dp*(AbsLam7) + 3._dp*(g1p4)/4._dp - (3*g1p2*g2p2)   & 
& /2._dp + 9._dp*(g2p4)/4._dp + 4._dp*(Lam3p2) + 2._dp*(Lam4p2) - 3*g1p2*Lam3 -          & 
&  9*g2p2*Lam3 + 6._dp*Lam1*Lam3 + 6._dp*Lam2*Lam3 + 2._dp*Lam1*Lam4 + 2._dp*Lam2*Lam4 + & 
&  6*Lam3*Treta1Dadjeta1D - 12._dp*(Treta1Dadjeta1Deta2Dadjeta2D) + 12._dp*(Treta1Dadjeta2Ueta1Uadjeta2D)& 
&  - 12._dp*(Treta1Dadjeta2Ueta2Uadjeta1D) + 2*Lam3*Treta1Ladjeta1L - 4._dp*(Treta1Ladjeta1Leta2Ladjeta2L)& 
&  + 6*Lam3*Treta1Uadjeta1U - 12._dp*(Treta1Uadjeta1Ueta2Uadjeta2U) - 12._dp*(Treta1Uadjeta2Deta2Dadjeta1U)& 
&  + 3*Lam6*Treta1Uadjeta2U + 3*Lam7*Treta1Uadjeta2U + 3*Lam6*Treta2Dadjeta1D +          & 
&  3*Lam7*Treta2Dadjeta1D + 12._dp*(Treta2Dadjeta1Ueta2Uadjeta1D) + 6*Lam3*Treta2Dadjeta2D +& 
&  Lam6*Treta2Ladjeta1L + Lam7*Treta2Ladjeta1L + 2*Lam3*Treta2Ladjeta2L + 6*Lam3*Treta2Uadjeta2U +& 
&  (4._dp*(Lam6) + 8._dp*(Lam7) + 3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L +            & 
&  3._dp*(Treta2Uadjeta1U))*Conjg(Lam6) + 8*Lam6*Conjg(Lam7) + 3*Treta1Dadjeta2D*Conjg(Lam7)& 
&  + Treta1Ladjeta2L*Conjg(Lam7) + 3*Treta2Uadjeta1U*Conjg(Lam7)

 
 
If (TwoLoopRGE) Then 
betaLam32 = 0

 
DLam3 = oo16pi2*( betaLam31 + oo16pi2 * betaLam32 ) 

 
Else 
DLam3 = oo16pi2* betaLam31 
End If 
 
 
Call Chop(DLam3) 

!-------------------- 
! Lam2 
!-------------------- 
 
betaLam21  = 2._dp*AbsLam5 + 0.75_dp*g1p4 + 1.5_dp*g1p2*g2p2 + 2.25_dp*g2p4 +         & 
&  12._dp*Lam2p2 + 4._dp*Lam3p2 + 2._dp*Lam4p2 - 3._dp*g1p2*Lam2 - 9._dp*g2p2*Lam2 +     & 
&  4._dp*Lam3*Lam4 + 6._dp*Lam7*Treta1Uadjeta2U + 6._dp*Lam7*Treta2Dadjeta1D +           & 
&  12._dp*Lam2*Treta2Dadjeta2D - 12._dp*Treta2Dadjeta2Deta2Dadjeta2D + 2._dp*Lam7*Treta2Ladjeta1L +& 
&  4._dp*Lam2*Treta2Ladjeta2L - 4._dp*Treta2Ladjeta2Leta2Ladjeta2L + 12._dp*Lam2*Treta2Uadjeta2U -& 
&  12._dp*Treta2Uadjeta2Ueta2Uadjeta2U + (24._dp*Lam7 + 6._dp*Treta1Dadjeta2D +          & 
&  2._dp*Treta1Ladjeta2L + 6._dp*Treta2Uadjeta1U)*Conjg(Lam7)

 
 
If (TwoLoopRGE) Then 
betaLam22 = 0

 
DLam2 = oo16pi2*( betaLam21 + oo16pi2 * betaLam22 ) 

 
Else 
DLam2 = oo16pi2* betaLam21 
End If 
 
 
Call Chop(DLam2) 

!-------------------- 
! eta1U 
!-------------------- 
 
betaeta1U1  = eta1U*(-17._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)        & 
&  + 3._dp*(Treta1Dadjeta1D) + Treta1Ladjeta1L + 3._dp*(Treta1Uadjeta1U)) +              & 
&  (-3._dp*(eta1Uadjeta1Deta1D) + 3._dp*(eta1Uadjeta1Ueta1U) + eta1Uadjeta2Deta2D +      & 
&  eta1Uadjeta2Ueta2U - 4._dp*(eta2Uadjeta1Deta2D) + 2._dp*(eta2Uadjeta2Ueta1U)          & 
&  + 2*eta2U*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L))/2._dp

 
 
If (TwoLoopRGE) Then 
betaeta1U2 = 0

 
Deta1U = oo16pi2*( betaeta1U1 + oo16pi2 * betaeta1U2 ) 

 
Else 
Deta1U = oo16pi2* betaeta1U1 
End If 
 
 
Call Chop(Deta1U) 

!-------------------- 
! eta2U 
!-------------------- 
 
betaeta2U1  = (2._dp*(eta1Uadjeta1Ueta2U) - 4._dp*(eta1Uadjeta2Deta1D) +              & 
&  eta2Uadjeta1Deta1D + eta2Uadjeta1Ueta1U - 3._dp*(eta2Uadjeta2Deta2D) + 3._dp*(eta2Uadjeta2Ueta2U)& 
&  + 2*eta1U*(3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L + 3._dp*(Treta2Uadjeta1U)))      & 
& /2._dp + eta2U*(-17._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)              & 
&  + 3._dp*(Treta2Dadjeta2D) + Treta2Ladjeta2L + 3._dp*(Treta2Uadjeta2U))

 
 
If (TwoLoopRGE) Then 
betaeta2U2 = 0

 
Deta2U = oo16pi2*( betaeta2U1 + oo16pi2 * betaeta2U2 ) 

 
Else 
Deta2U = oo16pi2* betaeta2U1 
End If 
 
 
Call Chop(Deta2U) 

!-------------------- 
! eta1D 
!-------------------- 
 
betaeta1D1  = eta1D*(-5._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)         & 
&  + 3._dp*(Treta1Dadjeta1D) + Treta1Ladjeta1L + 3._dp*(Treta1Uadjeta1U)) +              & 
&  (3._dp*(eta1Dadjeta1Deta1D) - 3._dp*(eta1Dadjeta1Ueta1U) + eta1Dadjeta2Deta2D +       & 
&  eta1Dadjeta2Ueta2U - 4._dp*(eta2Dadjeta1Ueta2U) + 2._dp*(eta2Dadjeta2Deta1D)          & 
&  + 2*eta2D*(3._dp*(Treta1Dadjeta2D) + Treta1Ladjeta2L + 3._dp*(Treta2Uadjeta1U)))/2._dp

 
 
If (TwoLoopRGE) Then 
betaeta1D2 = 0

 
Deta1D = oo16pi2*( betaeta1D1 + oo16pi2 * betaeta1D2 ) 

 
Else 
Deta1D = oo16pi2* betaeta1D1 
End If 
 
 
Call Chop(Deta1D) 

!-------------------- 
! eta1L 
!-------------------- 
 
betaeta1L1  = (3._dp*(eta1Ladjeta1Leta1L) + eta1Ladjeta2Leta2L + 2._dp*(eta2Ladjeta2Leta1L))& 
& /2._dp + eta1L*(-15._dp*(g1p2)/4._dp - 9._dp*(g2p2)/4._dp + 3._dp*(Treta1Dadjeta1D)    & 
&  + Treta1Ladjeta1L + 3._dp*(Treta1Uadjeta1U)) + eta2L*(3._dp*(Treta1Dadjeta2D)         & 
&  + Treta1Ladjeta2L + 3._dp*(Treta2Uadjeta1U))

 
 
If (TwoLoopRGE) Then 
betaeta1L2 = 0

 
Deta1L = oo16pi2*( betaeta1L1 + oo16pi2 * betaeta1L2 ) 

 
Else 
Deta1L = oo16pi2* betaeta1L1 
End If 
 
 
Call Chop(Deta1L) 

!-------------------- 
! eta2D 
!-------------------- 
 
betaeta2D1  = (2._dp*(eta1Dadjeta1Deta2D) - 4._dp*(eta1Dadjeta2Ueta1U) +              & 
&  eta2Dadjeta1Deta1D + eta2Dadjeta1Ueta1U + 3._dp*(eta2Dadjeta2Deta2D) - 3._dp*(eta2Dadjeta2Ueta2U)& 
&  + 2*eta1D*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L))      & 
& /2._dp + eta2D*(-5._dp*(g1p2)/12._dp - 9._dp*(g2p2)/4._dp - 8._dp*(g3p2)               & 
&  + 3._dp*(Treta2Dadjeta2D) + Treta2Ladjeta2L + 3._dp*(Treta2Uadjeta2U))

 
 
If (TwoLoopRGE) Then 
betaeta2D2 = 0

 
Deta2D = oo16pi2*( betaeta2D1 + oo16pi2 * betaeta2D2 ) 

 
Else 
Deta2D = oo16pi2* betaeta2D1 
End If 
 
 
Call Chop(Deta2D) 

!-------------------- 
! eta2L 
!-------------------- 
 
betaeta2L1  = (2._dp*(eta1Ladjeta1Leta2L) + eta2Ladjeta1Leta1L + 3._dp*(eta2Ladjeta2Leta2L))& 
& /2._dp + eta1L*(3._dp*(Treta1Uadjeta2U) + 3._dp*(Treta2Dadjeta1D) + Treta2Ladjeta1L)   & 
&  + eta2L*(-15._dp*(g1p2)/4._dp - 9._dp*(g2p2)/4._dp + 3._dp*(Treta2Dadjeta2D)          & 
&  + Treta2Ladjeta2L + 3._dp*(Treta2Uadjeta2U))

 
 
If (TwoLoopRGE) Then 
betaeta2L2 = 0

 
Deta2L = oo16pi2*( betaeta2L1 + oo16pi2 * betaeta2L2 ) 

 
Else 
Deta2L = oo16pi2* betaeta2L1 
End If 
 
 
Call Chop(Deta2L) 

!-------------------- 
! M12 
!-------------------- 
 
betaM121  = -6*Lam6*M112 - (3*g1p2*M12)/2._dp - (9*g2p2*M12)/2._dp + 2*Lam3*M12 +     & 
&  4*Lam4*M12 - 6*Lam7*M222 + 3*M12*Treta1Dadjeta1D + M12*Treta1Ladjeta1L +              & 
&  3*M12*Treta1Uadjeta1U + 3*M12*Treta2Dadjeta2D + M12*Treta2Ladjeta2L + 3*M12*Treta2Uadjeta2U +& 
&  6*Conjg(Lam5)*Conjg(M12)

 
 
If (TwoLoopRGE) Then 
betaM122 = 0

 
DM12 = oo16pi2*( betaM121 + oo16pi2 * betaM122 ) 

 
Else 
DM12 = oo16pi2* betaM121 
End If 
 
 
Call Chop(DM12) 

!-------------------- 
! M112 
!-------------------- 
 
betaM1121  = (-3*g1p2*M112)/2._dp - (9*g2p2*M112)/2._dp + 6._dp*Lam1*M112 +           & 
&  4*Lam3*M222 + 2*Lam4*M222 + 6*M112*Treta1Dadjeta1D + 2*M112*Treta1Ladjeta1L +         & 
&  6*M112*Treta1Uadjeta1U - 6*M12*Conjg(Lam6) - 6*Lam6*Conjg(M12)

 
 
If (TwoLoopRGE) Then 
betaM1122 = 0

 
DM112 = oo16pi2*( betaM1121 + oo16pi2 * betaM1122 ) 

 
Else 
DM112 = oo16pi2* betaM1121 
End If 
 
 
Call Chop(DM112) 

!-------------------- 
! M222 
!-------------------- 
 
betaM2221  = 4*Lam3*M112 + 2*Lam4*M112 - (3*g1p2*M222)/2._dp - (9*g2p2*M222)          & 
& /2._dp + 6._dp*Lam2*M222 + 6*M222*Treta2Dadjeta2D + 2*M222*Treta2Ladjeta2L +           & 
&  6*M222*Treta2Uadjeta2U - 6*M12*Conjg(Lam7) - 6*Lam7*Conjg(M12)

 
 
If (TwoLoopRGE) Then 
betaM2222 = 0

 
DM222 = oo16pi2*( betaM2221 + oo16pi2 * betaM2222 ) 

 
Else 
DM222 = oo16pi2* betaM2221 
End If 
 
 
Call Chop(DM222) 

!-------------------- 
! vd 
!-------------------- 
 
betavd1  = (3*g1p2*vd + 9*g2p2*vd - 12*Treta1Dadjeta1D*vd - 4*Treta1Ladjeta1L*vd -    & 
&  12*Treta1Uadjeta1U*vd - 6*Treta1Dadjeta2D*vu - 2*Treta1Ladjeta2L*vu - 6*Treta1Uadjeta2U*vu -& 
&  6*Treta2Dadjeta1D*vu - 2*Treta2Ladjeta1L*vu - 6*Treta2Uadjeta1U*vu + g1p2*vd*Xi +     & 
&  3*g2p2*vd*Xi)/4._dp

 
 
If (TwoLoopRGE) Then 
betavd2 = 0

 
Dvd = oo16pi2*( betavd1 + oo16pi2 * betavd2 ) 

 
Else 
Dvd = oo16pi2* betavd1 
End If 
 
 
!-------------------- 
! vu 
!-------------------- 
 
betavu1  = (-6*Treta1Dadjeta2D*vd - 2*Treta1Ladjeta2L*vd - 6*Treta1Uadjeta2U*vd -     & 
&  6*Treta2Dadjeta1D*vd - 2*Treta2Ladjeta1L*vd - 6*Treta2Uadjeta1U*vd + 3*g1p2*vu +      & 
&  9*g2p2*vu - 12*Treta2Dadjeta2D*vu - 4*Treta2Ladjeta2L*vu - 12*Treta2Uadjeta2U*vu +    & 
&  g1p2*vu*Xi + 3*g2p2*vu*Xi)/4._dp

 
 
If (TwoLoopRGE) Then 
betavu2 = 0

 
Dvu = oo16pi2*( betavu1 + oo16pi2 * betavu2 ) 

 
Else 
Dvu = oo16pi2* betavu1 
End If 
 
 
Call ParametersToG133(Dg1,Dg2,Dg3,DLam6,DLam5,DLam7,DLam1,DLam4,DLam3,DLam2,          & 
& Deta1U,Deta2U,Deta1D,Deta1L,Deta2D,Deta2L,DM12,DM112,DM222,Dvd,Dvu,f)

Iname = Iname - 1 
 
End Subroutine rge133  

End Module RGEs_THDM_GEN 
 
