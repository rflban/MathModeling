#include "MTPicard.h"

#include <thread>
#include <vector>

static inline int pow(int num, unsigned int p)
{
    int powered = 1;

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
        mmlabs::Picard::Real *squared, int begin, int end,
        mmlabs::Picard::Real *polynomial
        )
{
    for (int i, idx = begin; idx < end; idx++)
    {
        i = idx - 1;

        do
            squared[idx] += polynomial[i] *
                            polynomial[idx - 1 - i] /
                            (idx * 4 + 3);
        while (i--);
    }
}

namespace mmlabs {

void MTPicard::computePol(int approx)
{
    Real *squared;
    int curLen = 1;
    int sqrLen;

    delete[] polynomial;

    polLen = ::pow(2, approx);
    squared = new Real[polLen];
    polynomial = new Real[polLen];
    polynomial[0] = 1.0 / 3;

    std::vector<std::thread> threads;

    while (--approx)
    {
        sqrLen = curLen << 1;
        for (int i = 0; i < sqrLen; i++)
            squared[i] = 0;

        int step = (polLen / 8) ? (polLen / 8) : polLen;
        for (int i = 1; i < polLen; i += step)
            threads.push_back(std::thread(
                        &::squarePolPart,
                        squared, i, std::min(i + step, polLen),
                        polynomial));

        for (std::thread &thread: threads)
            thread.join();

        squared[0] = 1.0 / 3;

        std::swap(polynomial, squared);
        curLen = sqrLen;
        threads.clear();
    }

    delete[] squared;
}

}

