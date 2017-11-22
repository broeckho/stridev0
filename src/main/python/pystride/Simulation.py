import os
import argparse

import pystride

class Simulation():
    def __init__():
        pass

    def loadConfig(filename: str):
        """ Load a configuration from a file """
        pass

    def stop(self):
        """ Stop the simulation if it's running """
        pass

    def registerCallback(self, callback, event):
        """
            Registers a callback to the simulation

            :param callback: a function appropriate for the event type.
            :param event: either an event specified in SimulatorObserver,
                    an integer (converted to TimestepIntervalEvent), list of
                    events, or list of integers (converted to list of
                    TimestepIntervalEvents).
        """
        pass

    def fork(self, name:str):
        """
            Create a new simulation instance from this one.

            :param str name: the name of the fork.
        """
        pass

    def getWorkingDirectory(self):
        pass

    def getOutputDirectory(self):
        pass

    def _linkData(self):
        pass

    def _setup(self):
        """
            Create folder in workspace to run simulation.
            Copy config and link to data.
        """
        pass

    def _build(self):
        pass

    def run(self):
        """ Run current simulation. """
        pass

    def runForks(self):
        """ Run all forks but not the root simulation. """
        pass

    def runAll(self):
        """ Run root simulation and forks. """
        pass

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass


from .Fork import Fork

#TODO PUQ integration
