#ifndef MMLAB01_FFT_H_
#define MMLAB01_FFT_H_

namespace mmlabs {

template<typename TComplex>
class FFT
{
public:
    FFT(int maxVectorSize=100);
    ~FFT();

    void setMaxVectorSize(int sz);

    void operator()(TComplex *vec, int sz, bool invert=false);

private:
    int m_maxVectorSize;
    TComplex *wlenPowers;
};

}

#include "FFT.hxx"

#endif // MMLAB01_FFT_H_

