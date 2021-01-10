from random import uniform


class Event:
    def __init__(self,
            sender,
            emission_time
            ):
        self.sender = sender
        self.emission_time = emission_time


class EventReceiver:
    def __init__(self):
        pass
    def receive(self, event: Event):
        pass


class Entity:
    def __init__(self, name, mean_t, dev_t):
        self.name = name
        self.response_time = 0
        self.mean_t = mean_t
        self.dev_t = dev_t

        self.receivers = []

    def __str__(self):
        return self.name

    def time_step(self, current_time):
        step_value = \
            uniform(self.mean_t - self.dev_t, self.mean_t + self.dev_t)

        self.response_time = current_time + step_value

        return step_value

    def respond(self, current_time):
        if current_time != self.response_time:
            return False
        return True

    def notify(self, event):
        for receiver in self.receivers:
            receiver.receive(event)

    def attach(self, receiver):
        if receiver not in self.receivers:
            self.receivers.append(receiver)

    def detach(self, receiver):
        if receiver in self.receivers:
            self.receivers.remove(receiver)

    def is_active(self):
        pass
