#include "Picard.h"

#include <utility>
#include <string.h>

static inline long long pow(long long num, unsigned int p)
{
    long long powered = 1;

    while (p)
    {
        if (p & 1)
            powered *= num;
        num *= num;

        p >>= 1;
    }

    return powered;
}

static inline void freePols(mmlabs::Picard::Real **&pols,
                            long long *&polLens,
                            int approx)
{
    if (pols)
    {
        for (int i = 0; i < approx; ++i)
            delete[] pols[i];

        delete[] pols;
        pols = nullptr;

    }

    if (polLens)
    {
        delete[] polLens;
        polLens = nullptr;
    }

    approx = 0;
}

namespace mmlabs {

Picard::Picard() :
    approx(0),
    polLens(nullptr),
    polynomials(nullptr)
{
}

Picard::~Picard()
{
    ::freePols(polynomials, polLens, approx);
}

void Picard::allocatePols(int maxApprox)
{
    ::freePols(polynomials, polLens, approx);
    this->approx = maxApprox;

    polLens = new long long[approx];
    polynomials = new Real *[approx];

    for (int i = 0; i < approx; ++i)
    {
        polLens[i] = ::pow(2ll, i + 1);
        polynomials[i] = new Real[polLens[i]];
    }
}

void Picard::computePol(int approx)
{
    Real *squared;
    Real *polynomial;
    long long curLen = 1;
    long long sqrLen;

    allocatePols(approx);

    squared = new Real[polLens[approx - 1]];
    polynomial = new Real[polLens[approx - 1]];
    polynomial[0] = 1.0 / 3;

    for (int idx = 0; idx < approx; ++idx)
    {
        for (long long i = 0; i < curLen; ++i)
            polynomials[idx][i] = polynomial[i];

        sqrLen = curLen << 1;
        for (long long i = 0; i < sqrLen; i++)
            squared[i] = 0;

        for (long long i = 0; i < curLen; i++)
            for (long long j = 0; j < curLen; j++)
                squared[i + j + 1] += polynomial[i] *
                                      polynomial[j] /
                                      ((int)(i + j + 1) * 4 + 3);
        squared[0] = 1.0 / 3;

        std::swap(polynomial, squared);
        curLen = sqrLen;
    }

    delete[] squared;
    delete[] polynomial;
}

double Picard::operator()(double x)
{
    return (*this)(x, approx);
}

double Picard::operator()(double x, int approx)
{
    if (!polynomials)
        return 0;
    if (approx > this->approx)
        return 0;

    Real res = 0;

    for (int i = 0; i < polLens[approx - 1]; i++)
        res += polynomials[approx - 1][i] * pow(Real(x), i * 4 + 3);

    return res;
}

}

