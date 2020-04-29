#include <iostream>

#include <complex>

#include "Picard.h"
#include "MTPicard.h"
#include "FFTPicard.h"

#include "FFT.h"

//void sqrpol(double *pol, int sz, double *res, int &ressz)
//{
    //mmlabs::FFT<std::complex<double>> fft(1000);

    //ressz = sz << 1;
    //ressz = 1;
    //while (ressz < sz)
        //ressz <<= 1;
    //ressz <<= 1;
    //std::complex<double> *vec = new std::complex<double>[ressz];

    //for (int idx = 0; idx < ressz; ++idx)
        //if (idx < sz)
            //vec[idx] = std::complex<double>(pol[idx]);
        //else
            //vec[idx] = std::complex<double>(0);

    //fft(vec, ressz);
    //for (int idx = 0; idx < ressz; ++idx)
        //vec[idx] *= vec[idx];
    //fft(vec, ressz, true);

    ////while (vec[ressz - 1] == std::complex<double>(0))
        ////ressz--;
    //ressz = sz * 2 - 1;
    //for (int idx = 0; idx < ressz; idx++)
        //res[idx] = vec[idx].real();

    //delete[] vec;
//}

//int main()
//{
    ////double pol[] = { 1, 1 };
    //double pol[] = { 4, 5, 2, 3, 3, 2,4,32,52, 231,234,324,2,42,34,23,423,42};
    //double res[100];
    //int sz = sizeof(pol) / sizeof(pol[0]);
    //int ressz;

    //sqrpol(pol, sz, res, ressz);

    //for (int idx = 0; idx < ressz; ++idx)
        //std::cout << res[idx] << ' ';
    //std::cout << '\n';

    //return 0;
//}

int main()
{
    mmlabs::APicard *picard = new mmlabs::FFTPicard;
    //mmlabs::APicard *picard = new mmlabs::MTPicard;
    //mmlabs::APicard *picard = new mmlabs::Picard;

    picard->computePol(10);
    std::cout << (*picard)(2) << '\n';

    delete picard;

    return 0;
}

