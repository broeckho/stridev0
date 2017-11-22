'''
import stride

from stride.Simulation import Simulation
from stride.PUQIntegration import *

stride.workspace = "simulations"

# Configure simulation
simulation = Simulation.fromFile("config/run_python.xml")
simulation.num_days = 10
simulation.generate_person_file = False

# Add parameter to aproximate
simulation.r0 = StrideUniformParameter(min=10, max=15)
simulation.disease.b0 = StrideUniformParameter(min=1, max=2)

# Set UQ method
simulation.uq = StrideLHS(num=2, ds=True)

# Run simulation
simulation.run()

'''
