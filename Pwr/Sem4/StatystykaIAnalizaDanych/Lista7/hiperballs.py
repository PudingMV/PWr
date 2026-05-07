import numpy as np
import matplotlib.pyplot as plt
from math import gamma, pi

np.random.seed(42)

dimensions = range(1, 21)
N = 20000

def volume_ball(d):
    return (pi ** (d / 2)) / gamma(d / 2 + 1)

analytic_volumes = []
mc_volumes = []
ratios = []

for d in dimensions:
    V_ball = volume_ball(d)
    V_cube = 2 ** d
    ratio_true = V_ball / V_cube

    analytic_volumes.append(ratio_true)

    X = np.random.uniform(-1, 1, size=(N, d))
    norms = np.linalg.norm(X, axis=1)

    inside = np.sum(norms <= 1)
    ratio_mc = inside / N

    mc_volumes.append(ratio_mc)

    print(f"d={d:2d} | analytic={ratio_true:.6e} | MC={ratio_mc:.6e}")

plt.figure(figsize=(8,5))
plt.plot(list(dimensions), analytic_volumes, label="Analytical", marker='o')
plt.plot(list(dimensions), mc_volumes, label="Monte Carlo", marker='x')

plt.yscale('log')
plt.xlabel("Dimension d")
plt.ylabel("Volume ratio V_ball / V_cube")
plt.title("Vanishing volume of high-dimensional ball")
plt.grid(True)
plt.legend()

plt.savefig("hiperkula_mc.png", dpi=200)
print("Saved plot -> hiperkula_mc.png")