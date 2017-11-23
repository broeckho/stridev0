import os
import argparse
import xml.etree.ElementTree as ET

import pystride

from pystride.SimulationUtils import SimulationUtils

class Simulation():
    def __init__(self):
        self.forks = list()
        self.simulator = None
        #TODO observer
        self._runConfig = None              # ElementTree with run config
        self._diseaseConfig = None          # ElementTree with disease config
        self.label = "default"

    def loadRunConfig(self, filename: str):
        runConfigFile = os.path.join(self.getWorkingDirectory(), filename)
        if os.path.isfile(runConfigFile):
            self._runConfig = ET.parse(runConfigFile).getroot()
            # Update disease configuration
            self._updateDiseaseConfig()
            # Update label
            if self._runConfig.find("output_prefix").text:
                self.label = self._runConfig.find("output_prefix").text
        else:
            print("Run configuration file " + filename + " could not be found")

    def _updateDiseaseConfig(self):
        diseaseFilename = self._runConfig.find("disease_config_file").text
        diseaseFile = os.path.join(self.getWorkingDirectory(), diseaseFilename)
        if os.path.isfile(diseaseFile):
            self._diseaseConfig = ET.parse(diseaseFile).getroot()
        else:
            print("Disease configuration file " + diseaseFilename + " could not be found")

    def getRunConfigParam(self, name: str):
        if self._runConfig:
            if self._runConfig.find(name).text:
                return self._runConfig.find(name).text
        print("No run configuration parameter with namme " + name)

    def getDiseaseConfigParam(self, name: str):
        # TODO
        pass

    def setRunConfigParam(self, name: str, value):
        if _runConfig:
            if self._runConfig.find(name):
                self._runConfig.find(name).text = str(value)
            else:
                ET.SubElement(self._runConfig, name).text = str(value)
        else:
            self._runConfig = ET.Element('run')
            ET.SubElement(self._runConfig, name).text = str(value)

    def setDiseaseConfigParam(self, name: str, value):
        # TODO
        pass

    def showRunConfig(self):
        """ Print out the run configuration parameters. """
        if self._runConfig:
            print("Run configuration: ")
            print(ET.tostring(self._runConfig))
        else:
            print("No run configuration found!")

    def showDiseaseConfig(self):
        """ Print out the disease configuration parameters specified in the disease file. """
        if self._diseaseConfig:
            print("Disease configuration: ")
            print(ET.tostring(self._diseaseConfig))
        else:
            print("No disease configuration found!")

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
        f = Fork(name, self)
        return f

    def getWorkingDirectory(self):
        return pystride.workspace

    def getOutputDirectory(self):
        return os.path.join(self.getWorkingDirectory(), self.label)

    def _linkData(self):
        dataDir = os.path.join(self.getOutputDirectory(), "data")
        os.makedirs(dataDir, exist_ok=True)
        files = [
            self.getRunConfigParam('population_file'),
            self.getRunConfigParam('disease_config_file'),
            self.getRunConfigParam('holidays_file'),
            self.getRunConfigParam('age_contact_matrix_file'),
        ]
        for src in files:
            dst = os.path.join(dataDir, os.path.basename(src))
            if os.path.isfile(src) and not os.path.isfile(dst):
                os.symlink(src, dst)

    def _setup(self, linkData=True):
        """
            Create folder in workspace to run simulation.
            Copy config and link to data.
        """
        if linkData:
            self._linkData()

    '''
            os.makedirs(self.getOutputDirectory(), exist_ok=True)
            diseasePath = os.path.join(self.getOutputDirectory(), "data", self.disease.label + ".xml")
            self.disease.toFile(diseasePath)

            configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
            # only store last part of label (previous dirs already made)
            oldLabel = self.label
            self.label = os.path.basename(self.label)
            self.toFile(configPath)
            self.label = oldLabel
    '''

    def _build(self, *args, **kwargs):
        pass

    '''
        def build(self, runParallel=True, trackIndexCase=False, output=True):
            self.simulator = getSimulator(self, self.getWorkingDirectory(), runParallel, trackIndexCase, output)
            if self.simulator:
                self.simulator.registerObserver(self.observer)
    '''

    def run(self, *args, **kwargs):
        """ Run current simulation. """
        # Check if setup is done and if necessary continue previous simulations.
        self._setup()

    '''

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

    def runForks(self, *args, **kwargs):
        """ Run all forks but not the root simulation. """
        self._setup()
        for fork in self.forks:
            fork.run(*args, **kwargs)

    def runAll(self, *args, **kwargs):
        """ Run root simulation and forks. """
        self.run(*args, **kwargs)
        self.runForks(*args, **kwargs)

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass

from .Fork import Fork

#TODO PUQ integration
