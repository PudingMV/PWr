import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

g = df.groupby(["algo", "n"]).mean().reset_index()

algos = g["algo"].unique()

def get(algo, col):
    d = g[g["algo"] == algo]
    return d["n"].values, d[col].values

plt.figure()
for a in algos:
    n, c = get(a, "c")
    plt.plot(n, c, label=a)

plt.title("Comparisons (c)")
plt.legend()
plt.grid()
plt.show()

plt.figure()
for a in algos:
    n, s = get(a, "s")
    plt.plot(n, s, label=a)

plt.title("Swaps (s)")
plt.legend()
plt.grid()
plt.show()

plt.figure()
for a in algos:
    n, c = get(a, "c")
    plt.plot(n, c/n, label=a)

plt.title("c/n")
plt.legend()
plt.grid()
plt.show()

plt.figure()
for a in algos:
    n, s = get(a, "s")
    plt.plot(n, s/n, label=a)

plt.title("s/n")
plt.legend()
plt.grid()
plt.show()