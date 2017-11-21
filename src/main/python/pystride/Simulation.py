import os
import xml.etree.ElementTree as ET

import pystride

from stride.stride import Simulator

class Simulation():
    def __init__(self):
        # Load default config for simulations
        self.config = ET.parse('run_default.xml').getroot()
        self.disease_config = None
        # TODO self.population
        self.simulator = None


    def loadConfig(self, filename):
        self.config = ET.parse(filename).getroot()

    def showConfig(self):
        print (self.config)

    def stop(self):
        """ Stop the simulation if it's running """
        if self.simulator:
            self.Simulator.Stop()

    def fork(self, name: str):
        """ Create a new simulation instance from this one.

            :param str name: the name of the fork.
        """
        f = Fork(name, self)
        return f

    def registerCallback(self, callback, event):
        """ Registers a callback to the simulation.

            :param callback: a function appropriate for the event type.
            :param event: either an event specified in SimulationObserver,
                            an integer (timestep interval),
                            list of events, or list of integers
                            (list of timestep intervals).
        """
        pass

    def getWorkingDirectory(self):
        return stride.workspace

    def getOutputDirectory(self):
        return os.path.join(self.getWorkingDirectory(), self.label)

    def run(self):
        pass

    def runForks(self):
        """ Run all forks, but not the root simulation. """
        pass

    def runAll(self):
        """ Run root simulation and forks. """
        pass

    def __getstate__(self):
        return dict()

    def __setstate__(self, state):
        pass


# TODO PUQ integration


'''
import os
import argparse

import stride

from .stride import SimulationConfig, getSimulator
from .SimulationObserver import *
from .LogLevel import LogLevel
from .Disease import Disease
from .Generator import Generator
from .stride import Society
from .PUQIntegration import *
from puq.puq_cmd import load_internal


class Simulation(SimulationConfig):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.forks = list()
        self.simulator = None
        self.observer = SimulationObserver(self)
        self.strideUQ = StrideMonteCarlo(num=5)
        self.uqProperties = dict()
        self._disease = None        # wraps C++ disease config once set

    @staticmethod
    def fromFile(filename: str):
        config = Simulation()
        config.parseXML(filename)
        return config

    # Properties
    @uq_property
    def rng_seed(self):
        return self.p_rng_seed.get()
    @rng_seed.setter
    def rng_seed(self, value):
        self.p_rng_seed.set(value)

    @uq_property
    def r0(self):
        return self.p_r0.get()
    @r0.setter
    def r0(self, value):
        self.p_r0.set(value)

    @uq_property
    def seeding_rate(self):
        return self.p_seeding_rate.get()
    @seeding_rate.setter
    def seeding_rate(self, value):
        self.p_seeding_rate.set(value)

    @uq_property
    def immunity_rate(self):
        return self.p_immunity_rate.get()
    @immunity_rate.setter
    def immunity_rate(self, value):
        self.p_immunity_rate.set(value)

    @property
    def population(self):
        return self.p_population_file.GetPopulation()
    @population.setter
    def population(self, value):
        if isinstance(value, str):
            self.p_population_file.set(value)
            return
        if isinstance(value, Generator):
            value.thisown = 0
            shared_ptr = stride.stride.GeneratorConfigToSharedPtr(value)
            self.p_population_file.SetGeneratorConfig(shared_ptr)

            return
        if isinstance(value, Society):
            pop = value.GetStridePopulation()
            self.p_population_file.SetPopulation(pop)
            return

        raise ValueError("Value should be a string, instance of Generator or instance of Society.")

    @property
    def num_days(self):
        return self.p_num_days.get()
    @num_days.setter
    def num_days(self, value):
        self.p_num_days.set(value)

    @property
    def label(self):
        return self.p_label.get()
    @label.setter
    def label(self, value):
        self.p_label.set(value)

    @property
    def disease(self):
        if self._disease is None:
            self._disease = Disease(config=self.p_disease_config_file.getConfig())
        return self._disease
    @disease.setter
    def disease(self, value):
        if isinstance(value, str):
            self.p_disease_config_file.set(value)
        else:
            self.p_disease_config_file.setConfig(value)
        self._disease = Disease(config=self.p_disease_config_file.getConfig())

    @property
    def generate_person_file(self):
        return self.p_generate_person_file.get()
    @generate_person_file.setter
    def generate_person_file(self, value):
        self.p_generate_person_file.set(value)

    @property
    def num_participants_survey(self):
        return self.p_num_participants_survey.get()
    @num_participants_survey.setter
    def num_participants_survey(self, value):
        self.p_num_participants_survey.set(value)

    @property
    def start_date(self):
        return self.p_start_date.get()
    @start_date.setter
    def start_date(self, value):
        self.p_start_date.set(value)

    @property
    def holidays_file(self):
        return self.p_holidays_file.get()
    @holidays_file.setter
    def holidays_file(self, value):
        self.p_holidays_file.set(value)

    @property
    def age_contact_matrix_file(self):
        return self.p_age_contact_matrix_file.get()
    @age_contact_matrix_file.setter
    def age_contact_matrix_file(self, value):
        self.p_age_contact_matrix_file.set(value)

    @property
    def log_level(self):
        return self.p_log_level.get()
    @log_level.setter
    def log_level(self, value):
        if isinstance(value, LogLevel):
            value = str(value)
        self.p_log_level.set(value)

    @property
    def checkpoint_interval(self):
        return self.p_checkpoint_interval.get()
    @checkpoint_interval.setter
    def checkpoint_interval(self, value):
        self.p_checkpoint_interval.set(value)

    @property
    def uq(self):
        return self._uc
    @uq.setter
    def uq(self, value):
        if isinstance(value, PSweep):
            raise RuntimeError("Can't assign default PUQ UQ methods. User Stride{} variant.")
        elif isinstance(value, StridePSweep):
            self.strideUQ = value
        else:
            raise RuntimeError("Invalid UQ method. User Stride{} variant of PUQ UQ methods.")

    def stop(self):
        """ Stop the simulation if it's running """
        if self.simulator:
            self.simulator.Stop()

    def registerCallback(self, callback, event):
        """ Registers a callback to the simulation.

            :param callback: a function appropriate for the event type.
            :param event: either an event specified in SimulationObserver, an integer, list of events, or list of integers. A single integer is converted to a TimestepIntervalEvent. A list of integers to a list of TimestepEvents.
        """
        if isinstance(event, list):
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

    def runForks(self, *args, **kwargs):
        """ Run all forks, but not the root simulation. """
        self.setup()
        for fork in self.forks:
            fork.run(*args, **kwargs)

    def runAll(self, *args, **kwargs):
        """ Run simulation and forks. """
        self.run(*args, **kwargs)
        self.runForks(*args, **kwargs)
'''

from .Fork import Fork
