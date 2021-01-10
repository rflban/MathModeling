#!/usr/bin/env python3
from model.model import Model
from model.device import Device
from model.transaction import TransactionGenerator


def main():
    refusals_counters = []
    iterations_qty = 100
    transactions_qty = 500

    while iterations_qty := iterations_qty - 1:
        model = Model(TransactionGenerator(3, 2, 0, transactions_qty))

        t1 = Device("Терминал 1", 4, 3)
        t2 = Device("Терминал 2", 4, 3)

        w1 = Device("Окно 1", 10, 5, 5)
        w2 = Device("Окно 2", 15, 5, 5)
        w3 = Device("Окно 3", 15, 10, 5)
        w4 = Device("Окно 4", 20, 10, 5)
        w5 = Device("Окно 5", 20, 5, 5)

        c = Device("Кабинет", 10, 5, None)

        r = Device("Отказ", 0, 0, -1)
        s = Device("Успех", 0, 0, -1)

        t1.add_filled_transfer(1, t2)
        t1.add_post_transfer(0.9 / 5, w1)
        t1.add_post_transfer(0.9 / 5, w2)
        t1.add_post_transfer(0.9 / 5, w3)
        t1.add_post_transfer(0.9 / 5, w4)
        t1.add_post_transfer(0.9 / 5, w5)
        t1.add_post_transfer(0.1, r)

        t2.add_filled_transfer(1, r)
        t2.add_post_transfer(0.9 / 5, w1)
        t2.add_post_transfer(0.9 / 5, w2)
        t2.add_post_transfer(0.9 / 5, w3)
        t2.add_post_transfer(0.9 / 5, w4)
        t2.add_post_transfer(0.9 / 5, w5)
        t2.add_post_transfer(0.1, r)

        w1.add_filled_transfer(1, r)
        w2.add_filled_transfer(1, r)
        w3.add_filled_transfer(1, r)
        w4.add_filled_transfer(1, r)
        w5.add_filled_transfer(1, r)

        w1.add_post_transfer(1, c)
        w2.add_post_transfer(1, c)

        w3.add_post_transfer(1, s)
        w4.add_post_transfer(1, s)
        w5.add_post_transfer(1, s)

        c.add_post_transfer(0.95, s)
        c.add_post_transfer(0.05, r)

        model.pipeline += [
            t1, t2,
            w1, w2, w3, w4, w5,
            c,
            s, r
        ]

        model.run()

        refusals_counters.append(r.income_counters.copy())

    from tabulate import tabulate

    refusals_sums = [sum(refusal.values()) for refusal in refusals_counters]
    refusal_min_sum = min(refusals_sums)
    refusal_max_sum = max(refusals_sums)

    print(
        "Число отказов находится в промежутке [%d, %d], "
        "а вероятность отказа - [%.4f, %.4f]\n" % (
            refusal_min_sum, refusal_max_sum,
            refusal_min_sum / transactions_qty,
            refusal_max_sum / transactions_qty
        )
    )

    min_col = {}
    max_col = {}

    for device_name in refusals_counters[0]:
        device_refusals = [
            (refusals[device_name] if device_name in refusals else 0)
            for refusals in refusals_counters
        ]
        min_col[device_name] = min(device_refusals)
        max_col[device_name] = max(device_refusals)

    print(tabulate(
        [[
            "Этап",
            "Минимальное\nчисло отказов",
            "Максимальное\nчисло отказов"
        ]] +
        [
            [device_name, min_col[device_name], max_col[device_name]]
            for device_name in min_col.keys()
        ],
        headers="firstrow"
    ))


if __name__ == "__main__":
    main()
