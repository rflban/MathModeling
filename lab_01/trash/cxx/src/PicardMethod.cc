#include "PicardMethod.h"

#include "Monomial.h"
#include "Polynomial.h"

double PicardMethod::calc(const Monomial &m, double x, unsigned int n)
{
    double res;
    Polynomial *p = new Polynomial;

    while (n--)
    {
        p->power(2);
        (*p) += new Monomial(m);
        p->integrate();
    }

    res = (*p)(x);
    delete p;

    return res;
}

