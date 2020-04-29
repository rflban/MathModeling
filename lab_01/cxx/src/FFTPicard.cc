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
    int curLen = 1;
    int sqrLen;

    delete[] polynomial;

    polLen = ::pow(2, approx);
    polynomial= new Real[polLen];
    polynomialComplex = new Complex[polLen];
    polynomialComplex[0] = Complex(1.0 / 3);

    fft->setMaxVectorSize(polLen);

    while (--approx)
    {
        sqrLen = curLen << 1;

        (*fft)(polynomialComplex, sqrLen, false);
        for (int i = 0; i < sqrLen; ++i)
            polynomialComplex[i] *= polynomialComplex[i];
        (*fft)(polynomialComplex, sqrLen, true);
        for (int i = 0; i < sqrLen; ++i)
            polynomialComplex[i + 1] = polynomialComplex[i] / Complex((i + 1) * 4 + 3);
        polynomialComplex[0] = 1.0 / 3;

        //if (approx == 8)
        //{
            //for (int i = 0; i < sqrLen; ++i)
                //std::cout << polynomialComplex[i] << ' ';
            //std::cout << '\n';
        //}

        curLen = sqrLen;
    }

    for (int i = 0; i < polLen; ++i)
        polynomial[i] = polynomialComplex[i].real();

    //for (int i = 0; i < polLen; ++i)
        //std::cout << polynomialComplex[i] << ' ';
    //std::cout << '\n';

    delete[] polynomialComplex;
}

}

