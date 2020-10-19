#ifndef MMLAB01_FFTPICARD_H_
#define MMLAB01_FFTPICARD_H_

#include "Picard.h"

#include <complex>

namespace mmlabs {

template<typename TComplex>
class FFT;

class FFTPicard : public Picard
{
public:
    FFTPicard();
    ~FFTPicard();

    void computePol(int approx) override;

    using Complex = std::complex<Real>;

private:
    FFT<Complex> *fft;
};

}

#endif // MMLAB01_FFTPICARD_H_

