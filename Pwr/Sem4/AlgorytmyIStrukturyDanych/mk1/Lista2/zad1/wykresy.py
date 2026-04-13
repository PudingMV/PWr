import numpy as np
import matplotlib.pyplot as plt
import sys

k = sys.argv[1]

def load_avg(file):
    data = np.loadtxt(file)
    n = data[:,0]
    c = data[:,1]
    s = data[:,2]

    unique_n = np.unique(n)

    avg_c = []
    avg_s = []

    for val in unique_n:
        mask = n == val
        avg_c.append(np.mean(c[mask]))
        avg_s.append(np.mean(s[mask]))

    return unique_n, np.array(avg_c), np.array(avg_s)

n_i, c_i, s_i = load_avg(f"results/insertion_k{k}.txt")
n_q, c_q, s_q = load_avg(f"results/quick_k{k}.txt")
n_h, c_h, s_h = load_avg(f"results/hybrid_k{k}.txt")

small = n_i

mask_q_small = n_q <= 50
mask_h_small = n_h <= 50

mask_q_big = n_q >= 1000
mask_h_big = n_h >= 1000


plt.figure()
plt.plot(n_i, c_i, 'o-', label="Insertion")
plt.plot(n_q[mask_q_small], c_q[mask_q_small], 'o-', label="Quick")
plt.plot(n_h[mask_h_small], c_h[mask_h_small], 'o-', label="Hybrid")
plt.xlabel("n")
plt.ylabel("comparisons")
plt.title(f"Porównania (małe n), k={k}")
plt.legend()
plt.grid()
plt.show()


plt.figure()
plt.plot(n_q[mask_q_big], c_q[mask_q_big], label="Quick")
plt.plot(n_h[mask_h_big], c_h[mask_h_big], label="Hybrid")
plt.xlabel("n")
plt.ylabel("comparisons")
plt.title(f"Porównania (duże n), k={k}")
plt.legend()
plt.grid()
plt.show()


plt.figure()
plt.plot(n_q[mask_q_big], c_q[mask_q_big]/n_q[mask_q_big], label="Quick")
plt.plot(n_h[mask_h_big], c_h[mask_h_big]/n_h[mask_h_big], label="Hybrid")
plt.xlabel("n")
plt.ylabel("c/n")
plt.title(f"c/n (duże n), k={k}")
plt.legend()
plt.grid()
plt.show()


plt.figure()
plt.plot(n_q[mask_q_big], s_q[mask_q_big]/n_q[mask_q_big], label="Quick")
plt.plot(n_h[mask_h_big], s_h[mask_h_big]/n_h[mask_h_big], label="Hybrid")
plt.xlabel("n")
plt.ylabel("s/n")
plt.title(f"s/n (duże n), k={k}")
plt.legend()
plt.grid()
plt.show()