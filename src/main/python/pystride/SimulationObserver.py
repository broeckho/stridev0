from pystride.stride.stride import SimulatorObserver

class SimulationObserver(SimulatorObserver):
    def __init__(self, simulator):
        super().__init__()
        self.simulator = simulator
        # TODO list of callbacks to perform

    def Update(self, timestep):
        print("Timestep " + str(timestep))

'''
class SimulationObserver(SimulatorObserver):
    def __init__(self, simulation):
        super().__init__()
        self.simulation = simulation
        self.callbacks = dict()

    def getCallbacks(self, eventType):
        return self.callbacks.get(eventType, list())

    def registerCallback(self, callback, event):
        eventCallbacks = self.getCallbacks(type(event))
        eventCallbacks.append((callback, event))
        self.callbacks[type(event)] = eventCallbacks

    def callCallback(self, callback, *args, **kwargs):
        try:
            callback(*args, **kwargs)
        except Exception as e:
            print("Exception occurred in callback:")
            print(e)

    def TimeStepStarted(self, simulator, timestep):
        for callback, event in self.getCallbacks(TimestepEvent):
            if event.timestep == timestep and event.beforeTS:
                self.callCallback(callback, self.simulation, timestep)

        for callback, event in self.getCallbacks(TimestepIntervalEvent):
            if timestep % event.interval == 0 and event.beforeTS:
                self.callCallback(callback, self.simulation, timestep)

    def TimeStepFinished(self, simulator, timestep):
        for callback, event in self.getCallbacks(TimestepEvent):
            if event.timestep == timestep and not event.beforeTS:
                self.callCallback(callback, self.simulation, timestep)

        for callback, event in self.getCallbacks(TimestepIntervalEvent):
            if timestep % event.interval == 0 and not event.beforeTS:
                self.callCallback(callback, self.simulation, timestep)

    def SimulationFinished(self, simulator, timestep):
        pass


class Event:
    pass

class TimestepEvent(Event):
    def __init__(self, timestep: int, beforeTS=False):
        self.timestep = timestep
        self.beforeTS = beforeTS
        if self.timestep < 1:
            raise RuntimeError("Timestep can't be smaller than 1. Given timestep: {}".format(str(self.timestep)))

class TimestepIntervalEvent(Event):
    def __init__(self, interval: int, beforeTS=False):
        self.interval = interval
        self.beforeTS = beforeTS

        if self.interval < 1:
            raise RuntimeError("Interval can't be smaller than 1. Given interval: {}".format(str(self.interval)))
'''
