import matplotlib.pyplot as plt

countries = ["Western Sahara", "Djibouti", "Qatar", "Uruguay", "Zimbabwe"]

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
    a = sections[:5]      # batch 10
    b = sections[5:10]    # batch 50
    c = sections[10:15]   # batch 1000
    return a, b, c


# === Wczytanie danych ===
sections = read_results("results.txt")
a, b, c = split_data(sections)


# === Wykresy dla każdego kraju ===
for i, country in enumerate(countries):

    minima10 = a[i][0]     # lista 100 wartości
    minima50 = b[i][0]     # lista 20 wartości
    min1000 = c[i][0][0]   # tylko jedna wartość

    plt.figure()

    # batch 10
    plt.plot(minima10, label="batch = 10")

    # batch 50 (rozciągamy na oś X żeby pasowało)
    x50 = [i * (len(minima10)//len(minima50)) for i in range(len(minima50))]
    plt.plot(x50, minima50, 'o-', label="batch = 50")

    # linia globalnego minimum
    plt.axhline(y=min1000, linestyle='--', label="min z 1000")

    plt.title(f"{country}")
    plt.xlabel("Numer grupy")
    plt.ylabel("Długość trasy")
    plt.legend()
    plt.tight_layout()

    plt.savefig(f"{country.replace(' ', '_')}.png")
    plt.show()