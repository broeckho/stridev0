import os

import pystride

from .Simulation import Simulation

class Fork(Simulation):
    def __init__(self, name: str, parent):
        super().__init__()
        # Copy config from parent
        self.runConfig = parent.runConfig.copy()
        self.runConfig.setParameter('output_prefix', name)
        self.diseaseConfig = parent.diseaseConfig.copy()

        if isinstance(parent, Fork):
            self.parent = parent.parent # Flattened fork
        else:
            self.parent = parent
        self.parent.forks.append(self)

    def getWorkingDirectory(self):
        return os.path.join(pystride.workspace, self.parent.getLabel())

    def _setup(self, linkData=True):
        """ """
        if linkData:
            self._linkData()

        os.makedirs(self.getOutputDirectory(), exist_ok=True)
        # Store disease configuration
        oldDiseaseFile = self.runConfig.getParameter("disease_config_file")[:-4]
        diseaseFile = oldDiseaseFile + "_" + self.getLabel() + ".xml"
        diseasePath = os.path.join(self.getOutputDirectory(), diseaseFile)
        self.diseaseConfig.toFile(diseasePath)
        self.runConfig.setParameter("disease_config_file", diseasePath)

        configPath = os.path.join(self.getOutputDirectory(), self.getLabel() + ".xml")
        oldLabel = self.getLabel()
        self.runConfig.setParameter("output_prefix", self.getOutputDirectory())
        self.runConfig.toFile(configPath)
        self.runConfig.setParameter('output_prefix', oldLabel)

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass
