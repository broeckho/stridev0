'''
import stride
from stride.Simulation import Simulation

# Set the workspace (default = .)
stride.workspace = "simulations"

# Configure simulation
simulation = Simulation.fromFile("config/run_python.xml")
simulation.num_days = 10

# Create forks with varying parameter
for b0, ir in zip([14, 10, 5], [0.2, 0.4, 0.6]):
	# Create fork with name b0_xx
	fork = simulation.fork("b0_" + str(b0) + "-ir_" + str(ir))

	fork.disease.b0 = b0
	fork.immunity_rate = ir

	fork.r0 = 15

# run all forks
simulation.runForks()


'''
