#!/usr/bin/env python3
from model.model import Model
from model.device import Device
from model.transaction import TransactionGenerator


def main():
    refusals_counters = []
    iterations_qty = 100
    transactions_qty = 500

    while iterations_qty := iterations_qty - 1:
        model = Model(TransactionGenerator(4, 1, 0, transactions_qty))

        cashier = Device("Касса", 3, 2, 5)

        cook1 = Device("Повар 1", 10, 2, None)
        cook2 = Device("Повар 2", 11, 2, None)
        cook3 = Device("Повар 3", 20, 4, None)

        delivery = Device("Выдача", 1, 0.25, None)

        refusal = Device("Отказ", 0, 0, -1)
        success = Device("Успех", 0, 0, -1)

        cashier.add_filled_transfer(1, refusal)
        cashier.add_post_transfer(0.1, refusal)
        cashier.add_post_transfer(0.4, delivery)
        cashier.add_post_transfer(0.05, refusal)
        cashier.add_post_transfer(0.45 * 0.6 / 2, cook1)
        cashier.add_post_transfer(0.45 * 0.6 / 2, cook2)
        cashier.add_post_transfer(0.45 * 0.4, cook3)

        cook1.add_post_transfer(1, delivery)
        cook2.add_post_transfer(1, delivery)
        cook3.add_post_transfer(1, delivery)

        delivery.add_post_transfer(0.05, refusal)
        delivery.add_post_transfer(0.95, success)

        model.pipeline += [
            cashier,
            cook1,
            cook2,
            cook3,
            delivery,
        ]

        model.run()

        refusals_counters.append(refusal.income_counters.copy())

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
