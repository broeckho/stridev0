
import os

from .Simulation import Simulation

class Fork(Simulation):
    pass

'''
from copy import copy

import stride

class Fork(Simulation):
    def __init__(self, name: str, parent):
        super().__init__(parent)    # Copy original simulation
        self.label = name
        if isinstance(parent, Fork):
            self.parent = parent.parent # flattened fork
        else:
            self.parent = parent
        self.parent.forks.append(self)
        self.observer.callbacks = copy(parent.observer.callbacks)

    def getWorkingDirectory(self):
        return os.path.join(stride.workspace, self.parent.label)

    def setup(self, linkData=True):
        """ Only write changed parameters """
        os.makedirs(self.getOutputDirectory(), exist_ok=True)

        # create .sim file to indicate simulation folder (for GUI)
        open(os.path.join(self.getOutputDirectory(), ".sim"), 'a').close()

        if self.disease != self.parent.disease:
            os.makedirs(os.path.join(self.getOutputDirectory(), "data"), exist_ok=True)
            diseasePath = os.path.join(self.getOutputDirectory(), "data", self.disease.label + ".xml")
            self.disease.diffToFile(self.parent.disease, diseasePath)

        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        self.diffToFile(self.parent, configPath)

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass

'''
'''
import os
from copy import copy
import xml.etree.ElementTree as ET

import pystride

from .Simulation import Simulation

class Fork(Simulation):
    def __init__(self, name: str, parent):
        super().__init__(parent)            # Copy original simulation
        self.label = name
        if isinstance(parent, Fork):
            self.parent = parent.parent     # Flattened fork
        else:
            self.parent = parent
        self.parent.forks.append(self)
        # self.observer.callbacks = copy(parent.observer.callbacks)

    def getWorkingDirectory(self):
        return os.path.join(pystride.workspace, self.parent.label)

    def _setup(self, linkData=True):
        """" Only write changed parameters """
        os.makedirs(self.getOutputDirectory(), exist_ok=True)

        # Store disease configuration
        origDiseasePath = self.getRunConfigParam("disease_config_file")
        origDiseasePath = origDiseasePath[:-4] # remove .xml
        diseasePath = origDiseasePath + "_" + self.label + ".xml"
        ET.ElementTree(self._diseaseConfig).write(diseasePath)
        self.setRunConfigParam("disease_config_file", diseasePath)

        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        # only store last part of label (previous dirs already made)
        self._runConfig.find('output_prefix').text = self.parent.label + '/' + self.label
        ET.ElementTree(self._runConfig).write(configPath)


                #if linkData:
            #        self._linkData()


    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass
'''
