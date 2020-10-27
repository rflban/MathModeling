import random
import datetime


class StandardRandom():
    def get(self, num_len: int):
        return random.randint(10**(num_len - 1), 10**num_len)


class TableRandom():
    def __init__(self, path_to_table: str = "./table.txt"):
        self.digits = ""

        with open(path_to_table) as table_file:
            for line in table_file:
                self.digits += line[:-1]

        self.idx_init()

    def idx_step(self, step: int = 1):
        self.idx += step

        if self.idx + step >= len(self.digits):
            self.idx_init(step)

    def idx_init(self, offset: int = 0):
        now = datetime.datetime.now()
        self.idx = int((now.microsecond % 60)/60 * (len(self.digits) - offset))

    def get(self, num_len: int):
        self.idx_step(num_len)
        rnd_num = int(self.digits[self.idx:self.idx+num_len])

        while len(str(rnd_num)) < num_len:
            self.idx_step()
            rnd_num = rnd_num * 10 + int(self.digits[self.idx])

        return rnd_num
