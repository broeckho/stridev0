import pystride
from pystride.Simulation import Simulation

# Configure simulation
simulation = Simulation()

# Set disease
simulation.setRunConfigParam("disease_config_file", "data/disease_influenza.xml")
simulation.setDiseaseConfigParam("transmission/b0", 0.35)

# Set other parameters
simulation.setRunConfigParam("rng_seed", 1)
simulation.setRunConfigParam("r0", 2.8)
simulation.setRunConfigParam("seeding_rate", 0.005)
simulation.setRunConfigParam("seeding_age_min", 1)
simulation.setRunConfigParam("seeding_age_max", 99)
simulation.setRunConfigParam("immunity_rate", 0.8)
simulation.setRunConfigParam("immunity_profile", "None")
simulation.setRunConfigParam("vaccine_profile", "Random")
simulation.setRunConfigParam("vaccine_rate", 0.8)
simulation.setRunConfigParam("vaccine_link_probability", 0)
simulation.setRunConfigParam("population_file", "data/pop_antwerp.csv")
simulation.setRunConfigParam("num_days", 10)
simulation.setRunConfigParam("output_prefix", "simulation1")
simulation.setRunConfigParam("generate_person_file", 1)
simulation.setRunConfigParam("num_participants_survey", 0)
simulation.setRunConfigParam("start_date", "2017-01-01")
simulation.setRunConfigParam("holidays_file", "data/holidays_flanders_2017.json")
simulation.setRunConfigParam("age_contact_matrix_file", "data/contact_matrix_flanders_15touch.xml")
simulation.setRunConfigParam("log_level", "Transmissions")
#<local_information_policy>NoLocalInformation</local_information_policy>
#<global_information_policy>NoGlobalInformation</global_information_policy>
#<belief_policy>NoBelief</belief_policy>
#<behaviour_policy>NoBehaviour</behaviour_policy>

# Run simulation
simulation.run()
