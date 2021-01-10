from .base import EventReceiver, Event
from .device import Device

from functools import reduce


class Model(EventReceiver):
    def __init__(self,
            transaction_generator
            ):
        super().__init__()

        self.pipeline = [transaction_generator]
        self.pipeline[0].attach(self)

    def run(self):
        current_time = 0
        current_time = -1

        while any([entity.is_active() for entity in self.pipeline]):
            # ensuing_time = min(
                # [
                    # entity.response_time
                    # for entity in self.pipeline
                    # if entity.response_time >= current_time
                # ]
            # )
            ensuing_time = min(
                [
                    entity.response_time
                    for entity in self.pipeline
                    if entity.response_time > current_time
                ]
            )

            # ensuing_time = reduce(
                # lambda x, y: y if (current_time <= y < x) else x,
                # map(lambda x: x.response_time, self.pipeline)
            # )

            current_time = ensuing_time

            for entity in self.pipeline:
                entity.respond(current_time)

    def receive(self, event: Event):
        # self.pipeline[1].accept_transaction(event.emission_time)
        self.pipeline[1].receive(event)
