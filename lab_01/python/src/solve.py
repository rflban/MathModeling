#!/usr/bin/env python3

import bigfloat as bf
import texttable as tt

from threading import Thread
from math import ceil


def gcd(a, b):
    while (b):
        a %= b
        a, b = b, a
    return a


def lcm(a, b):
    return a // gcd(a, b) * b


class Fraction:
    def __init__(self, numerator=0, denominator=1, isneg=False):
        self.numerator = numerator
        self.denominator = denominator
        self.isneg = isneg

    def __str__(self):
        return '%s%d/%d' % (
            '-' if self.isneg else '',
            self.numerator,
            self.denominator
        )

    def __float__(self):
        return (self.numerator / self.denominator) * \
               (-1.0 if self.isneg else 1.0)

    def __add__(self, other):
        denominator = lcm(self.denominator, other.denominator)

        ratio1 = denominator // self.denominator
        if self.isneg:
            ratio1 *= -1

        ratio2 = denominator // other.denominator
        if other.isneg:
            ratio2 *= -1

        numerator = self.numerator * ratio1 + other.numerator * ratio2

        return Fraction(
            abs(numerator),
            denominator,
            numerator < 0
        )

    def __mul__(self, other):
        numerator = self.numerator * other.numerator
        denominator = self.denominator * other.denominator

        cd = gcd(numerator, denominator)
        numerator //= cd
        denominator //= cd

        return Fraction(numerator, denominator, self.isneg ^ other.isneg)

    def __truediv__(self, other):
        return self * \
               Fraction(other.denominator, other.numerator, other.isneg)

    def __pow__(self, exp):
        if type(exp) == int:
            return Fraction(
                    self.numerator ** exp,
                    self.denominator ** exp,
                    self.isneg and (exp & 1 == 1)
            )


class Monomial:
    def __init__(self, power=1, ratio=Fraction(1, 1)):
        self.power = power
        self.ratio = ratio

    def __str__(self):
        return '%s*x**%d' % (self.ratio, self.power)

    def __add__(self, other):
        if type(other) == Monomial:
            if (self.power == other.power):
                return Monomial(self.power, self.ratio + other.ratio)
            else:
                return Polynomial([self, other])
        elif type(other) == Polynomial:
            return other + self

    def __mul__(self, other):
        if type(other) == Monomial:
            power = other.power
            ratio = other.ratio
        elif type(other) == Polynomial:
            return other * self
        else:
            power = 0
            ratio = other

        return Monomial(self.power + power, self.ratio * ratio)

    def pow(self, power):
        return Monomial(self.power * power, self.ratio ** power)

    def integrate(self):
        return Monomial(self.power + 1, self.ratio / (self.power + 1))

    def calc(self, x):
        return self.ratio * x ** self.power


class Polynomial:
    def __init__(self, monomials=[]):
        self.monomials = []

        for item in monomials:
            if type(item) == Monomial:
                if Polynomial.do_group:
                    for i in range(len(self.monomials)):
                        if self.monomials[i].power == item.power:
                            self.monomials[i] += item
                            break
                    else:
                        self.monomials.append(item)
                else:
                    self.monomials.append(item)

    do_group = True

    def __str__(self):
        return ' + '.join('%s' % (monomial) for monomial in self.monomials)

    def __add__(self, other):
        copy = Polynomial(self.monomials.copy())

        if type(other) == Polynomial:
            for monomial in other.monomials:
                copy += monomial
        elif type(other) == Monomial:
            if Polynomial.do_group:
                for i in range(len(copy.monomials)):
                    if copy.monomials[i].power == other.power:
                        copy.monomials[i] += other
                        break
                else:
                    copy.monomials.append(other)
            else:
                copy.monomials.append(other)

        return copy

    def __mul__(self, other):
        polynomial = Polynomial()

        if type(other) == Monomial:
            polynomial.monomials = self.monomials.copy()

            for i in range(len(polynomial.monomials)):
                polynomial.monomials[i] *= other
        elif type(other) == Polynomial:
            # for monomial1 in self.monomials:
                # for monomial2 in other.monomials:
                    # polynomial += monomial1 * monomial2
            if len(self.monomials) > 0:
                threads = []

                step = ceil(len(self.monomials) / 8)
                submonomials = [self.monomials[i : i + step] for i in range(0, len(self.monomials), step)]

                for monomials in submonomials:
                    thread = Polynomial.MulThread()
                    thread.set(monomials, other.monomials)
                    threads.append(thread)

                for thread in threads:
                    thread.start()

                for thread in threads:
                    thread.join()
                    polynomial.monomials += thread.products

        else:
            polynomial.monomials = self.monomials.copy()

            for i in range(len(polynomial.monomials)):
                polynomial.monomials[i] *= other

        return polynomial

    def pow(self, power):
        powered = Polynomial([Monomial(0, 1)])

        while power:
            powered *= self
            power -= 1

        return powered

    def integrate(self):
        integrated = Polynomial()

        for monomial in self.monomials:
            integrated.monomials.append(monomial.integrate())

        return integrated

    def mintegrate(self):
        integrated = Polynomial()
        threads = []

        step = ceil(len(self.monomials) / 8)
        submonomials = [self.monomials[i : i + step] for i in range(0, len(self.monomials), step)]

        # for ms in submonomials:
            # print(*ms, sep=' + ')

        for monomials in submonomials:
            thread = Polynomial.IntegrateThread()
            thread.set(monomials)
            threads.append(thread)

        for thread in threads:
            thread.start()

        for thread in threads:
            thread.join()
            integrated.monomials += thread.monomials

        return integrated

    def calc(self, x):
        res = 0

        for monomial in self.monomials:
            res += monomial.calc(x)

        return res

    class IntegrateThread(Thread):
        def __init__(self):
            Thread.__init__(self)

        def set(self, monomials):
            self.monomials = monomials

        def run(self):
            for i in range(len(self.monomials)):
                self.monomials[i] = self.monomials[i].integrate()

    class MulThread(Thread):
        def __init__(self):
            Thread.__init__(self)

        def set(self, factors1, factors2):
            self.factors1 = factors1
            self.factors2 = factors2

        def run(self):
            self.products = []

            for factor1 in self.factors1:
                for factors2 in self.factors2:
                    self.products.append(factor1 * factors2)


def my_pol(x, u):
    return x.pow(2) + u.pow(2)


def picard(f, n):
    x = Monomial(1, 1)
    u = Polynomial()

    while n:
        u = f(x, u).mintegrate()
        # u = f(x, u).integrate()
        n -= 1

    return u


def main():
    print(Fraction(7, 6) ** 2)

    # with bf.precision(1000):
        # pol = picard(my_pol, 12)
        # print(pol)
        # print(pol.calc(1))


if __name__ == '__main__':
    main()

