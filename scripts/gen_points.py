import numpy
import matplotlib.pyplot as plt
mean=[50, 50]
sigma=[[100, 0], [0, 100]]
data = numpy.random.multivariate_normal(mean, sigma, 100)
x,y=data.T
for point in data:
	print "votingSpace->vote(" + str(point[0]) + "," + str(point[1]) +", 0);"

plt.plot(x, y, "x")
plt.axis("equal")
plt.show()
