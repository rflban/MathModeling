#ifndef MMLAB01_PICARD_H_
#define MMLAB01_PICARD_H_

#include "APicard.h"

#define MPFR_REAL_ENABLE_CONV_OPS
#include "mpfrwrapper/real.hpp"
#undef MPFR_REAL_ENABLE_CONV_OPS

namespace mmlabs {

class Picard : public APicard
{
public:
    Picard();
    ~Picard();

    virtual void computePol(int approx) override;
    double operator()(double x) override;

    static const int precision = 200;

    using Real = mpfr::real<precision>;

protected:
    inline Real pow(const Real &r, int p) { return mpfr::pow(r, p); }

    int polLen;
    Real *polynomial;
};

}

#endif // MMLAB01_PICARD_H_

