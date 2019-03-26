// Globals.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Globals
 * @author: Joel Oredsson
 *============================================================================*/
#include "Globals.h"

#include <vector>
#include <cmath>

namespace THDME
{
constexpr double Global::Qf[3];

double get_qmass_pole(int flav)
{
  static const std::vector<double> mQ_pole = {Global::md, Global::mu, Global::ms, Global::mc, Global::mb, Global::mt};

  if (flav < 1 || flav > 6)
    return 0.;
  else
  {
    return mQ_pole[flav - 1];
  }
}

double get_qmass_MSbar(int flav)
{
  // Evaluation of running mass from pole mass
  // arXiv:0712.1419
  constexpr static bool b_HD = false;
  /// Default value for running quark mass at 5 GeV m_s(5) (GeV)
  constexpr static double ms_5 = 0.08160951656;
  /// Default value for running quark mass at 5 GeV m_c(5) (GeV)
  constexpr static double mc_5 = 0.8960809463;
  /// Default value for running quark mass at 5 GeV m_b(5) (GeV)
  constexpr static double mb_5 = 4.310679848;
  /// Default value for running quark mass at 5 GeV m_t(5) (GeV)
  constexpr static double mt_5 = 246.5552058;

  static double m_qmass_ms[7] = {-1., -1., -1., -1., -1., -1., -1.};

  int loops = 1;

  if ((flav > 6) || (flav < 1))
    return 0;

  if ((!b_HD) && (flav <= 3))
    return get_qmass_pole(flav);
  if ((b_HD) && (flav <= 2))
    return get_qmass_pole(flav);
  // Lookup table
  if (m_qmass_ms[flav] >= 0.)
    return m_qmass_ms[flav];

  double Kq2 = 0.0;
  double Kq3 = 0.0;
  double mq_p = get_qmass_pole(flav);
  double mb = get_qmass_pole(5);
  double mt = get_qmass_pole(6);

  if (flav == 4)
  {
    Kq2 = 11.21;
    Kq3 = 123.8;
  }
  if (flav == 5)
  {
    Kq2 = 10.17;
    Kq3 = 101.5;
  }
  if (flav == 6)
  {
    Kq2 = 9.13;
    Kq3 = 80.4;
  }

  double as = run_alphas_MSbar(mq_p, get_qmass_pole(6), get_qmass_pole(5)) / M_PI;

  // 1-loop conversion
  double mq_msbar = mq_p / (1. + 4. / 3. * as);

  // Change to higher loops
  if (loops == 2)
  {
    mq_msbar = mq_p / (1. + 4. / 3. * as + Kq2 * std::pow(as, 2));
  }
  else if (loops == 3)
  {
    mq_msbar = mq_p / (1. + 4. / 3. * as + Kq2 * pow(as, 2) + Kq3 * pow(as, 3));
  }

  // Run to mq(mq)
  mq_msbar = run_qmass_MSbar(mq_msbar, mq_p, mq_msbar, mt, mb);

  //  HD comparison
  if ((b_HD) && (flav == 3))
  {
    mq_msbar = ms_5;
  }
  if ((b_HD) && (flav == 4))
  {
    mq_msbar = mc_5;
  }
  if ((b_HD) && (flav == 5))
  {
    mq_msbar = mb_5;
  }
  if ((b_HD) && (flav == 6))
  {
    mq_msbar = mt_5;
  }

  m_qmass_ms[flav] = mq_msbar;

  return mq_msbar;
}

double get_lmass_pole(int l)
{
  static const std::vector<double> ml = {Global::me, Global::mmuon, Global::mtau};
  if (l < 1 || l > 3)
    return 0.;
  else
    return ml[l - 1];
}

double get_dmass_pole(int d)
{
  static const std::vector<double> md = {Global::md, Global::ms, Global::mb};
  if (d < 1 || d > 3)
    return 0.;
  else
    return md[d - 1];
}

double get_umass_pole(int u)
{
  static const std::vector<double> mu = {Global::mu, Global::mc, Global::mt};
  if (u < 1 || u > 3)
    return 0.;
  else
    return mu[u - 1];
}

double get_dmass_MSbar(int dq)
{
  if ((dq >= 1) && (dq <= 3))
    return get_qmass_MSbar(2 * dq - 1);
  else
    return 0.;
}

double get_umass_MSbar(int dq)
{
  if ((dq >= 1) && (dq <= 3))
    return get_qmass_MSbar(2 * dq);
  else
    return 0.;
}

double QCD_beta(int c, int nf)
{

  if ((c < 0) || (c > 2))
    return 0.;

  double beta = -1;

  switch (c)
  {

  case 0:
    beta = 11. - 2. * nf / 3.;
    break;
  case 1:
    beta = 51. - 19. * nf / 3.;
    break;
  case 2:
    beta = 2857. - 5033. * nf / 9. + 325. * nf * nf / 27.;
    break;
  }

  return beta;
}

// Quark mass evolution function R.
// Adapted from arXiv:0712.1419
double RQ(double as, int nf, int nloops = 3)
{

  const double zeta3 = 1.202056903;
  const double zeta4 = 1.082323234;
  const double zeta5 = 1.036927755;

  double beta[4] = {0., 0., 0., 0.};
  double gamma[4] = {0., 0., 0., 0.};

  if (nloops > 4)
    nloops = 3;

  if (nloops >= 1)
  {
    beta[0] = 1. / 4. * (11. - 2. / 3. * nf);
    gamma[0] = 1.;
  }
  if (nloops >= 2)
  {
    beta[1] = 1. / 16. * (102. - 38. / 3. * nf);
    gamma[1] = 1. / 16. * (202. / 3. - 20. / 9. * nf);
  }
  if (nloops >= 3)
  {
    beta[2] = 1. / 64. * (2857. / 2. - 5033. / 18. * nf + 325. / 54. * nf * nf);
    gamma[2] = 1. / 64. * (1249. - (2216. / 27. + 160. / 3. * zeta3) * nf - 140. / 81. * nf * nf);
  }
  if (nloops >= 4)
  {
    beta[3] = 1. / 256. * (149753. / 6. + 3564. * zeta3 - (1078361. / 162. + 6508. / 27. * zeta3) * nf + (50065. / 162. + 6472. / 81. * zeta3) * nf * nf + 1093. / 729. * nf * nf * nf);
    gamma[3] = 1. / 256. * (4603055. / 162. + 135680. / 27. * zeta3 - 8800. * zeta5 - (91723. / 27. + 34192. / 9. - 880. * zeta4 - 18400. / 9. * zeta5) * nf + (5242. / 243. + 800. / 9. * zeta3 - 160. / 3. * zeta4) * nf * nf - (332. / 243. - 64. / 27. * zeta3) * nf * nf);
  }

  double ras = as / M_PI;

  double C1 = gamma[1] / beta[0] - beta[1] * gamma[0] / pow(beta[0], 2);
  double C2 = gamma[2] / beta[0] - beta[1] * gamma[1] / pow(beta[0], 2) - beta[2] * gamma[0] / pow(beta[0], 2) + pow(beta[1], 2) * gamma[0] / pow(beta[0], 3);
  //  double C3 = gamma[3]/beta[0]-beta[1]*gamma[2]/pow(beta[0],2)+pow(beta[1],2)*gamma[1]/pow(beta[0],3)-beta[2]*gamma[1]/pow(beta[0],2)-pow(beta[1],3)*gamma[0]/pow(beta[0],4)+2.*beta[1]*beta[2]*gamma[0]/pow(beta[0],3)-beta[3]*gamma[0]/pow(beta[0],2);

  double R = pow(ras, gamma[0] / beta[0]) * (1. + ras * C1 + 0.5 * pow(ras, 2) * (pow(C1, 2) + C2));
  //  double R = pow(ras,gamma[0]/beta[0])*(1.+ras*C1+pow(ras,2)*(pow(C1,2)+C2)+pow(ras,3)*(1./6.*pow(C1,3)+1./2.*C1*C2+1./3.*C3));

  return R;
}

double run_qmass_MSbar(double quark_mass, double Qinit, double Qfin, double mtop, double mbot)
{
  // Joel: This functions does nothing by default in 2HDMC
  auto m_threshold = [](double as) {
    return 1.;
  };

  // Running of quark masses

  double R_Qinit,
      R_Qfin, running_mass;

  double alphas_Qinit = run_alphas_MSbar(Qinit, mtop, mbot);
  double alphas_Qfin = run_alphas_MSbar(Qfin, mtop, mbot);
  double mcharm = get_qmass_pole(4);

  // Are we asking for the impossible?
  if ((alphas_Qinit != alphas_Qinit) || (alphas_Qfin != alphas_Qfin))
    return quark_mass;

  int nf = 0;

  if (Qinit < mcharm)
  {
    nf = 3;
    R_Qinit = RQ(alphas_Qinit, nf);

    if (Qfin <= mcharm)
    {
      R_Qfin = RQ(alphas_Qfin, nf);
      return R_Qfin / R_Qinit * quark_mass;
    }
    else
    {
      alphas_Qfin = run_alphas_MSbar(mcharm, mtop, mbot);
      R_Qfin = RQ(alphas_Qfin, nf);
      running_mass = R_Qfin / R_Qinit * quark_mass;
      double K = 1. / m_threshold(alphas_Qfin);
      return run_qmass_MSbar(K * running_mass, mcharm, Qfin, mtop, mbot);
    }
  }
  else if ((Qinit >= mcharm) && (Qinit < mbot))
  {
    nf = 4;
    R_Qinit = RQ(alphas_Qinit, nf);

    if ((Qfin >= mcharm) && (Qfin < mbot))
    {
      R_Qfin = RQ(alphas_Qfin, nf);
      return R_Qfin / R_Qinit * quark_mass;
    }
    else if (Qfin >= mbot)
    {
      alphas_Qfin = run_alphas_MSbar(mbot, mtop, mbot);
      R_Qfin = RQ(alphas_Qfin, nf);
      running_mass = R_Qfin / R_Qinit * quark_mass;
      double K = 1. / m_threshold(alphas_Qfin);
      return run_qmass_MSbar(K * running_mass, mbot, Qfin, mtop, mbot);
    }
    else if (Qfin < mcharm)
    {
      alphas_Qfin = run_alphas_MSbar(mcharm, mtop, mbot);
      R_Qfin = RQ(alphas_Qfin, nf);
      running_mass = R_Qfin / R_Qinit * quark_mass;
      double K = m_threshold(alphas_Qfin);
      return run_qmass_MSbar(K * running_mass, mcharm - 1.E-12, Qfin, mtop, mbot);
    }
  }
  else if ((Qinit >= mbot) && (Qinit < mtop))
  {
    nf = 5;
    R_Qinit = RQ(alphas_Qinit, nf);

    if ((Qfin >= mbot) && (Qfin < mtop))
    {
      R_Qfin = RQ(alphas_Qfin, nf);
      return R_Qfin / R_Qinit * quark_mass;
    }
    else if (Qfin >= mtop)
    {
      alphas_Qfin = run_alphas_MSbar(mtop, mtop, mbot);
      R_Qfin = RQ(alphas_Qfin, nf);
      running_mass = R_Qfin / R_Qinit * quark_mass;
      double K = 1. / m_threshold(alphas_Qfin);
      return run_qmass_MSbar(K * running_mass, mtop, Qfin, mtop, mbot);
    }
    else if (Qfin < mbot)
    {
      alphas_Qfin = run_alphas_MSbar(mbot, mtop, mbot);
      R_Qfin = RQ(alphas_Qfin, nf);
      running_mass = R_Qfin / R_Qinit * quark_mass;
      double K = m_threshold(alphas_Qfin);
      return run_qmass_MSbar(K * running_mass, mbot - 1.E-12, Qfin, mtop, mbot);
    }
  }
  else if (Qinit >= mtop)
  {
    nf = 6;
    R_Qinit = RQ(alphas_Qinit, nf);

    if (Qfin >= mtop)
    {
      R_Qfin = RQ(alphas_Qfin, nf);
      return R_Qfin / R_Qinit * quark_mass;
    }
    else
    {
      alphas_Qfin = run_alphas_MSbar(mtop, mtop, mbot);
      R_Qfin = RQ(alphas_Qfin, nf);
      running_mass = R_Qfin / R_Qinit * quark_mass;
      double K = m_threshold(alphas_Qfin);
      return run_qmass_MSbar(K * running_mass, mtop - 1.E-12, Qfin, mtop, mbot);
    }
  }

  return 0.;
}

double run_alphas_MSbar(double Q, double mtop, double mbot)
{
  double MZ = Global::mZ;
  double alphas_MZ = Global::alpha_s_mZ;

  // Only five flavours is used for the running of alpha_s in agreement with HD
  double beta0, beta1, beta2, alpha_s_running, Lambda4, Lambda5, Lambda_min, Lambda_max, Lambda_moy, alphas_min, alphas_moy;

  int nf;

  double pi = M_PI;

  nf = 5;
  beta0 = QCD_beta(0, nf);
  beta1 = QCD_beta(1, nf);
  beta2 = QCD_beta(2, nf);

  Lambda_min = 1.e-3;
  Lambda_max = 1.;
  alphas_min = 0.;

  while (fabs(1. - alphas_min / alphas_MZ) >= 1.e-6)
  {
    alphas_min = 4. * pi / beta0 / log(pow(MZ / Lambda_min, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(MZ / Lambda_min, 2.))) / log(pow(MZ / Lambda_min, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(MZ / Lambda_min, 2.)), 2.) * (pow(log(log(pow(MZ / Lambda_min, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

    Lambda_moy = (Lambda_min + Lambda_max) / 2.;
    alphas_moy = 4. * pi / beta0 / log(pow(MZ / Lambda_moy, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(MZ / Lambda_moy, 2.))) / log(pow(MZ / Lambda_moy, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(MZ / Lambda_moy, 2.)), 2.) * (pow(log(log(pow(MZ / Lambda_moy, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

    if ((alphas_MZ >= alphas_min) && (alphas_MZ <= alphas_moy))
      Lambda_max = Lambda_moy;
    else
      Lambda_min = Lambda_moy;
  }

  Lambda5 = Lambda_min;

  // only use five flavours in alpha_s running for agreement with Hdecay
  //  if((Q<=mtop)&&(Q>=mbot))
  if (Q >= mbot)
  {
    /* 5 active flavors */
    alpha_s_running = 4. * pi / beta0 / log(pow(Q / Lambda5, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(Q / Lambda5, 2.))) / log(pow(Q / Lambda5, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(Q / Lambda5, 2.)), 2.) * (pow(log(log(pow(Q / Lambda5, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));
    return alpha_s_running;

    /*** These aren't the flavours you are looking for - please move along ***
  } if((Q>mtop)) {
    // 6 active flavors 
    alpha_s_match=4.*pi/beta0/log(pow(mtop/Lambda5,2.))*(1.-2.*beta1/beta0/beta0*log(log(pow(mtop/Lambda5,2.)))/log(pow(mtop/Lambda5,2.))+4.*beta1*beta1/pow(beta0*beta0*log(pow(mtop/Lambda5,2.)),2.)*(pow(log(log(pow(mtop/Lambda5,2.)))-1./2.,2.)+beta2*beta0/8./beta1/beta1-5./4.));
  
    nf=6;
    beta0 = QCD_beta(0,nf);
    beta1 = QCD_beta(1,nf);
    beta2 = QCD_beta(2,nf);
  
    Lambda_min=1.e-3;
    Lambda_max=1.;
    alphas_min=0.;
  
    while(fabs(1.-alphas_min/alpha_s_match)>=1.e-5) {
      alphas_min=4.*pi/beta0/log(pow(mtop/Lambda_min,2.))*(1.-2.*beta1/beta0/beta0*log(log(pow(mtop/Lambda_min,2.)))/log(pow(mtop/Lambda_min,2.))+4.*beta1*beta1/pow(beta0*beta0*log(pow(mtop/Lambda_min,2.)),2.)*(pow(log(log(pow(mtop/Lambda_min,2.)))-1./2.,2.)+beta2*beta0/8./beta1/beta1-5./4.));
    
	  Lambda_moy=(Lambda_min+Lambda_max)/2.;
      alphas_moy=4.*pi/beta0/log(pow(mtop/Lambda_moy,2.))*(1.-2.*beta1/beta0/beta0*log(log(pow(mtop/Lambda_moy,2.)))/log(pow(mtop/Lambda_moy,2.))+4.*beta1*beta1/pow(beta0*beta0*log(pow(mtop/Lambda_moy,2.)),2.)*(pow(log(log(pow(mtop/Lambda_moy,2.)))-1./2.,2.)+beta2*beta0/8./beta1/beta1-5./4.));
    
      if((alpha_s_match>=alphas_min)&&(alpha_s_match<=alphas_moy))
        Lambda_max=Lambda_moy;
	  else Lambda_min=Lambda_moy;
	}

    Lambda6=Lambda_min;

    alpha_s_running=4.*pi/beta0/log(pow(Q/Lambda6,2.))*(1.-2.*beta1/beta0/beta0*log(log(pow(Q/Lambda6,2.)))/log(pow(Q/Lambda6,2.))+4.*beta1*beta1/pow(beta0*beta0*log(pow(Q/Lambda6,2.)),2.)*(pow(log(log(pow(Q/Lambda6,2.)))-1./2.,2.)+beta2*beta0/8./beta1/beta1-5./4.));
    return alpha_s_running;
*/
  }
  else
  {
    /* 4 active flavors */

    alpha_s_running = 4. * pi / beta0 / log(pow(mbot / Lambda5, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mbot / Lambda5, 2.))) / log(pow(mbot / Lambda5, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mbot / Lambda5, 2.)), 2.) * (pow(log(log(pow(mbot / Lambda5, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

    //  1-loop ratio between pole mass and MSbar mass at pole mass
    double mratio = 1 + 4. / 3. * alpha_s_running / pi;

    double alpha_s_msbar = 4. * pi / beta0 / log(pow(mbot / mratio / Lambda5, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mbot / mratio / Lambda5, 2.))) / log(pow(mbot / mratio / Lambda5, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mbot / mratio / Lambda5, 2.)), 2.) * (pow(log(log(pow(mbot / mratio / Lambda5, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

    double R_Qinit = RQ(alpha_s_running, 4);
    double R_Qfin = RQ(alpha_s_msbar, 4);
    mratio = mratio / R_Qfin * R_Qinit;

    double xi_factor = 1. - alpha_s_running / pi / 6. * log(pow(mratio, 2)) + pow(alpha_s_running / pi, 2) * (11. / 12. - 11. / 24. * log(pow(mratio, 2)) + 1. / 36. * pow(log(pow(mratio, 2)), 2));

    double alpha_s_match = alpha_s_running * pow(xi_factor, 2);

    nf = 4;
    beta0 = QCD_beta(0, nf);
    beta1 = QCD_beta(1, nf);
    beta2 = QCD_beta(2, nf);

    Lambda_min = 1.e-3;
    Lambda_max = 1.;
    alphas_min = 0.;

    while (fabs(1. - alphas_min / alpha_s_match) >= 1.e-5)
    {
      alphas_min = 4. * pi / beta0 / log(pow(mbot / Lambda_min, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mbot / Lambda_min, 2.))) / log(pow(mbot / Lambda_min, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mbot / Lambda_min, 2.)), 2.) * (pow(log(log(pow(mbot / Lambda_min, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

      Lambda_moy = (Lambda_min + Lambda_max) / 2.;
      alphas_moy = 4. * pi / beta0 / log(pow(mbot / Lambda_moy, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mbot / Lambda_moy, 2.))) / log(pow(mbot / Lambda_moy, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mbot / Lambda_moy, 2.)), 2.) * (pow(log(log(pow(mbot / Lambda_moy, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

      if ((alpha_s_match >= alphas_min) && (alpha_s_match <= alphas_moy))
        Lambda_max = Lambda_moy;
      else
        Lambda_min = Lambda_moy;
    }

    Lambda4 = Lambda_min;

    alpha_s_running = 4. * pi / beta0 / log(pow(Q / Lambda4, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(Q / Lambda4, 2.))) / log(pow(Q / Lambda4, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(Q / Lambda4, 2.)), 2.) * (pow(log(log(pow(Q / Lambda4, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

    if (Q <= get_qmass_pole(4))
    {
      /* 3 active flavors */
      double mcharm = get_qmass_pole(4);

      alpha_s_running = 4. * pi / beta0 / log(pow(mcharm / Lambda4, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mcharm / Lambda4, 2.))) / log(pow(mcharm / Lambda4, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mcharm / Lambda4, 2.)), 2.) * (pow(log(log(pow(mcharm / Lambda4, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

      //  1-loop ratio between pole mass and MSbar mass at pole mass
      double mratio = 1 + 4. / 3. * alpha_s_running / pi;

      double alpha_s_msbar = 4. * pi / beta0 / log(pow(mcharm / mratio / Lambda4, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mcharm / mratio / Lambda4, 2.))) / log(pow(mcharm / mratio / Lambda4, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mcharm / mratio / Lambda4, 2.)), 2.) * (pow(log(log(pow(mcharm / mratio / Lambda4, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

      //  run MSbar mass to itself i.e. (m(m(M))
      double R_Qinit = RQ(alpha_s_running, 3);
      double R_Qfin = RQ(alpha_s_msbar, 3);
      mratio = mratio / R_Qfin * R_Qinit;

      double xi_factor = 1. - alpha_s_running / pi / 6. * log(pow(mratio, 2)) + pow(alpha_s_running / pi, 2) * (11. / 12. - 11. / 24. * log(pow(mratio, 2)) + 1. / 36. * pow(log(pow(mratio, 2)), 2));

      double alpha_s_match = alpha_s_running * pow(xi_factor, 2);

      nf = 3;
      beta0 = QCD_beta(0, nf);
      beta1 = QCD_beta(1, nf);
      beta2 = QCD_beta(2, nf);

      Lambda_min = 1.e-3;
      Lambda_max = 1.;
      alphas_min = 0.;

      while (fabs(1. - alphas_min / alpha_s_match) >= 1.e-5)
      {
        alphas_min = 4. * pi / beta0 / log(pow(mcharm / Lambda_min, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mcharm / Lambda_min, 2.))) / log(pow(mcharm / Lambda_min, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mcharm / Lambda_min, 2.)), 2.) * (pow(log(log(pow(mcharm / Lambda_min, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

        Lambda_moy = (Lambda_min + Lambda_max) / 2.;
        alphas_moy = 4. * pi / beta0 / log(pow(mcharm / Lambda_moy, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(mcharm / Lambda_moy, 2.))) / log(pow(mcharm / Lambda_moy, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(mcharm / Lambda_moy, 2.)), 2.) * (pow(log(log(pow(mcharm / Lambda_moy, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));

        if ((alpha_s_match >= alphas_min) && (alpha_s_match <= alphas_moy))
          Lambda_max = Lambda_moy;
        else
          Lambda_min = Lambda_moy;
      }

      double Lambda3 = Lambda_min;
      alpha_s_running = 4. * pi / beta0 / log(pow(Q / Lambda3, 2.)) * (1. - 2. * beta1 / beta0 / beta0 * log(log(pow(Q / Lambda3, 2.))) / log(pow(Q / Lambda3, 2.)) + 4. * beta1 * beta1 / pow(beta0 * beta0 * log(pow(Q / Lambda3, 2.)), 2.) * (pow(log(log(pow(Q / Lambda3, 2.))) - 1. / 2., 2.) + beta2 * beta0 / 8. / beta1 / beta1 - 5. / 4.));
    }

    return alpha_s_running;
  }
}
double get_gamma_tWd(int d)
 {
   auto vCkm = get_pdg_ckm_matrix();
  if ((d<1)||(d>3)) return 0.;

  std::complex<double> VCKM = vCkm(2, d-1);
  double GF = Global::GF; 
  double mt = get_umass_pole(3);
  double md = get_dmass_pole(d);
  double mb = get_dmass_pole(3);
  double mw = Global::mW;

  double x = mw/mt;
  double y = md/mt;

  double l = (1.-pow(x+y,2))*(1.-pow(x-y,2));
  double I = ((1.-pow(x,2))*(1.+2.*pow(x,2))-pow(y,2)*(2.-pow(x,2)-pow(y,2)))*sqrt(l);

  double gtot = GF*pow(mt,3)/(8.*M_PI*sqrt(2))*std::norm(VCKM)*I;

  double as = run_alphas_MSbar(mt,mt,mb);
  double CF = 4./3.;
  double K = 1.+CF*as/M_PI*(5./4.-M_PI*M_PI/3.);

  gtot = gtot*K;

  return gtot;

}

Eigen::Matrix3cd get_pdg_ckm_matrix()
{
  Eigen::Matrix3cd CKM_input;
  CKM_input.setZero();

  // Parameterisation of CKM matrix from PDG 2018.
  constexpr static double lambda = 0.22453;
  constexpr static double A = 0.836;
  constexpr static double rhobar = 0.122;
  constexpr static double etabar = 0.355;

  static const double s12 = lambda;
  static const double s23 = A * lambda * lambda;
  static const double c12 = sqrt(1. - s12 * s12);
  static const double c23 = sqrt(1. - s23 * s23);
  static const double preFactor =
      (A * pow(lambda, 3) * sqrt(1. - A * A * pow(lambda, 4))) /
      ((1. - lambda * lambda) *
       (1. - 2. * A * A * pow(lambda, 4) * rhobar +
        pow(A, 4) * pow(lambda, 8) * (pow(rhobar, 2) + pow(etabar, 2))));
  static const std::complex<double> s13eid = std::complex<double>(
      preFactor *
          (rhobar - A * A * pow(lambda, 4) * (pow(rhobar, 2) + pow(etabar, 2))),
      preFactor * etabar);
  static const double c13 = std::sqrt(std::abs(1. - std::abs(s13eid) * std::abs(s13eid)));

  CKM_input(0, 0) = std::complex<double>(c12 * c13, 0.);
  CKM_input(0, 1) = std::complex<double>(s12 * c13, 0.);
  CKM_input(0, 2) = conj(s13eid);
  CKM_input(1, 0) = std::complex<double>(-s12 * c23 - c12 * s23 * real(s13eid),
                                         -c12 * s23 * imag(s13eid));
  CKM_input(1, 1) = std::complex<double>(c12 * c23 - s12 * s23 * real(s13eid),
                                         -s12 * s23 * imag(s13eid));
  CKM_input(1, 2) = std::complex<double>(s23 * c13, 0.);
  CKM_input(2, 0) = std::complex<double>(s12 * s23 - c12 * c23 * real(s13eid),
                                         -c12 * c23 * imag(s13eid));
  CKM_input(2, 1) = std::complex<double>(-c12 * s23 - s12 * c23 * real(s13eid),
                                         -s12 * c23 * imag(s13eid));
  CKM_input(2, 2) = std::complex<double>(c23 * c13, 0.);

  return CKM_input;
}

int get_Nactivef(double M) {
  int nf = 0;
  for (int i=1;i<=6;i++) {
    if (get_qmass_pole(i)<M) nf++;
  }

  return nf;

}
} // namespace THDME