import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

g = df.groupby("mn").mean().reset_index()

mn = g["mn"].values
c = g["comparisons"].values

plt.figure(figsize=(10,6))
plt.plot(mn, c, marker="o", label="comparisons")

plt.title("Number of comparisons vs input size")
plt.xlabel("m * n (matrix size)")
plt.ylabel("comparisons")
plt.grid()
plt.legend()
plt.show()

theoretical = 2 * np.sqrt(mn)

plt.figure(figsize=(10,6))
plt.plot(mn, c, marker="o", label="experimental")
plt.plot(mn, theoretical, "--", label="O(m+n) theory")

plt.title("Experimental vs theoretical complexity")
plt.xlabel("m * n")
plt.ylabel("comparisons")
plt.grid()
plt.legend()
plt.show()