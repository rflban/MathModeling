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
    Complex *polynomialComplex;
    Complex *integratedComplex;
    int sqrLen = 1;

    delete[] polynomial;

    polLen = ::pow(2, approx - 1);
    polynomial = new Real[polLen + 1];
    polynomialComplex = new Complex[polLen];
    integratedComplex = new Complex[polLen];
    polynomialComplex[0] = 1.0 / 3;

    fft->setMaxVectorSize(polLen);

    while (--approx)
    {
        sqrLen <<= 1;

        (*fft)(polynomialComplex, sqrLen, false);
        for (int i = 0; i < sqrLen; ++i)
            polynomialComplex[i] *= polynomialComplex[i];
        (*fft)(polynomialComplex, sqrLen, true);
        //for (int i = 0; i < sqrLen; ++i)
            //polynomialComplex[i + 1] = polynomialComplex[i] / Complex((i + 1) * 4 + 3);
        //polynomialComplex[0] = 1.0 / 3;
        for (int i = 0; i < sqrLen - 1; ++i)
            integratedComplex[i + 1] = polynomialComplex[i] / Complex((i + 1) * 4 + 3);
        integratedComplex[0] = 1.0 / 3;
        std::swap(integratedComplex, polynomialComplex);

        //if (approx == 8)
        //{
            //for (int i = 0; i < sqrLen; ++i)
                //std::cout << polynomialComplex[i] << ' ';
            //std::cout << '\n';
        //}
    }

    for (int i = 0; i < polLen; ++i)
        polynomial[i] = polynomialComplex[i].real();

    //for (int i = 0; i < polLen; ++i)
        //std::cout << polynomialComplex[i] << ' ';
    for (int i = 0; i < polLen; ++i)
        std::cout << polynomial[i] << ' ';
    std::cout << '\n';

    delete[] polynomialComplex;
    delete[] integratedComplex;
}

}

