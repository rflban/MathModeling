from .base import Entity, Event, EventReceiver
from random import random


class Device(Entity, EventReceiver):
    def __init__(self,
            name,
            mean_hold_time,
            time_deviation,
            queue_limit=0
            ):
        super().__init__(
            name=name, mean_t=mean_hold_time,
            dev_t=time_deviation
        )

        self.queue_size = 0
        if queue_limit is not None:
            self.queue_limit = queue_limit + 1
        else:
            self.queue_limit = None

        self.filled_transfer = []
        self.pre_transfer = []
        self.post_transfer = []

        self.is_seized = False

        self.income_counters = {}
        self.outcome_counters = {}

        self.missed_counter = 0

    def accept_transaction(self, current_time):
        if self.queue_limit is not None:
            if self.queue_size >= self.queue_limit:
                return False

        self.queue_size += 1

        if not self.is_active():
            self.seize(current_time)

            if self.response_time == current_time:
                self.respond(current_time)

        return True

    def is_active(self):
        return self.is_seized

    def seize(self, current_time):
        if self.queue_size > 0:
            # pre seize redirection
            if self.transfer(self.pre_transfer, current_time):
                self.queue_size -= 1
                return self.seize(current_time)
            self.is_seized = True
            self.time_step(current_time)

    def release(self):
        self.is_seized = False
        self.queue_size -= 1

    def respond(self, current_time):
        if (not super().respond(current_time)) or (not self.is_active()):
            return False

        self.release()
        # post release redirection
        # if self.transfer(self.post_transfer, current_time):
            # return True
        self.transfer(self.post_transfer, current_time)

        # if not self.is_active():
        self.seize(current_time)

        return True

    def receive(self, event: Event):
        if str(event.sender) not in self.income_counters:
            self.income_counters[str(event.sender)] = 0
        self.income_counters[str(event.sender)] += 1

        if self.accept_transaction(event.emission_time):
            return
        # income redirection
        self.transfer(self.filled_transfer, event.emission_time)

    def transfer(self, transfer_list, current_time):
        source_device = None
        received_probability = random()

        high_bound = 0
        for probability, device in transfer_list:
            high_bound += probability
            if received_probability < high_bound:
                source_device = device
                break

        # if source_device is not None:
        if source_device is not None:
            source_device.receive(Event(
                self,
                current_time
            ))

            if str(source_device) not in self.outcome_counters:
                self.outcome_counters[str(source_device)] = 0
            self.outcome_counters[str(source_device)] += 1

            return True
        return False

    def update_transfer_dict(transfer_list, probability, device):
        if sum([prob for prob, _ in transfer_list]) + probability > 1:
            return False

        transfer_list.append((probability, device))
        return True

    def add_filled_transfer(self, probability, device):
        return type(self).update_transfer_dict(
            self.filled_transfer, probability, device
        )

    def add_pre_transfer(self, probability, device):
        return type(self).update_transfer_dict(
            self.pre_transfer, probability, device
        )

    def add_post_transfer(self, probability, device):
        return type(self).update_transfer_dict(
            self.post_transfer, probability, device
        )

    def income_counter(self):
        return sum([cnt for cnt in self.income_counters.values()])

    def outcome_counter(self):
        return sum([cnt for cnt in self.outcome_counters.values()])
