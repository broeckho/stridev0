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

from pystride.Simulation import Simulation

def shout(simulation, timestep):
    print("Timestep " + str(timestep))

# Build simulation
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")

# Register
simulation.registerCallback(shout, None)

simulation.run()
