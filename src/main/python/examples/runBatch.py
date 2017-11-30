import pystride
from pystride.Simulation import Simulation

# Set the workspace (default = .)
#pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")
simulation.setRunConfigParam("num_days", 10)

# Create forks with varying parameters
for b0, ir in zip([14, 10, 5],[0.2, 0.4, 0.6]): #TODO for b0, ir in zip([14, 10, 5], [0.2, 0.4, 0.6])
    # Create fork
    fork = simulation.fork("b0_" + str(b0) + "-ir_" + str(ir)) #TODO add b0 to name
    fork.setDiseaseConfigParam("transmission/b0", b0)
    fork.setRunConfigParam("immunity_rate", ir)
    fork.setRunConfigParam("r0", 15)

# run all forks
simulation.runForks()
