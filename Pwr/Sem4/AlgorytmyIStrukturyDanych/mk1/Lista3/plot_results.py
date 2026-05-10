import pandas as pd
import matplotlib.pyplot as plt

# =========================
# LOAD DATA
# =========================

rand_df = pd.read_csv("randomized_results.csv")
sel_df = pd.read_csv("select_results.csv")

# =========================
# CLEAN AGGREGATION
# (average over all k for each n)
# =========================

rand_n = rand_df.groupby("n").mean(numeric_only=True).reset_index()
sel_n = sel_df.groupby("n").mean(numeric_only=True).reset_index()

# =========================
# 1. RANDOMIZED SELECT - comparisons
# =========================

plt.figure()
plt.plot(rand_n["n"], rand_n["comparisons"], label="Randomized Select")

plt.title("Randomized Select - Average Comparisons")
plt.xlabel("n")
plt.ylabel("Comparisons")
plt.legend()
plt.grid()
plt.show()

# =========================
# 2. RANDOMIZED SELECT - swaps
# =========================

plt.figure()
plt.plot(rand_n["n"], rand_n["swaps"], label="Randomized Select")

plt.title("Randomized Select - Average Swaps")
plt.xlabel("n")
plt.ylabel("Swaps")
plt.legend()
plt.grid()
plt.show()

# =========================
# 3. SELECT (MoM) - comparisons
# =========================

plt.figure()
plt.plot(sel_n["n"], sel_n["comparisons"], label="Median of Medians")

plt.title("SELECT (MoM) - Average Comparisons")
plt.xlabel("n")
plt.ylabel("Comparisons")
plt.legend()
plt.grid()
plt.show()

# =========================
# 4. FINAL COMPARISON (FIXED - NO k DEPENDENCY)
# =========================

plt.figure()

plt.plot(rand_n["n"], rand_n["comparisons"], label="Randomized Select")
plt.plot(sel_n["n"], sel_n["comparisons"], label="SELECT (MoM)")

plt.title("Algorithm Comparison - Average Behavior")
plt.xlabel("n")
plt.ylabel("Comparisons")
plt.legend()
plt.grid()
plt.show()