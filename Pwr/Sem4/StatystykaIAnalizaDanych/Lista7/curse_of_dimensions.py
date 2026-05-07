import numpy as np
import matplotlib.pyplot as plt

n = 500
dimensions = [1, 2, 5, 10, 50, 100, 500]

def compute_R(n, d):
    X = np.random.rand(n, d)
    
    diff = X[:, np.newaxis, :] - X[np.newaxis, :, :]
    dist_matrix = np.linalg.norm(diff, axis=2)
    
    dist_min_matrix = dist_matrix.copy()
    np.fill_diagonal(dist_min_matrix, np.inf)
    
    dmin = np.min(dist_min_matrix, axis=1)
    
    dmax = np.max(dist_matrix, axis=1)
    
    mask = dmin > 1e-12
    R = np.mean((dmax[mask] - dmin[mask]) / dmin[mask])
    
    return R

R_values = []
for d in dimensions:
    R = compute_R(n, d)
    R_values.append(R)
    print(f"d={d}, R(d)={R:.4f}")

plt.figure(figsize=(8,5))
plt.plot(dimensions, R_values, marker='o')
plt.xscale('log')
plt.xlabel('Wymiar d (skala log)')
plt.ylabel('R(d)')
plt.title('Koncentracja odległości')
plt.grid(True)

plt.savefig("R_plot.png", dpi=200)
print("Saved plot as R_plot.png")