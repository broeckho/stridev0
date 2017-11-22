'''
import stride
from stride.Generator import Generator

g = Generator.fromFile("config/run_generator_default.xml")

print("Generating population")
society = g.generate()

print("Calculating age distribution")
persons = society.GetPersons()
ages = [0] * 100
for person in persons:
	age = person.GetAge()
	ages[age] += 1

# print age distribution
for age, amount in enumerate(ages):
	frequency = amount/len(persons) * 100
	print("Age: {:2d}\tfreq: {:.2f}%".format(age, frequency))


'''
