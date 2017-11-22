'''
import stride
from stride.Disease import Disease
from stride.Simulation import Simulation
from stride.LogLevel import LogLevel

# Load disease from configuration file
disease = Disease.fromFile("data/disease_influenza.xml")
disease.b0 = 0.35 # optional override

# Configure simulation
simulation = Simulation()
simulation.label = "simulation1"
simulation.disease = disease
simulation.population = "data/pop_antwerp.csv"
simulation.age_contact_matrix_file = "data/contact_matrix_average.xml"
simulation.holidays_file = "data/holidays_flanders_2017.json"
simulation.log_level = LogLevel.TRANSMISSIONS
simulation.start_date = "2017-01-01"
simulation.num_days = 10

# Run simulation
simulation.run()
'''
