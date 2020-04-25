#!/usr/bin/env python3

import bigfloat as bf
import texttable as tt


class Monomial:
    def __init__(self, power=1, ratio=1):
        self.power = power
        self.ratio = bf.BigFloat(ratio)

    def __str__(self):
        return '%.5f*x**%d' % (self.ratio, self.power)

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
        return Monomial(self.power * power, bf.pow(self.ratio, power))

    def integrate(self):
        return Monomial(self.power + 1, self.ratio / (self.power + 1))

    def calc(self, x):
        return self.ratio * x ** self.power


class Polynomial:
    def __init__(self, monomials=[]):
        self.monomials = []

        for item in monomials:
            if type(item) == Monomial:
                for i in range(len(self.monomials)):
                    if self.monomials[i].power == item.power:
                        self.monomials[i] += item
                        break
                else:
                    self.monomials.append(item)


    def __str__(self):
        return ' + '.join('%s' % (monomial) for monomial in self.monomials)

    def __add__(self, other):
        copy = Polynomial(self.monomials.copy())

        if type(other) == Polynomial:
            for monomial in other.monomials:
                copy += monomial
        elif type(other) == Monomial:
            for i in range(len(copy.monomials)):
                if copy.monomials[i].power == other.power:
                    copy.monomials[i] += other
                    break
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
            for monomial1 in self.monomials:
                for monomial2 in other.monomials:
                    polynomial += monomial1 * monomial2
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

    def calc(self, x):
        res = 0

        for monomial in self.monomials:
            res += monomial.calc(x)

        return res


def my_pol(x, u):
    return x.pow(2) + u.pow(2)


def picard(f, n):
    x = Monomial(1, 1)
    u = Polynomial()

    while n:
        u = f(x, u).integrate()
        n -= 1

    return u


def main():
    with bf.precision(1000):
        pol = picard(my_pol, 10)
        print(pol)
        print(pol.calc(1))


if __name__ == '__main__':
    main()

