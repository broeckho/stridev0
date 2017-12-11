from pystride.stride.stride import SimulatorObserver, Simulator, Population

class SimulationObserver(SimulatorObserver):
    def __init__(self):
        super().__init__()
        self.callbacks = list()
        self.simulator = None

    def SetSimulator(self, simulator):
        self.simulator = simulator

    def RegisterCallback(self, callback):
        self.callbacks.append(callback)

    def Update(self, timestep):
        for c in self.callbacks:
            try:
                c(self.simulator, timestep)
            except Exception as e:
                print(e)
