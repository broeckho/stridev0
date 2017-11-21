import os
from copy import copy

import pystride

from .Simulation import Simulation

class Fork(Simulation):
    def __init__(self, name: str, parent):
        super.__init__(parent)      # Copy original simulation
        self.label = name
        if isinstance(parent, Fork):
            self.parent = parent.parent # flattened fork
        else:
            self.parent = parent
        #self.parent.forks.append(self)
        #self.observer.callbacks = copy(parent.observer.callbacks)

        def getWorkingDirectory(self):
            return os.path.join(pystride.workspace, self.parent.label)

        '''
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

        '''

        def __getstate__(self):
            return dict()

        def __setstate__(self, state):
            pass
