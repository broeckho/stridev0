'''
from stride.Simulation import Simulation

# The callback function.
def printInfected(simulation, timestep):
	print("Amount infected at timestep {}: {}".format(
		timestep, simulation.population.GetInfectedCount()))

# Build simulation
simulation = Simulation.fromFile("config/run_python.xml")

# Register "printInfected" callback every timestep (interval = 1)
simulation.registerCallback(printInfected, 1)

simulation.run()


'''
