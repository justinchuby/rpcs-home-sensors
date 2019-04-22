import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from scipy.interpolate import griddata
import requests
import json

w, h = 8, 8
matrix = np.zeros((8, 8))

points = [((ix // 8), (ix % 8)) for ix in range(0, 64)]
grid_x, grid_y = np.mgrid[0:7:64j, 0:7:64j]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

colorbar_shown = False

def animate(i):
    resp = requests.get('http://kinect.andrew.cmu.edu:8000/hs/events', auth=('readonly_user', 'rpcs_readonly2019'))
    data = resp.json()[-1]["data"]
    data = json.loads(data)

    # Make sure the line is valid
    try:
        temperatures = data["value"]
    except:
        return

    # Interpolate the data to show finer images
    bicubic = griddata(points, temperatures, (grid_x, grid_y), method="cubic")

    ax.clear()
    # Set the range of values displayed
    plt.imshow(bicubic, vmin=30, vmax=100)
    # Show a color bar
    global colorbar_shown
    if not colorbar_shown:
        plt.colorbar()
        colorbar_shown = True


# Animate the plot
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()
