'''
import stride
from stride.Simulation import Simulation

# Set the workspace (default = .)
stride.workspace = "simulations"

# Configure simulation
simulation = Simulation.fromFile("config/run_python.xml")
simulation.num_days = 10

# Create 5 identical forks except for the seed
for index in range(5):
	fork = simulation.fork("instance" + str(index))
	fork.rng_seed = index

# run all forks
simulation.runForks()


'''
