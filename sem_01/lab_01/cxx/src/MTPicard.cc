#include "MTPicard.h"

#include <thread>
#include <vector>

#define THREADS_QTY 8

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

static void squarePolPart(
        mmlabs::Picard::Real *squared,
        long long begin, long long end, long long step,
        mmlabs::Picard::Real *polynomial
        )
{
    for (long long i, idx = begin; idx < end; idx += step)
    {
        i = idx - 1;

        do
            squared[idx] += polynomial[i] *
                            polynomial[idx - 1 - i] /
                            ((int)idx * 4 + 3);
        while (i--);
    }
}

namespace mmlabs {

void MTPicard::computePol(int approx)
{
    Real *squared;
    Real *polynomial;
    long long curLen = 1;
    long long sqrLen;

    allocatePols(approx);
    squared = new Real[polLens[approx - 1]];
    polynomial = new Real[polLens[approx - 1]];
    polynomial[0] = 1.0 / 3;

    std::vector<std::thread> threads;

    for (int idx = 0; idx < approx; ++idx)
    {
        for (long long i = 0; i < curLen; ++i)
            polynomials[idx][i] = polynomial[i];

        sqrLen = curLen << 1;
        for (long long i = 0; i < sqrLen; i++)
            squared[i] = 0;

        for (int i = 1; i <= THREADS_QTY; i++)
            threads.push_back(std::thread(
                        &::squarePolPart,
                        squared, i, sqrLen, THREADS_QTY,
                        polynomial));

        for (std::thread &thread: threads)
            thread.join();

        squared[0] = 1.0 / 3;

        std::swap(polynomial, squared);
        curLen = sqrLen;
        threads.clear();
    }

    delete[] squared;
    delete[] polynomial;
}

}

