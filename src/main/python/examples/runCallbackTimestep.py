'''
def vaccinate(simulation, timestep):
	print("Vaccinating everyone who is not yet infected")
	for person in simulation.population:
		if person.GetHealth().IsSusceptible():
			person.GetHealth().SetImmune()

'''

from pystride.Simulation import Simulation

def vaccinate(simulator, timestep):
    print("Vaccinating everyone who is not yet infected")

# Build simulation
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")

# Register the "vaccinate" callback
simulation.registerCallback(vaccinate)

simulation.run()
