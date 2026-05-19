import matplotlib.pyplot as plt

############################################################
# CONFIG
############################################################

TSP_FILE = "data/tz6117.tsp"
ROUTE_FILE = "best_route.txt"

############################################################
# READ TSPLIB
############################################################

cities = {}

with open(TSP_FILE, "r") as f:

    read_coords = False

    for line in f:

        line = line.strip()

        if "NODE_COORD_SECTION" in line:
            read_coords = True
            continue

        if "EOF" in line:
            break

        if not read_coords:
            continue

        parts = line.split()

        if len(parts) >= 3:

            city_id = int(parts[0])
            x = float(parts[1])
            y = float(parts[2])

            cities[city_id] = (x, y)

print(f"[DEBUG] Loaded {len(cities)} cities")

############################################################
# READ BEST ROUTE
############################################################

route = []

with open(ROUTE_FILE, "r") as f:

    lines = f.readlines()

    ########################################################
    # Find line with route
    ########################################################

    for line in lines:

        line = line.strip()

        if not line:
            continue

        if line.startswith("COST"):
            continue

        parts = line.split()

        ####################################################
        # Route line
        ####################################################

        if len(parts) > 10:
            route = list(map(int, parts))
            break

print(f"[DEBUG] Route length = {len(route)}")

############################################################
# BUILD COORDINATES
############################################################

x_coords = []
y_coords = []

for city_id in route:

    x, y = cities[city_id]

    x_coords.append(x)
    y_coords.append(y)

############################################################
# CLOSE THE TOUR
############################################################

first_x, first_y = cities[route[0]]

x_coords.append(first_x)
y_coords.append(first_y)

############################################################
# PLOT
############################################################

plt.figure(figsize=(12, 12))

############################################################
# Draw route
############################################################

plt.plot(
    x_coords,
    y_coords,
    linewidth=0.8)

############################################################
# Draw cities
############################################################

#plt.scatter(
    #x_coords[:-1],
    #y_coords[:-1],
    #s=8)

############################################################
# Labels
############################################################

plt.title("TSP Best Route")
plt.xlabel("X")
plt.ylabel("Y")

plt.axis("equal")

############################################################
# SAVE
############################################################

OUTPUT_IMAGE = "best_route.png"

plt.savefig(
    OUTPUT_IMAGE,
    dpi=300,
    bbox_inches="tight")

print(f"[DEBUG] Saved plot to {OUTPUT_IMAGE}")

############################################################
# SHOW
############################################################

plt.show()