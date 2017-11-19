from .SimulationConfig import SimulationConfig

class Simulation():
    def __init__(self):
        self.config = SimulationConfig()
        # TODO self.disease
        # TODO self.population
        # TODO self.simulator

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

# TODO run
# TODO stop
# TODO forks
# TODO PUQ integration
