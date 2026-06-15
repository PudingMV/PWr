import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("broadcast_results.csv")

plt.figure(figsize=(10, 6))

plt.plot(df["n"], df["min"],
         label="Minimum")

plt.plot(df["n"], df["avg"],
         label="Average")

plt.plot(df["n"], df["max"],
         label="Maximum")

plt.xlabel("Number of vertices (n)")
plt.ylabel("Number of rounds")
plt.title("Broadcast time in MST trees")

plt.grid(True)
plt.legend()

plt.savefig("broadcast_plot.png", dpi=300)

plt.show()