#pragma once

#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>

namespace THDME
{
// If success is given, it sets it to false if any of the integrations fail.
double integrate_1D(const gsl_function *f, double a, double b,
                           bool *success = nullptr);

// double integrate_ND(const gsl_monte_function *f, bool *success = nullptr);

} // namespace THDME