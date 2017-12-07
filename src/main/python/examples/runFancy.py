'''
import stride

from stride.Simulation import Simulation
from stride.SimulationObserver import TimestepIntervalEvent

stride.workspace = "simulations"


def checkFinished(simulation, timestep):
    pop = simulation.population
    infected = pop.GetInfectedCount()
    print("Amount infected:", infected)
    if infected < 8000:
        simulation.num_days = simulation.num_days + 1
    else:
        simulation.stop()


def mutateDisease(simulation, timestep):
    print("Mutating disease.")
    simulation.disease.b0 = 8
    simulation.disease.b1 = 10


# Configure simulation
simulation = Simulation.fromFile("config/run_python.xml")
simulation.generate_person_file = False

simulation.registerCallback(checkFinished, TimestepIntervalEvent(1, beforeTS=True))

fork = simulation.fork("mutate")
fork.registerCallback(mutateDisease, [5])

simulation.runAll()


'''
