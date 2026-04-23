import matplotlib.pyplot as plt
import sys
import os

# =======================
# LOAD TSP COORDINATES
# =======================
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

            if read:
                parts = line.split()
                if len(parts) >= 3:
                    idx = int(parts[0])
                    x = float(parts[1])
                    y = float(parts[2])
                    coords[idx] = (x, y)

    return coords


# =======================
# LOAD ROUTE
# =======================
def read_route(filename):
    with open(filename, "r") as f:
        lines = f.readlines()

    for line in reversed(lines):
        parts = line.strip().split()

        if not parts:
            continue

        if parts[0].isdigit():
            return list(map(int, parts))

    raise ValueError("No route found in file")


# =======================
# PLOT ROUTE
# =======================
def plot_route(coords, route, title="TSP Route"):
    x, y = [], []

    for node in route:
        cx, cy = coords[node]
        x.append(cx)
        y.append(cy)

    # close loop
    x.append(x[0])
    y.append(y[0])

    plt.figure(figsize=(8, 6))
    plt.plot(x, y, marker='o')

    for node in route:
        cx, cy = coords[node]
        plt.text(cx, cy, str(node), fontsize=8)

    plt.title(title)
    plt.grid(True)
    plt.show()


# =======================
# MAIN
# =======================
if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Usage: python3 route.py <file.tsp>")
        sys.exit(1)

    tsp_name = sys.argv[1]

    tsp_file = os.path.join("data", tsp_name)
    route_file = f"task2_stats_{tsp_name}"

    if not os.path.exists(tsp_file):
        print(f"Error: {tsp_file} not found")
        sys.exit(1)

    if not os.path.exists(route_file):
        print(f"Error: {route_file} not found")
        sys.exit(1)

    coords = read_tsp(tsp_file)
    route = read_route(route_file)

    # Fix indexing if needed
    if 0 in route:
        route = [r + 1 for r in route]

    plot_route(coords, route, tsp_name)