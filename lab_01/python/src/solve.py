#!/usr/bin/env python3

import texttable as tt

from decimal import Decimal
from threading import Thread
from math import ceil


def picard(x, n):
    polynomial = [Decimal(1 / 3)]

    while (n - 1):
        squared = [Decimal(0)] * (len(polynomial) * 2)

        for i in range(len(polynomial)):
            for j in range(len(polynomial)):
                squared[i + j + 1] += polynomial[i] * polynomial[j] / ((i+j+1)*4 + 3)

        squared[0] = Decimal(1 / 3)
        polynomial = squared

        n -= 1

    print(polynomial)
    res = Decimal(0)
    x = Decimal(x)

    for i in range(len(polynomial)):
        res += polynomial[i] * x ** (i * 4 + 3)

    print((len(polynomial) - 1) * 4 + 3)

    return res


def mtpicard(x, n):
    polynomial = [Decimal(1 / 3)]

    while (n - 1):
        squared = [Decimal(0)] * (len(polynomial) * 2)

        fullrange = range(len(polynomial))

        step = ceil(len(fullrange) / 8)
        subranges = [fullrange[i:i+step] for i in range(0, len(fullrange), step)]
        threads = []
        # print(subranges)

        for subrange in subranges:
            thread = PicardThread()
            thread.squared = squared
            thread.polynomial = polynomial
            thread.subrange = subrange
            threads.append(thread)

        print(len(threads))

        for thread in threads:
            thread.start()
        for thread in threads:
            thread.join()

        squared[0] = Decimal(1 / 3)
        polynomial = squared

        n -= 1

    # print(polynomial)
    res = Decimal(0)
    x = Decimal(x)

    for i in range(len(polynomial)):
        res += polynomial[i] * x ** (i * 4 + 3)

    print((len(polynomial) - 1) * 4 + 3)

    return res


class PicardThread(Thread):
    def __init__(self):
        Thread.__init__(self)

    def run(self):
        for i in self.subrange:
            for j in range(len(self.polynomial)):
                self.squared[i+j+1] += self.polynomial[i] * self.polynomial[j] / ((i+j+1)*4 + 3)


def main():
    n = 15

    # print(picard(2, n))
    print(mtpicard(2, n))


if __name__ == '__main__':
    main()

