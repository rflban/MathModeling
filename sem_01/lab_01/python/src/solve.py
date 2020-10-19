#!/usr/bin/env python3

import texttable as tt

from decimal import Decimal


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


def main():
    print(picard(2, 17))


if __name__ == '__main__':
    main()

