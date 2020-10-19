#ifndef MMLAB01_PICARD_H_
#define MMLAB01_PICARD_H_

#include "APicard.h"

#define MPFR_REAL_ENABLE_CONV_OPS
#include "mpfrwrapper/real.hpp"

namespace mmlabs {

class Picard : public APicard
{
public:
    Picard();
    ~Picard();

    void computePol(int approx) override;
    double operator()(double x) override;
    double operator()(double x, int approx) override;

    static const int precision = 200;

    using Real = mpfr::real<precision>;

protected:
    void allocatePols(int maxApprox);

    static inline Real pow(const Real &r, int p) { return mpfr::pow(r, p); }

    int approx;
    long long *polLens;
    Real **polynomials;

private:
};

}

#endif // MMLAB01_PICARD_H_

