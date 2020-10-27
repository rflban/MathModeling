#!/usr/bin/env python3

import tabulate
import numpy

import myrnd


def main():
    n = 10000
    n = 10

    slists = [ [], [], [] ]
    tlists = [ [], [], [] ]
    sgen = myrnd.StandardRandom()
    tgen = myrnd.TableRandom()

    for i in range(n):
        for j in range(3):
            slists[j].append(sgen.get(j + 1))
            tlists[j].append(tgen.get(j + 1))

    print("Программный метод:")
    print_table(create_table(slists), n)
    print()

    print("Табличный метод:")
    print_table(create_table(tlists), n)


def create_table(sequences: list) -> str:
    cols = [list(range(1, len(sequences[0]) + 1))]

    for seq in sequences:
        cols.append(seq.copy())

    cols[0] += ['Ожидаемый', 'Полученный']

    for i in range(1, len(cols)):
        cols[i] += list(frequency_criterion(cols[i], i))

    table = tabulate.tabulate(
        {
            "No" : cols[0],
            "1 разр." : cols[1],
            "2 разр." : cols[2],
            "3 разр." : cols[3],
        }, headers="keys", tablefmt="presto"
    )

    return table


def print_table(table, n):
    rows = table.split('\n')
    print('\n'.join(rows[:5 + 2]))
    if n > 10:
        print(("{:^%ds}" % len(rows[0])).format('\u2022'*3))
    print('\n'.join(rows[-5 - 2: -2]))
    print(rows[1])
    print('\n'.join(rows[-2:]))


def frequency_criterion(sequence, num_len):
    mean = numpy.mean(sequence)
    stdd = numpy.sqrt(numpy.var(sequence))

    cnt = 0
    for item in sequence:
        if (mean - stdd) < item < (mean + stdd):
            cnt += 1

    sequence_max_delta = 10
    if num_len > 1:
        sequence_max_delta = 9 * 10**(num_len-1)

    return (2 * stdd / sequence_max_delta), (cnt /  len(sequence))


if __name__ == "__main__":
    main()
