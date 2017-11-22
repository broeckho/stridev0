'''
import stride
from stride.Simulation import Simulation
from stride.Generator import Generator

s = Simulation.fromFile("config/run_python.xml")

g = Generator.fromFile("config/run_generator_default.xml")
s.population = g.generateLazy()
s.run()


'''
