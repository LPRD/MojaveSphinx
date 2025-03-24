import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("DATA.txt", dtype="float", skiprows=1, delimiter=',')
ox_data = data[:,3]
print(np.min(ox_data))
print(np.max(ox_data))
plt.plot(ox_data)
plt.show()
