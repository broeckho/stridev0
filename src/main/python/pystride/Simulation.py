import os
import argparse
import xml.etree.ElementTree as ET

import pystride

class Simulation():
    def __init__(self):
        self.forks = list()
        self.simulator = None
        # TODO observer
        self.label = "Default"
        self._runConfig = None      # ETree with run config for simulation
        self._diseaseConfig = None  # ETree with disease config
        self._setDefaultConfig()    # Set default values for run & disease config

    def _setDefaultConfig(self):
        # Load run config
        self._runConfig = ET.parse('pystride/default_configs/run_default.xml').getroot()
        # Load disease config
        disease_file = self.getRunConfigParam('disease_config_file')
        self._diseaseConfig = ET.parse(os.path.join('pystride/default_configs', disease_file)).getroot()

    def loadRunConfig(self, filename: str):
        """ Load a configuration from a file """
        old_disease_file = self.getRunConfigParam('disease_config_file')
        self._config = ET.parse(filename).getroot()
        new_disease_file = self.getRunConfigParam('disease_config_file')
        if new_disease_file != old_disease_file:
            self._diseaseConfig = ET.parse(new_disease_file).getroot()

    def getRunConfigParam(self, name: str):
        if self._runConfig != None:
            configParam = self._runConfig.find(name)
            if configParam != None:
                return configParam.text
            else:
                print("No configuration parameter with name {} could be found", name)
        else:
            print("No run configuration found")

    def setRunConfigParam(self, name:str, value):
        '''configParam = self._config.find(name)
        if configParam != None:
            configParam.text = str(value)
        else:
            print("No configuration parameter with name {} could be found", name)'''
        pass

    def showRunConfig(self):
        print("Run configuration:")
        print(ET.tostring(self._runConfig))

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
        return os.path.join(self.getWorkingDirectory(),
                            self.getConfigParam('output_prefix'))

    def _linkData(self):
        pass

    def _setup(self):
        """
            Create folder in workspace to run simulation.
            Copy config and link to data.
        """
        pass

    def _build(self, *args, **kwargs):
        pass

    def run(self, *args, **kwargs):
        """ Run current simulation. """
        # Check if setup is done and if necessary continue previous simulaitons.
        self._setup()

        self._build(*args, *kwargs)
        if self.simulator:
            try:
                numDays = self.getRunConfigParam("num_days")
                self.simulator.Run(numDays)
            except:
                print("Exception when running simulator. Closing down.")
                exit(1)

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

'''
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
'''
