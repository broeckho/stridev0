from pystride.Simulation import Simulation

# Load configuration from file
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")
# Print the loaded configuration
simulation.showRunConfig()
# Run simulation
simulation.run()
