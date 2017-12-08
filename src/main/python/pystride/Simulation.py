import os
import argparse
import xml.etree.ElementTree as ET

from copy import copy
from time import gmtime, strftime

import pystride
from pystride.stride.stride import StrideRunner

class Simulation:
    def __init__(self):
        #elf.forks = list()
        self.runner = StrideRunner()
        # TODO observer
        self._runConfig = None              # ElementTree with run config
        self.label = strftime("%Y%m%d_%H%M%S", gmtime())
        self._diseaseConfig = None          # ElementTree with disease config
        # TODO copy config params if necessary

    def loadRunConfig(self, filename: str):
        runConfigFile = os.path.join(self.getWorkingDirectory(), filename)
        if os.path.isfile(runConfigFile):
            self._runConfig = ET.parse(runConfigFile).getroot()
            self._updateLabel()
            self._updateDiseaseConfig()
        else:
            print("Run configuration file " + filename + " could not be found")

    def _updateDiseaseConfig(self):
        pass

    def _updateLabel(self):
        newLabel = self.getRunConfigParam('output_prefix')
        if newLabel != None:
            self.label = newLabel

    def getRunConfigParam(self, name: str):
        pass

    def getWorkingDirectory(self):
        return pystride.workspace

    def getOutputDirectory(self):
        return os.path.join(self.getWorkingDirectory(), self.label)

    def _linkData(self):
        pass

    def _setup(self, linkData=True):
        """
            Create folder in workspace to run simulation.
            Copy config and link to data
        """
        if linkData:
            self._linkData()

        os.makedirs(self.getOutputDirectory(), exist_ok=True)

        # Store run configuration
        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        ET.ElementTree(self._runConfig).write(configPath)

                        # Store disease configuration
    '''            origDiseasePath = self.getRunConfigParam("disease_config_file")
                        origDiseasePath = origDiseasePath[:-4] # remove .xml
                        diseasePath = origDiseasePath + "_" + self.label + ".xml"
                        ET.ElementTree(self._diseaseConfig).write(diseasePath)
                        self.setRunConfigParam("disease_config_file", diseasePath)

                        # only store last part of label (previous dirs already made)
                        self._runConfig.find('output_prefix').text = os.path.basename(self.label) + '/'
                        ET.ElementTree(self._runConfig).write(configPath)'''

    def run(self):
        self._setup()

        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        self.runner.Run(False, configPath)
'''
    def _updateDiseaseConfig(self):
        diseaseFilename = self._runConfig.find("disease_config_file").text
        diseaseFile = os.path.join(self.getWorkingDirectory(), diseaseFilename)
        if os.path.isfile(diseaseFile):
            self._diseaseConfig = ET.parse(diseaseFile).getroot()
        else:
            print("Disease configuration file " + diseaseFilename + " could not be found")

    def getRunConfigParam(self, name: str):
        if self._runConfig != None:
            if self._runConfig.find(name) != None:
                if self._runConfig.find(name).text != None:
                    return self._runConfig.find(name).text
        print("No run configuration parameter with name " + name)

    def getDiseaseConfigParam(self, name: str):
        if self._diseaseConfig:
            if self._diseaseConfig.find(name) != None:
                if self._diseaseConfig.find(name).text != None:
                    return self._diseaseConfig.find(name).text
        print("No disease configuration parameter with name " + name)

    def setRunConfigParam(self, name: str, value):
        if self._runConfig != None:
            elem = self._runConfig.find(name)
            if self._runConfig.find(name) != None:
                self._runConfig.find(name).text = str(value)
            else:
                ET.SubElement(self._runConfig, name).text = str(value)
        else:
            self._runConfig = ET.Element('run')
            ET.SubElement(self._runConfig, name).text = str(value)
        if name == "disease_config_file":
                self._updateDiseaseConfig()

    def setDiseaseConfigParam(self, name: str, value):
        if self._diseaseConfig:
            if self._diseaseConfig.find(name) != None:
                self._diseaseConfig.find(name).text = str(value)
            else:
                newElems = name.split('/')
                root = self._diseaseConfig
                for elem in newElems:
                    if root.find(elem) != None:
                        root = elem
                    else:
                        newElem = ET.SubElement(root, elem)
                        root = newElem
                self._diseaseConfig.find(name).text = str(value)
        else:
            self._diseaseConfig = ET.Element('disease')
            newElems = name.split('/')
            root = self._diseaseConfig
            for elem in newElems:
                newElem = ET.SubElement(root, elem)
                root = newElem
            self._diseaseConfig.find(name).text = str(value)

    def stop(self):
        """ Stop the simulation if it's running """

        if self.simulator:
            self.simulator.Stop()
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
        '''
'''    if isinstance(event, list):
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

                    self.observer.RegisterCallback(callback)
        pass

    def fork(self, name:str):
        """
            Create a new simulation instance from this one.
                :param str name: the name of the fork.
        """
        f = Fork(name, self)
        return f

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
            if (os.path.isfile(src)) and (not os.path.isfile(dst)):
                os.symlink(src, dst)



    def _build(self, trackIndexCase=False):
'''
'''        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")

            self.simulator.Setup(configPath, trackIndexCase)
            self.simulator.Build()
            self.simulator.RegisterObserver(self.observer)
        '''
'''        pass


    def run(self, *args, **kwargs):
        """ Run current simulation. """
        # Check if setup is done and if necessary continue previous simulations.
        self._setup
        '''
'''
        self._build(*args, **kwargs)

        if self.simulator:
            try:
                self.simulator.Run()
            except:
                print("Exception when running simulator. Closing down.")
                exit(1)'''
        #self.runner.Run(False, const std::string& config_file_name)

'''    def runForks(self, *args, **kwargs):
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
        pass'''


from .Fork import Fork
