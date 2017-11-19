from .LogLevel import LogLevel

"""
Class that keeps track of customisable simulation parameters.
"""

class SimulationConfig:
    def __init__(self):
        # Default configuration parameters
        self._rng_seed = 1
        self._label = ""
        self._checkpoint_interval = 0   # only checkpoint at END of simulation
        self._num_days = 50
        # self._start_date = 01/01/2017
        self._holidays_file = "holidays_none.json"
        self._log_level = "Transmissions"
        self._generate_person_file = False
        # self._disease_file = "disease_measles.xml"
        self._r0 = 11
        self._seeding_fraction = 0.002
        # self._seeding_age_min = 1
        # self._seeding_age_max = 99
        self._immunity_fraction = 0.8
        # self._immunity_profile = "None"
        # self._vaccine_profile = "Random"
        # self._vaccine_rate = 0.8
        # self._vaccine_link_probability = 0
        # self._population_file = "pop_flanders600.csv"
        self._contact_matrix_file = "contact_matrix_flanders_subpop.xml"
        self._num_participants_survey = 10
        # self._global_information_policy = "NoGlobalInformation"
        # self._local_information_policy = "NoLocalInformation"
        # self._belief_policy = "NoBelief"
        # self._behaviour_policy = "NoBehaviour"

    """
    Getters and setters for config parameters
    """
    @property
    def rng_seed(self):
        return self._rng_seed.get()
    @rng_seed.setter
    def rng_seed(self, value):
        self._rng_seed.set(value)

    @property
    def label(self):
        return self._label.get()
    @label.setter
    def label(self, value):
        self._label.set(value)

    @property
    def checkpoint_interval(self):
        return self._checkpoint_interval.get()
    @checkpoint_interval.setter
    def checkpoint_interval(self, value):
        self._checkpoint_interval.set(value)

    @property
    def num_days(self):
        return self._num_days.get()
    @num_days.setter
    def num_days(self, value):
        self._num_days.set(value)

    @property
    def start_date(self):
        return self._start_date.get()
    @start_date.setter
    def start_date(self, value):
        self._start_date.set(value)

    @property
    def holidays_file(self):
        return self._holidays_file.get()
    @holidays_file.setter
    def holidays_file(self, value):
        self._holidays_file.set(value)

    @property
    def log_level(self):
        return self._log_level.get()
    @log_level.setter
    def log_level(self, value):
        if isinstance(value, LogLevel):
            value = str(value)
        self._log_level.set(value)

    @property
    def generate_person_file(self):
        return self._generate_person_file.get()
    @generate_person_file.setter
    def generate_person_file(self, value):
        self._generate_person_file.set(value)

    # TODO get/set disease config file?

    @property
    def r0(self):
        return self._r0.get()
    @r0.setter
    def r0(self, value):
        self._r0.set(value)

    @property
    def seeding_fraction(self):
        return self._seeding_fraction.get()
    @seeding_fraction.setter
    def seeding_fraction(self, value):
        self._seeding_fraction.set(value)

    # TODO get/set seeding age min
    # TODO get/set seeding age max

    @property
    def immunity_fraction(self):
        return self._immunity_fraction.get()
    @immunity_fraction.setter
    def immunity_fraction(self, value):
        self._immunity_fraction.set(value)

    # TODO get/set immunity profile
    # TODO get/set vaccine profile
    # TODO vaccine link probability

    # TODO get/set population file?

    @property
    def contact_matrix_file(self):
        return self._contact_matrix_file.get()
    @contact_matrix_file.setter
    def contact_matrix_file(self, value):
        self._contact_matrix_file.set(value)

    @property
    def num_participants_survey(self):
        return self._num_participants_survey.get()
    @num_participants_survey.setter
    def num_participants_survey(self, value):
        self._num_participants_survey.set(value)

    # TODO get/set global information policy
    # TODO get/set local information policy
    # TODO get/set belief policy
    # TODO get/set behaviour policy

    """
    Load configuration from an xml file
    """
    def loadFromFile(self, filename):
        pass

    """
    Write configuration to an xml file
    """
    def writeToFile(self, filename):
        pass

    """
    Print the current configuration
    """
    def __str__(self):
        pass
