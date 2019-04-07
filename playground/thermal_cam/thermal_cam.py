import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from scipy.interpolate import griddata


ser = serial.Serial('/dev/tty.SLAB_USBtoUART', baudrate=115200)

w, h = 8, 8;
matrix = np.zeros((8,8))

points = [((ix // 8), (ix % 8)) for ix in range(0, 64)]
grid_x, grid_y = np.mgrid[0:7:64j, 0:7:64j]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

colorbar_shown = False

def animate(i):
    # Decode data from the serial port
    line = ser.readline().decode()
    
    # Make sure the line is valid
    temperatures = line.split(",")[:64]
    try:
        temperatures = [float(temp) for temp in temperatures]
    except:
        return
    if len(temperatures) != 64:
        return

    # Interpolate the data to show finer images
    bicubic = griddata(points, temperatures, (grid_x, grid_y), method='cubic')

    ax.clear()
    # Set the range of values displayed
    plt.imshow(bicubic, vmin=25, vmax=40)
    # Show a color bar
    global colorbar_shown
    if not colorbar_shown:
        plt.colorbar()
        colorbar_shown = True

# Animate the plot
ani = animation.FuncAnimation(fig, animate, interval=10)
plt.show()
