#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

from scipy.stats import norm
from math import sqrt


def main():
    a = float(input("Enter a: "))
    b = float(input("Enter b: "))

    delta = b - a
    x = np.arange(a - delta / 2, b + delta / 2, 0.001)
    y_cdf = [ud_cdf(a, b, _x) for _x in x]
    y_pdf = [ud_pdf(a, b, _x) for _x in x]

    draw_plots(x, y_cdf, y_pdf, 'Равномерное распределение')

    mean = float(input("Enter mean: "))
    stdd = float(input("Enter standard deviation: "))

    x = np.arange(-10, 10, 0.001)
    y_cdf = nd_cdf(x, mean, stdd)
    y_pdf = nd_pdf(x, mean, stdd)

    draw_plots(x, y_cdf, y_pdf, 'Нормальное распределение')


def draw_plots(x, y_cdf, y_pdf, name):
    fig, axs = plt.subplots(2, figsize=(6, 7))

    fig.suptitle(name)
    axs[0].plot(x, y_cdf)
    axs[1].plot(x, y_pdf)

    axs[0].set_xlabel('$x$')
    axs[0].set_ylabel('$F(x)$')

    axs[1].set_xlabel('$x$')
    axs[1].set_ylabel('$f(x)$')

    axs[0].grid(True)
    axs[1].grid(True)

    plt.show()


def ud_cdf(a, b, x):
    return (x - a) / (b - a) if (a <= x < b) else 0 if x < a else 1

def ud_pdf(a, b, x):
    return 1 / (b - a) if (a <= x <= b) else 0

def nd_cdf(x, mu, sigma):
    return norm.cdf(x, mu, sqrt(sigma))

def nd_pdf(x, mu, sigma):
    return norm.pdf(x, mu, sqrt(sigma))


if __name__ == '__main__':
    main()
