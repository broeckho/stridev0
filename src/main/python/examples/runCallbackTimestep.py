'''
from stride.Simulation import Simulation

def vaccinate(simulation, timestep):
	print("Vaccinating everyone who is not yet infected")
	for person in simulation.population:
		if person.GetHealth().IsSusceptible():
			person.GetHealth().SetImmune()


# Build simulation
simulation = Simulation.fromFile("config/run_python.xml")

# Register "vaccinate" callback at 20th timestep
simulation.registerCallback(vaccinate, [20])

simulation.run()

'''
