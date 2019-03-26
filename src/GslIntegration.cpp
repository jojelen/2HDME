#include "GslIntegration.h"

#include <cmath>

// #include <gsl/gsl_math.h>
// #include <gsl/gsl_monte.h>
// #include <gsl/gsl_monte_plain.h>
// #include <gsl/gsl_monte_miser.h>
// #include <gsl/gsl_monte_vegas.h>

using namespace std;

namespace THDME
{

double integrate_1D(const gsl_function *func, double a, double b,
                    bool *success)
{
  constexpr static double EPSABS = 1e-12;
  constexpr static double EPSREL = 1e-12;

  double result = 0;
  double abserr = 0;
  size_t neval = 0;

  gsl_set_error_handler_off();

  static size_t limit = 200; // Nr of subintervals in integration
  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(limit);

  int gslStatus = gsl_integration_qag(func, a, b, EPSABS, EPSREL, limit, 6,
                                      workspace, &result, &abserr);
  gsl_integration_workspace_free(workspace);

  if (gslStatus)
  {
    printf("[ERROR]: GSL failed in integrating function, gsl_errno=%d\n",
           gslStatus);
    printf("         %s\n", gsl_strerror(gslStatus));
    // DEBUG:
    printf("result: %16.8e\n", result);
    printf("abserr: %16.8e\n", abserr);
    printf("neval: %lu\n", neval);
    printf("Integrand value at %f = %16.8e\n", (b - a) / 2.,
           func->function((b - a) / 2., func->params));

    if (success != nullptr)
      *success = false;
    if (!std::isnan(result))
      return result;
    else
      return 0.;
  }

  return result;
}

// double integrate_2D(const gsl_monte_function *f, bool *success = nullptr)
// {
  // auto display_results = [](char *title, double result, double error) 
  // {
  //   printf("%s ==================\n", title);
  //   printf("result = % .6f\n", result);
  //   printf("sigma  = % .6f\n", error);
  //   printf("exact  = % .6f\n", exact);
  //   printf("error  = % .6f = %.2g sigma\n", result - exact,
  //          fabs(result - exact) / error);
  // };

  // double res, err;

  // double xl[3] = {0, 0};
  // double xu[3] = {M_PI, M_PI, M_PI};

  // const gsl_rng_type *T;
  // gsl_rng *r;

  // size_t calls = 500000;

  // gsl_rng_env_setup();

  // T = gsl_rng_default;
  // r = gsl_rng_alloc(T);

  // {
  //   gsl_monte_plain_state *s = gsl_monte_plain_alloc(3);
  //   gsl_monte_plain_integrate(f, xl, xu, 3, calls, r, s,
  //                             &res, &err);
  //   gsl_monte_plain_free(s);

  //   display_results("plain", res, err);
  // }

  // {
  //   gsl_monte_miser_state *s = gsl_monte_miser_alloc(3);
  //   gsl_monte_miser_integrate(f, xl, xu, 3, calls, r, s,
  //                             &res, &err);
  //   gsl_monte_miser_free(s);

  //   display_results("miser", res, err);
  // }

  // {
  //   gsl_monte_vegas_state *s = gsl_monte_vegas_alloc(3);

  //   gsl_monte_vegas_integrate(f, xl, xu, 3, 10000, r, s,
  //                             &res, &err);
  //   display_results("vegas warm-up", res, err);

  //   printf("converging...\n");

  //   do
  //   {
  //     gsl_monte_vegas_integrate(f, xl, xu, 3, calls / 5, r, s,
  //                               &res, &err);
  //     printf("result = % .6f sigma = % .6f "
  //            "chisq/dof = %.1f\n",
  //            res, err, gsl_monte_vegas_chisq(s));
  //   } while (fabs(gsl_monte_vegas_chisq(s) - 1.0) > 0.5);

  //   display_results("vegas final", res, err);

  //   gsl_monte_vegas_free(s);
  // }

  // gsl_rng_free(r);
// }
} // namespace THDME