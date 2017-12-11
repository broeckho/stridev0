from pystride.Simulation import Simulation

def vaccinate(simulator, timestep):
    if timestep == 20:
        print("Vaccinating everyone who is not yet infected")
        pop = simulator.GetPopulation()
        for pIndex in range(pop.size()):
            if pop[pIndex].GetHealth().IsSusceptible():
                pop[pIndex].GetHealth().SetImmune()

# Build simulation
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")

# Register the "vaccinate" callback
simulation.registerCallback(vaccinate)

simulation.run()
