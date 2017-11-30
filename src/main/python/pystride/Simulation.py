import os
import argparse
import xml.etree.ElementTree as ET

from time import gmtime, strftime

import pystride

from pystride.stride.stride import SimUtils

class Simulation():
    def __init__(self, original=None):
        self.forks = list()
        self.simulator = None
        #TODO observer
        self._runConfig = None              # ElementTree with run config
        self._diseaseConfig = None          # ElementTree with disease config
        self.label = strftime("%Y%m%d%H%M%S", gmtime())
        if original:
            self._runConfig = original._runConfig
            self._diseaseConfig = original._diseaseConfig

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
        print("No run configuration parameter with name " + name)

    def getDiseaseConfigParam(self, name: str):
        if self._diseaseConfig:
            if self._diseaseConfig.find(name).text:
                return self._diseaseConfig.find(name).text
        print("No disease configuration parameter with name " + name)

    def setRunConfigParam(self, name: str, value):
        if self._runConfig:
            if self._runConfig.find(name):
                self._runConfig.find(name).text = str(value)
            else:
                ET.SubElement(self._runConfig, name).text = str(value)
        else:
            self._runConfig = ET.Element('run')
            ET.SubElement(self._runConfig, name).text = str(value)

    def setDiseaseConfigParam(self, name: str, value):
        if self._diseaseConfig:
            if self._diseaseConfig.find(name):
                self._diseaseConfig.find(name).text = str(value)
            else:
                newElems = name.split('/')
                root = self._diseaseConfig
                for elem in newElems:
                    if root.find(elem):
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
            if (os.path.isfile(src)) and (not os.path.isfile(dst)):
                os.symlink(src, dst)

    def _setup(self, linkData=True):
        """
            Create folder in workspace to run simulation.
            Copy config and link to data.
        """
        if linkData:
            self._linkData()

        os.makedirs(self.getOutputDirectory(), exist_ok=True)

        # Store run configuration
        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        # only store last part of label (previous dirs already made)
        self._runConfig.find('output_prefix').text = os.path.basename(self.label) + '/'
        ET.ElementTree(self._runConfig).write(configPath)

        # Store disease configuration
        origDiseasePath = self.getRunConfigParam("disease_config_file")
        origDiseasePath = origDiseasePath[:-4] # remove .xml
        diseasePath = origDiseasePath + "_" + self.label + ".xml"
        ET.ElementTree(self._diseaseConfig).write(diseasePath)
        self.setRunConfigParam("disease_config_file", diseasePath)

    def _build(self, trackIndexCase=False):
        configPath = os.path.join(self.getOutputDirectory(), self.label + ".xml")
        self.simulator = SimUtils()
        self.simulator.Setup(configPath, trackIndexCase)
        #TODO register observer

    def run(self, *args, **kwargs):
        """ Run current simulation. """
        # Check if setup is done and if necessary continue previous simulations.
        self._setup()
        self._build(*args, **kwargs)

        if self.simulator:
            try:
                self.simulator.Run()
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
