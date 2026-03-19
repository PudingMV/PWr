import matplotlib.pyplot as plt

countries = ["Western Sahara", "Djibouti", "Qatar", "Uruguay", "Zimbabwe"]
files = ["wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp"]


def read_results(filename):
    with open(filename) as f:
        lines = [line.strip() for line in f if line.strip() != ""]

    sections = []
    current = []

    for line in lines:
        if all(c.isdigit() or c.isspace() for c in line):
            values = list(map(int, line.split()))
            current.append(values)
        else:
            avg = float(line)
            current.append(avg)
            sections.append(current)
            current = []

    return sections


def split_data(sections):
    return sections[:5], sections[5:10], sections[10:15]


def read_tsp(filename):
    coords = []
    read = False

    with open(filename) as f:
        for line in f:
            if "NODE_COORD_SECTION" in line:
                read = True
                continue
            if "EOF" in line:
                break
            if read:
                parts = line.split()
                if len(parts) == 3:
                    coords.append((float(parts[1]), float(parts[2])))
    return coords


def read_best_routes(filename):
    routes = {}
    with open(filename) as f:
        lines = [line.strip() for line in f if line.strip()]

    i = 0
    while i < len(lines):
        name = lines[i].split("/")[-1].strip()  
        perm = list(map(int, lines[i+1].split()))
        length = int(lines[i+2])
        routes[name] = (perm, length)
        i += 3

    return routes


def read_optimal(filename):
    optimal = {}
    with open(filename) as f:
        for line in f:
            name, val = line.split("-")
            optimal[name.strip()] = int(val.strip())
    return optimal


def plot_route(coords, perm, title):
    x = [coords[i-1][0] for i in perm] + [coords[perm[0]-1][0]]
    y = [coords[i-1][1] for i in perm] + [coords[perm[0]-1][1]]

    plt.figure()
    plt.plot(x, y, '-o')
    plt.title(title)
    plt.tight_layout()
    plt.show()



sections = read_results("results.txt")
a, b, c = split_data(sections)

avg10 = [x[-1] for x in a]
avg50 = [x[-1] for x in b]
min1000 = [x[0][0] for x in c]

routes = read_best_routes("bestFoundRoutes.txt")
optimal = read_optimal("data/bestRoutes.txt")


for i, country in enumerate(countries):

    minima10 = a[i][0]
    minima50 = b[i][0]
    best = min1000[i]

    plt.figure()

    plt.plot(minima10, label="batch 10")

    x50 = [i * (len(minima10)//len(minima50)) for i in range(len(minima50))]
    plt.plot(x50, minima50, 'o-', label="batch 50")

    plt.axhline(y=best, linestyle='--', label="min 1000")

    plt.title(country)
    plt.legend()
    plt.tight_layout()
    plt.show()


for f in files:
    coords = read_tsp("data/" + f)
    perm, length = routes[f]

    plot_route(coords, perm, f"{f} | best found = {length}")


print("\nTabela porównawcza:\n")
print(f"{'Kraj':20} {'avg10':10} {'avg50':10} {'min1000':10} {'optimum':10} {'% error':10}")

for i, country in enumerate(countries):
    opt = optimal[country]

    error = 100 * (min1000[i] - opt) / opt

    print(f"{country:20} {avg10[i]:<10.2f} {avg50[i]:<10.2f} {min1000[i]:<10} {opt:<10} {error:<10.2f}")