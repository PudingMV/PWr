import matplotlib.pyplot as plt
import numpy as np
import sys
import os

##############################################################
# LOAD TSP COORDINATES
##############################################################

def read_tsp(filename):

    coords = {}

    with open(filename, "r") as f:

        read = False

        for line in f:

            line = line.strip()

            if "NODE_COORD_SECTION" in line:
                read = True
                continue

            if "EOF" in line:
                break

            if not read:
                continue

            parts = line.split()

            if len(parts) >= 3:

                idx = int(parts[0])
                x = float(parts[1])
                y = float(parts[2])

                coords[idx] = (x, y)

    return coords


##############################################################
# LOAD ROUTE FROM SA OUTPUT
##############################################################

def read_route(filename):

    with open(filename, "r") as f:
        lines = f.readlines()

    route = []
    reading = False

    for line in lines:

        line = line.strip()

        if "Route:" in line:
            reading = True
            continue

        if reading:

            parts = line.split()

            for p in parts:
                if p.isdigit():
                    route.append(int(p))

    if not route:
        raise ValueError("No route found in file")

    return route


##############################################################
# PLOT ROUTE + INTERACTIVE ROTATE/FLIP
##############################################################

def plot_route(coords, route, title="Simulated Annealing Route"):

    x = []
    y = []

    ##########################################################
    # BUILD PATH
    ##########################################################

    for node in route:

        if node not in coords:
            continue

        cx, cy = coords[node]

        x.append(cx)
        y.append(cy)

    ##########################################################
    # CLOSE LOOP
    ##########################################################

    x.append(x[0])
    y.append(y[0])

    points = np.column_stack((x, y))

    ##########################################################
    # CREATE FIGURE
    ##########################################################

    fig, ax = plt.subplots(figsize=(10, 8))

    line, = ax.plot(
        points[:, 0],
        points[:, 1],
        linewidth=1
    )

    ax.set_title(title)
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.grid(True)

    # Keep proportions correct after rotation
    ax.set_aspect('equal', adjustable='box')

    ##########################################################
    # REDRAW FUNCTION
    ##########################################################

    def redraw():

        line.set_data(
            points[:, 0],
            points[:, 1]
        )

        ax.relim()
        ax.autoscale_view()

        fig.canvas.draw_idle()

    ##########################################################
    # KEYBOARD EVENTS
    ##########################################################

    def on_key(event):

        nonlocal points

        ######################################################
        # ROTATE 90°
        ######################################################

        if event.key == 'r':

            rotation = np.array([
                [0, -1],
                [1,  0]
            ])

            points = points @ rotation.T

            redraw()

        ######################################################
        # MIRROR HORIZONTAL
        ######################################################

        elif event.key == 'h':

            points[:, 0] *= -1

            redraw()

        ######################################################
        # MIRROR VERTICAL
        ######################################################

        elif event.key == 'v':

            points[:, 1] *= -1

            redraw()

        ######################################################
        # QUIT
        ######################################################

        elif event.key == 'q':

            plt.close(fig)

    fig.canvas.mpl_connect(
        'key_press_event',
        on_key
    )

    plt.tight_layout()
    plt.show()


##############################################################
# MAIN
##############################################################

if __name__ == "__main__":

    if len(sys.argv) != 2:

        print("Usage:")
        print("python3 plot_sa.py <file.tsp>")

        sys.exit(1)

    tsp_name = sys.argv[1]

    ##########################################################
    # FILE PATHS
    ##########################################################

    tsp_file = os.path.join(
        "data",
        tsp_name
    )

    route_file = f"IslandGA_{tsp_name}"

    ##########################################################
    # CHECK FILES
    ##########################################################

    if not os.path.exists(tsp_file):

        print(f"ERROR: {tsp_file} not found")
        sys.exit(1)

    if not os.path.exists(route_file):

        print(f"ERROR: {route_file} not found")
        sys.exit(1)

    ##########################################################
    # LOAD DATA
    ##########################################################

    coords = read_tsp(tsp_file)

    route = read_route(route_file)

    ##########################################################
    # PLOT
    ##########################################################

    plot_route(
        coords,
        route,
        f"Genetic Algorithm - {tsp_name}"
    )