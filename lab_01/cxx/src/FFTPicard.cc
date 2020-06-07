#include "FFTPicard.h"

#include "FFT.h"

#include <iostream>

static inline int intLog2(int num)
{
    int res = 1;

    while (res < num)
        res <<= 1;

    return res;
}

namespace mmlabs {

FFTPicard::FFTPicard() :
    fft(new FFT<Complex>)
{
}

FFTPicard::~FFTPicard()
{
    delete fft;
}

void FFTPicard::computePol(int approx)
{
    int sqrLen = 1;
    Complex *polynomialComplex;
    Complex *integratedComplex;

    allocatePols(approx);
    polynomialComplex = new Complex[polLens[approx - 1]];
    integratedComplex = new Complex[polLens[approx - 1]];
    polynomialComplex[0] = 1.0 / 3;

    fft->setMaxVectorSize(polLens[approx - 1]);

    for (int idx = 0; idx < approx; ++idx)
    {
        for (long long i = 0; i < polLens[idx]; ++i)
            polynomials[idx][i] = polynomialComplex[i].real();

        sqrLen <<= 1;

        (*fft)(polynomialComplex, sqrLen, false);
        for (long long i = 0; i < sqrLen; ++i)
            polynomialComplex[i] *= polynomialComplex[i];
        (*fft)(polynomialComplex, sqrLen, true);

        for (long long i = 0; i < sqrLen - 1; ++i)
            integratedComplex[i + 1] = polynomialComplex[i] /
                                       Complex((int)(i + 1) * 4 + 3);

        integratedComplex[0] = 1.0 / 3;
        std::swap(integratedComplex, polynomialComplex);
    }

    delete[] polynomialComplex;
    delete[] integratedComplex;
}

}

