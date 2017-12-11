from pystride.Simulation import Simulation

# The callback function
def printInfected(simulator, timestep):
    if (timestep % 2)  == 0:
        print("Amount infected at timestep {}: {}".format(
            timestep, simulator.GetPopulation().GetInfectedCount()))

# Build simulation
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")

# Register callback
simulation.registerCallback(printInfected)

simulation.run()
