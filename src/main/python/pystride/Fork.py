import os
from copy import copy

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

    def _setup(self):
        """" Only write changed parameters """
        pass

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass
