import os
from time import gmtime, strftime

import pystride
from pystride.stride.stride import StrideRunner
from .Config import Config

class Simulation:
    def __init__(self):
        self.forks = list()
        self.runner = StrideRunner()
        # TODO observer
        self.runConfig = None
        self.diseaseConfig = None
        self.timestamp =  strftime("%Y%m%d_%H%M%S", gmtime())

    def loadRunConfig(self, filename: str):
        self.runConfig = Config(filename)
        self.diseaseConfig = Config(self.runConfig.getParameter("disease_config_file"))

    def getLabel(self):
        label = self.runConfig.getParameter('output_prefix')
        if label == None:
            return self.timestamp
        return label

    def getWorkingDirectory(self):
        return pystride.workspace

    def getOutputDirectory(self):
        return os.path.join(self.getWorkingDirectory(), self.getLabel())

    def _linkData(self):
        dataDir = os.path.join(self.getOutputDirectory(), "data")
        os.makedirs(dataDir, exist_ok=True)
        files = [
            self.runConfig.getParameter("population_file"),
            self.runConfig.getParameter("holidays_file"),
            self.runConfig.getParameter("age_contact_matrix_file"),
        ]
        for src in files:
            dst = os.path.join(dataDir, os.path.basename(src))
            if (os.path.isfile(src)) and (not (os.path.isfile(dst))):
                os.symlink(src, dst)

    def _setup(self, linkData=True):
        """
            Create folder in workspace to run simulation.
            Copy config and link to data
        """
        if linkData:
            self._linkData()

        os.makedirs(self.getOutputDirectory(), exist_ok=True)
        # Store the run configuration
        configPath = os.path.join(self.getOutputDirectory(), self.getLabel() + ".xml")
        self.runConfig.toFile(configPath)
        # TODO store disease configuration

    def registerCallback(self, callback):
        pass

    def fork(self, name: str):
        """
            Create a new simulation instance from this one.
            :param str name: the name of the fork.
        """
        f = Fork(name, self)
        return f

    def run(self, trackIndexCase=False):
        """ Run the current simulation. """
        self._setup()

        configPath = os.path.join(self.getOutputDirectory(), self.getLabel() + ".xml")
        try:
            self.runner.Run(trackIndexCase, configPath)
        except:
            print("Exception while running the simulator. Closing down.")
            exit(1)

    def runForks(self, *args, **kwargs):
        """ Run all forks but not the root simulation. """
        self._setup()
        for fork in self.forks:
            fork.run(*args, **kwargs)

    def runAll(self, *args, **kwargs):
        """ Run the root simulation and all forks. """
        self.run(*args, **kwargs)
        self.runForks(*args, **kwargs)

    def stop(self):
        pass

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass

from .Fork import Fork
