// Oblique.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
#include "Oblique.h"

#include <cmath>
#include <cstdio>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>

namespace THDME
{

double calcS(const std::vector<double> &mi, const std::vector<double> &qi12)
{
  constexpr static double mZ = 91.15349;
  constexpr static double mZ2 = mZ * mZ;
  constexpr static double mPhi = 125.;

  constexpr static double prefactor = 1. / (M_PI * mZ * mZ);
  double s = 0.;

  if (mi.size() != 4 || qi12.size() < 3)
  {
    printf("[ERROR]: Wrong arguments when calculating S!\n");
    return 0.;
  }

  for (int i = 0; i < 3; ++i)
  {
    s += qi12[i] * (PVB(22, mZ, mZ, mi[i]) - mZ2 * PVB(0, mZ, mZ, mi[i]));
  }

  s += qi12[0] * PVB(22, mZ, mi[1], mi[2]);
  s += qi12[1] * PVB(22, mZ, mi[0], mi[2]);
  s += qi12[2] * PVB(22, mZ, mi[0], mi[1]);

  s -= PVB(22, mZ, mi[3], mi[3]);
  s -= PVB(22, mZ, mZ, mPhi);
  s += mZ2 * PVB(0, mZ, mZ, mPhi);

  s *= prefactor;

  // printf("S = %16.8e\n", s); // DEBUG

  return s;
}

double calcT(const std::vector<double> &mi, const std::vector<double> &qi12,
             const std::vector<double> &qi22)
{
  constexpr static double mZ = 91.15349;
  constexpr static double mW = 80.36951;
  constexpr static double mPhi = 125.;
  constexpr static double sW2 = 1. - mW * mW / (mZ * mZ);
  constexpr static double prefactor = 1. / (16. * M_PI * mW * mW * sW2);

  if (mi.size() != 4 || qi12.size() < 3 || qi22.size() < 3)
  {
    printf("[ERROR]: Wrong arguments when calculating T!\n");
    return 0.;
  }

  double t = 0;

  for (int i = 0; i < 3; ++i)
  {
    t += qi22[i] * Fm1m2(mi[3], mi[i]) +
         3. * qi12[i] * (Fm1m2(mZ, mi[i]) - Fm1m2(mW, mi[i]));
  }
  t += -qi12[0] * Fm1m2(mi[1], mi[2]) - qi12[1] * Fm1m2(mi[0], mi[2]) -
       qi12[2] * Fm1m2(mi[0], mi[1]) - 3. * (Fm1m2(mZ, mPhi) - Fm1m2(mW, mPhi));

  t *= prefactor;

  // printf("T = %16.8e\n", t); // DEBUG
  return t;
}

double calcU(const std::vector<double> &mi, const std::vector<double> &qi12,
             const std::vector<double> &qi22)
{
  constexpr static double mZ = 91.15349;
  constexpr static double mW = 80.36951;

  if (mi.size() != 4 || qi12.size() < 3 || qi22.size() < 3)
  {
    printf("[ERROR]: Wrong arguments when calculating U!\n");
    return 0.;
  }

  double u = GmV(mW, mi, qi12) - GmV(mZ, mi, qi12);

  for (int i = 0; i < 3; ++i)
  {
    u += qi22[i] * PVB(22, mW, mi[3], mi[i]) / (M_PI * mW * mW);
  }
  u -= 2. * PVB(22, mW, mi[3], mi[3]) / (M_PI * mW * mW);

  u -= (1. / (M_PI * mZ * mZ)) *
       (qi12[0] * PVB(22, mZ, mi[1], mi[2]) +
        qi12[1] * PVB(22, mZ, mi[0], mi[2]) +
        qi12[2] * PVB(22, mZ, mi[0], mi[1]) - PVB(22, mZ, mi[3], mi[3]));

  // printf("U = %16.8e\n", u); // DEBUG
  return u;
}

double Fm1m2(const double &m1, const double &m2)
{
  if (std::abs(m1 - m2) < 1e-9)
    return 0;

  double m12 = m1 * m1;
  double m22 = m2 * m2;

  double result = 0;

  result += 0.5 * (m12 + m22);
  result -= m12 * m22 * log(m12 / m22) / (m12 - m22);

  return result;
}

double GmV(const double &mV, const std::vector<double> &mi,
           const std::vector<double> &qi12)
{
  constexpr static double mPhi = 125.;

  double result = 0;

  for (int i = 0; i < 3; ++i)
  {
    result +=
        qi12[i] * (PVB(22, mV, mV, mi[i]) - mV * mV * PVB(0, mV, mV, mi[i]));
  }
  result += -PVB(22, mV, mV, mPhi) + mV * mV * PVB(0, mV, mV, mPhi);

  return result / (M_PI * mV * mV);
}

double PVB(const int i, const double &q, const double &m1, const double &m2)
{

  // The function will turn imaginary for m1 + m2 < q.
  // This corresponds to particles going on-shell and the S,T and U parameters
  // are not good observables in that case.
  if (m1 + m2 < q)
    return 0.;

  gsl_function F;
  integration_params params = {q, m1, m2};

  switch (i)
  {
  case 0:
  {
    F.function = &b0_integrand;
    break;
  }
  case 22:
  {
    F.function = &b22_integrand;
    break;
  }
  default:
  {
    printf("[ERROR]: This B_i function is not implemented!\n");
    return 0.;
  }
  }

  F.params = &params;

  constexpr double EPSABS = 1e-6;
  constexpr double EPSREL = 1e-6;

  double result = 0;
  double abserr = 0;
  size_t neval = 0;

  gsl_set_error_handler_off();

  static size_t limit = 100; // Nr of subintervals in integration
  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(limit);

  int gslStatus = gsl_integration_qag(&F, 0, 1, EPSABS, EPSREL, limit, 6,
                                      workspace, &result, &abserr);
  gsl_integration_workspace_free(workspace);

  if (gslStatus)
  {
    printf("[ERROR]: GSL failed in integrating PVB function, gsl_errno=%d\n",
           gslStatus);
    printf("q = %16.8e\n", q);
    printf("m1 = %16.8e\n", m1);
    printf("m2 = %16.8e\n", m2);
    // DEBUG:
    printf("\nB%i result: %16.8e\n", i, result);
    printf("B%i abserr: %16.8e\n", i, abserr);
    printf("B%i neval: %lu\n", i, neval);
    return 0.;
  }

  return result;
}

double b0_integrand(double x, void *p)
{
  struct integration_params *params = (struct integration_params *)p;
  double q2 = (params->q) * (params->q);
  double m12 = (params->m1) * (params->m1);
  double m22 = (params->m2) * (params->m2);

  return log((m12 * x + m22 * (1. - x)) /
             (m12 * x + m22 * (1. - x) - q2 * x * (1. - x)));
};

double b22_integrand(double x, void *p)
{
  struct integration_params *params = (struct integration_params *)p;
  double q2 = (params->q) * (params->q);
  double m12 = (params->m1) * (params->m1);
  double m22 = (params->m2) * (params->m2);

  double term1 = -0.5 * (m12 * x + m22 * (1. - x) - q2 * x * (1. - x)) *
                 log(m12 * x + m22 * (1. - x) - q2 * x * (1. - x));

  double term2 =
      0.5 * (m12 * x + m22 * (1. - x)) * log(m12 * x + m22 * (1. - x));
  return term1 + term2;
};

} // namespace THDME