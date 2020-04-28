#include "Picard.h"

#include <utility>

static inline int pow(int num, unsigned int p)
{
    if (p == 0)
        return 1;

    int powered = 1;

    for (int i = p >> 1; i != 0; i--)
        powered *= num;
    powered *= powered;

    if (p & 1)
        powered *= num;

    return powered;
}

namespace mmlabs {

Picard::Picard() :
    polLen(0),
    polynomial(nullptr)
{
}

Picard::~Picard()
{
    if (polynomial)
        delete[] polynomial;
}

void Picard::computePol(int approx)
{
    Real *squared;
    int curLen = 1;
    int sqrLen;

    polLen = ::pow(2, approx);
    squared = new Real[polLen];
    polynomial = new Real[polLen];
    polynomial[0] = 1.0 / 3;

    while (--approx)
    {
        sqrLen = curLen << 1;
        for (int i = 0; i < sqrLen; i++)
            squared[i] = 0;

        for (int i = 0; i < curLen; i++)
            for (int j = 0; j < curLen; j++)
                squared[i + j + 1] += polynomial[i] * polynomial[j] /
                                      ((i + j + 1) * 4 + 3);
        squared[0] = 1.0 / 3;

        std::swap(polynomial, squared);
        curLen = sqrLen;
    }

    delete[] squared;
}

double Picard::operator()(double x)
{
    if (!polynomial)
        return 0;

    Real res = 0;

    for (int i = 0; i < polLen; i++)
        res += polynomial[i] * pow(Real(x), i * 4 + 3);

    return res;
}

}

