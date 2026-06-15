import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

plt.plot(df["n"], df["prim"], label="Prim")
plt.plot(df["n"], df["kruskal"], label="Kruskal")

plt.xlabel("n")
plt.ylabel("czas [s]")
plt.title("Prim vs Kruskal (MST)")
plt.legend()
plt.grid()
plt.savefig("mst_algorithms_comparison.png", dpi=300)
plt.show()