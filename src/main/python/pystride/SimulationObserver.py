from pystride.stride.stride import SimulatorObserver


class SimulationObserver(SimulatorObserver):
    def __init__(self):
        super().__init__()
        self.callbacks = list()

    def RegisterCallback(self, callback):
        self.callbacks.append(callback)

    def Update(self, simulator, timestep):
        print "CALLBACK"


'''
    def RegisterCallback(self, callback):
        self.callbacks.append(callback)

    def Update(self, timestep):
        for c in self.callbacks:
            print("CALLBACK")
            try:
                c(self.simulator, timestep)
            except Exception as e:
                print(e)
'''
