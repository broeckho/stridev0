from .LogLevel import LogLevel

class Simulation():
    def __init__(self):
        # Default configuration parameters
        self._rng_seed = 1
        self._label = ""
        self._checkpoint_interval = 0   # only checkpoint at END of simulation
        self._num_days = 50
        #self._start_date = 01/01/2017
        self._holidays_file = "holidays_non.json"
        self._log_level = "Transmissions"
        # generate persob file = false

        # self._disease_file = "disease_measles.xml"
        self._r0 = 11
        self._seeding_fraction = 0.002
        self._seeding_age_min = 1
        self._seeding_age_max = 99
        # self._immunity_rate = 0.8
        # self._immunity_profile = "None"
        # self._vaccine_profile = "Random"
        # self._vaccine_rate = 0.8
        # self._vaccine_link_probability = 0

        # self._population_file = "pop_flanders600.csv"
        # self._contact_matrix_file = "contact_matrix_flanders_subpop.xml"
        self._num_participants_survey = 10

        # self._global_information_policy = "NoGlobalInformation"
        # self._local_information_policy = "NoLocalInformation"
        # self._belief_policy = "NoBelief"
        # self._behaviour_policy = "NoBehaviour"

    """
    Getters and setters for simulation config parameters
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

    # TODO checkpoint interval
    '''
        @property
        def checkpoint_interval(self):
            return self.p_checkpoint_interval.get()
        @checkpoint_interval.setter
        def checkpoint_interval(self, value):
            self.p_checkpoint_interval.set(value)
    '''

    @property
    def num_days(self):
        return self._num_days.get()
    @num_days.setter
    def num_days(self, value):
        self._num_days.set(value)

    # TODO start date
    '''
        @property
        def start_date(self):
            return self.p_start_date.get()
        @start_date.setter
        def start_date(self, value):
            self.p_start_date.set(value)

    '''

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

    # TODO generate_person_file
    '''
        @property
        def generate_person_file(self):
            return self.p_generate_person_file.get()
        @generate_person_file.setter
        def generate_person_file(self, value):
            self.p_generate_person_file.set(value)
    '''

    # TODO disease / disease file
    '''
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
    '''

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

    # TODO seeding age min
    # TODO seeding age max
    # TODO immunity rate
    '''
        @uq_property
        def immunity_rate(self):
            return self.p_immunity_rate.get()
        @immunity_rate.setter
        def immunity_rate(self, value):
            self.p_immunity_rate.set(value)
    '''
    # TODO immunity profile
    # TODO vaccine profile
    # TODO vaccine rate
    # TODO vaccine link probability

    # TODO population / population file
    '''
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

    '''

    # TODO contact matrix file
    '''
        @property
        def age_contact_matrix_file(self):
            return self.p_age_contact_matrix_file.get()
        @age_contact_matrix_file.setter
        def age_contact_matrix_file(self, value):
            self.p_age_contact_matrix_file.set(value)
    '''

    @property
    def num_participants_survey(self):
        return self._num_participants_survey.get()
    @num_participants_survey.setter
    def num_participants_survey(self, value):
        self._num_participants_survey.set(value)

    # TODO global information policy
    # TODO local information policy
    # TODO belief policy
    # TODO behaviour policy


# TODO load config from file
# TODO adapt config with script
# TODO show current config parameters
# TODO forks
# TODO PUQ integration
