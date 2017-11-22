from pystride.Simulation import Simulation

# Load configuration from file
simulation = Simulation()
simulation.loadRunConfig("config/run_python.xml")
# Run simulation
simulation.run()
