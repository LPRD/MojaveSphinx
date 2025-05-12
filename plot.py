import matplotlib.pyplot as plt
import numpy as np

# Load data
data = np.loadtxt("05-07/DATA5.txt", dtype="float", skiprows=1, delimiter=',')

# Extract columns
time_data = data[:,0]/1000
lc_data = data[:,1]
tc_data = data[:,2]
ox_data = data[:,3]

# Create the figure and axes
fig, ax1 = plt.subplots()
fig.subplots_adjust(right=0.8)  # Add space on the right side

# Create a second y-axis for tc_data
tc_filter = tc_data>-5
ax1.set_ylim(-5, 100)  # Sets TC Data y-axis from 0 to 100
ax1.plot(time_data[tc_filter], tc_data[tc_filter], 'r-', label='TC Data')
ax1.set_ylabel('TC Data', color='r')
ax1.tick_params(axis='y', labelcolor='r')

# Plot ox_data on the first y-axis
ax2 = ax1.twinx()
ox_filter = np.logical_and(ox_data < 900, ox_data>-10)
ax2.plot(time_data[ox_filter], ox_data[ox_filter], 'b-', label='Ox Data')
ax2.set_ylabel('Ox Data', color='b')
ax2.tick_params(axis='y', labelcolor='b')

# Plot lc_data on the same figure 
ax3 = ax1.twinx()
ax3.spines["right"].set_position(("axes", 1.15))  # Offset the axis
lc_filter = lc_data < 0
ax3.plot(time_data[lc_filter], lc_data[lc_filter], 'g-', label='LC Data')
ax3.set_ylabel('LC Data', color='g')
ax3.tick_params(axis='y', labelcolor='g')

# Show the plots
plt.show()
