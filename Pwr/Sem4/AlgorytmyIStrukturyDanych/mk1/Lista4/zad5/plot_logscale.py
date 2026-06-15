import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

n = df["n"].values.astype(float)
prim = df["prim"].values.astype(float)
kruskal = df["kruskal"].values.astype(float)

prim_theory = n**2
kruskal_theory = (n**2) * np.log2(n)

prim_c = np.mean(prim / prim_theory)
kruskal_c = np.mean(kruskal / kruskal_theory)

prim_fit = prim_c * prim_theory
kruskal_fit = kruskal_c * kruskal_theory

plt.figure(figsize=(10, 6))

plt.plot(n, prim, label="Prim (measured)", linewidth=2)
plt.plot(n, kruskal, label="Kruskal (measured)", linewidth=2)

plt.plot(n, prim_fit, "--", label="Prim fit: c·n²", linewidth=2)
plt.plot(n, kruskal_fit, "--", label="Kruskal fit: c·n²logn", linewidth=2)

plt.xlabel("n (number of vertices)")
plt.ylabel("Time (seconds)")
plt.title("MST Algorithms: Measured vs Fitted Complexity")

plt.grid(True, alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig("logscale_mst_algorithms_comparison.png", dpi=300)
plt.show()

