import os
import xml.etree.ElementTree as ET

import pystride

from .stride import Simulator

class Simulation():
    def __init__(self):
        # Load default config for simulations
        self.config = ET.parse('run_default.xml').getroot()
        self.disease_config = None
        # TODO self.population
        self.simulator = None


    def loadConfig(self, filename):
        self.config = ET.parse(filename).getroot()

    def showConfig(self):
        print (self.config)

    def run(self):
        pass

    def _build(self):
        pass

    def getWorkingDirectory(self):
        return stride.workspace

    def getOutputDirectory(self):
        return os.path.join(self.getWorkingDirectory(), self.label)

# TODO run
# TODO stop
# TODO forks
# TODO PUQ integration

'''
import argparse

from .stride import getSimulator
from .SimulationObserver import *
from .Disease import Disease
from .stride import Society

    def stop(self):
        """ Stop the simulation if it's running """
        if self.simulator:
            self.simulator.Stop()

    def registerCallback(self, callback, event):
        """ Registers a callback to the simulation.

            :param callback: a function appropriate for the event type.
            :param event: either an event specified in SimulationObserver, an integer, list of events, or list of integers. A single integer is converted to a TimestepIntervalEvent. A list of integers to a list of TimestepEvents.
        """
        if isinstance(event, list):
            # list of events
            for e in event:
                # if int -> specific timestep
                if isinstance(e, int):
                    self.observer.registerCallback(callback, TimestepEvent(e))
                else:
                    self.registerCallback(e)

        elif isinstance(event, int):
            # convert int to TimestepIntervalEvent
            self.observer.registerCallback(callback, TimestepIntervalEvent(event))

        elif isinstance(event, Event):
            # event
            self.observer.registerCallback(callback, event)
        else:
            raise RuntimeError("Unknown event type: " + str(event))

    def _linkData(self):
        dataDir = os.path.join(self.getOutputDirectory(), "data")
        os.makedirs(dataDir, exist_ok=True)
        files = [
            self.p_population_file.get(),
            self.p_disease_config_file.get(),
            self.holidays_file,
            self.age_contact_matrix_file,
        ]
        for src in files:
            dst = os.path.join(dataDir, os.path.basename(src))
            if os.path.isfile(src) and not os.path.isfile(dst):
                os.symlink(src, dst)

    def setup(self, linkData=True):
        """ Create folder in workspace to run simulation. Copy config and link to data. """
        if linkData:
            self._linkData()

        # create .sim file to indicate simulation folder (for GUI)
        open(os.path.join(self.getOutputDirectory(), ".sim"), 'a').close()

        os.makedirs(self.getOutputDirectory(), exist_ok=True)
        diseasePath = os.path.join(self.getOutputDirectory(), "data", self.disease.label + ".xml")
        self.disease.toFile(diseasePath)

        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        # only store last part of label (previous dirs already made)
        oldLabel = self.label
        self.label = os.path.basename(self.label)
        self.toFile(configPath)
        self.label = oldLabel

    def build(self, runParallel=True, trackIndexCase=False, output=True):
        self.simulator = getSimulator(self, self.getWorkingDirectory(), runParallel, trackIndexCase, output)
        if self.simulator:
            self.simulator.registerObserver(self.observer)

    def run(self, *args, numDays=0, **kwargs):
        """ Run current simulation.

        Check if setup is done and if necessary continue previous simulations. """
        self.setup()

        if len(self.getUQProperties()) > 0:
            print("Found PUQ parameters, running PUQ")
            self.runPUQ(*args, numDays=numDays, **kwargs)
            return

        self.build(*args, **kwargs)

        if self.simulator:
            try:
                self.simulator.Run(numDays)
            except:
                print("Exception when running simulator. Closing down.")
                exit(0)
'''