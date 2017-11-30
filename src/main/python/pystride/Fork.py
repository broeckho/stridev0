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

        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        # only store last part of label (previous dirs already made)
        self._runConfig.find('output_prefix').text = self.parent.label + '/' + self.label
        ET.ElementTree(self._runConfig).write(configPath)

        # Store disease configuration
        origDiseasePath = self.getRunConfigParam("disease_config_file")
        origDiseasePath = origDiseasePath[:-4] # remove .xml
        diseasePath = origDiseasePath + "_" + self.label + ".xml"
        ET.ElementTree(self._diseaseConfig).write(diseasePath)
        self.setRunConfigParam("disease_config_file", diseasePath)
        '''
                if linkData:
                    self._linkData()
        '''

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass
