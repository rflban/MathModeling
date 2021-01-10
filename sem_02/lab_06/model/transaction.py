from .base import Entity, Event


class TransactionGenerator(Entity):
    __instances_counter = 0

    def __init__(self,
            mean_time_interval,
            time_deviation,
            start_time=0,
            transacts_limit=None
            ):
        self.__i_no = type(self).__instances_counter
        super().__init__(
            type(self).__name__ + " #{}".format(self.__i_no),
            mean_time_interval, time_deviation
        )

        self.start_t = start_time
        self.limit = transacts_limit

        self.generated_counter = 0
        self.response_time = self.start_t

        type(self).__instances_counter += 1

    def is_active(self):
        if self.limit is None:
            return True
        if self.generated_counter < self.limit:
            return True
        return False

    def generate(self):
        if self.is_active():
            self.notify(Event(
                self,
                self.response_time
            ))
            self.generated_counter += 1

            return True
        return False

    def respond(self, current_time):
        if not super().respond(current_time):
            return False

        if self.generate():
            self.time_step(current_time)

        return True
