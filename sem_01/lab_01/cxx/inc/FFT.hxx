#ifndef MMLAB01_FFT_HXX_
#define MMLAB01_FFT_HXX_

#include "FFT.h"

#include <cmath>
#include <utility>

namespace mmlabs {

template<typename TComplex>
FFT<TComplex>::FFT(int maxVectorSize) :
    wlenPowers(nullptr)
{
    setMaxVectorSize(maxVectorSize);
}

template<typename TComplex>
FFT<TComplex>::~FFT()
{
    delete[] wlenPowers;
}

template<typename TComplex>
void FFT<TComplex>::setMaxVectorSize(int sz)
{
    delete[] wlenPowers;

    wlenPowers = new TComplex[sz];
    m_maxVectorSize = sz;
}

template<typename TComplex>
void FFT<TComplex>::operator()(TComplex *vec, int sz, bool invert)
{
    for (int i = 1, j = 0; i < sz; ++i)
    {
        int bit = sz >> 1;

        while (j >= bit)
        {
            j -= bit;
            bit >>= 1;
        }

        j += bit;

        if (i > j)
            std::swap(vec[i], vec[j]);
    }

    for (int len = 2; len <= sz; len <<= 1)
    {
        int lenDiv2 = len >> 1;
        double alpha = 2 * M_PI / len * (invert ? -1 : +1);

        wlenPowers[0] = TComplex(1, 0);
        TComplex wlen = TComplex(cos(alpha), sin(alpha));

        for (int i = 1; i < lenDiv2; ++i)
            wlenPowers[i] = wlenPowers[i - 1] * wlen;

        for (int i = 0; i < sz; i += len)
            for (TComplex t, *pu = vec + i,
                             *pv = vec + i + lenDiv2,
                             *pe = vec + i + lenDiv2,
                             *pw = wlenPowers;
                pu != pe; ++pu, ++pv, ++pw)
            {
                t = *pv * *pw;
                *pv = *pu - t;
                *pu += t;
            }
    }

    if (invert)
        for (int i = 0; i < sz; ++i)
            vec[i] /= sz;
}

}

#endif // MMLAB01_FFT_HXX_

