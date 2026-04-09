import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import t

np.random.seed(123)

def ci_t_student(mu, sigma, n):
    X = np.random.normal(loc=mu, scale=sigma, size=n)
    
    S = np.std(X, ddof=1)
    
    t_val = t.ppf(0.975, df=n-1)
    
    mean_X = np.mean(X)
    margin = t_val * S / np.sqrt(n)
    lower = mean_X - margin
    upper = mean_X + margin
    
    covers = (lower <= mu <= upper)
    
    return lower, upper, covers

# a)
mu = 260
sigma = 18
n = 9
simulations = 1000

results = [ci_t_student(mu, sigma, n) for _ in range(simulations)]

intervals = np.array([[r[0], r[1]] for r in results])
covers = np.array([r[2] for r in results])

coverage_empirical = np.mean(covers) * 100
mean_width = np.mean(intervals[:,1] - intervals[:,0])

print("Podpunkt a)")

print(f"Empiryczna częstość pokrycia: {coverage_empirical:.2f}%")
print(f"Średnia szerokość przedziału ufności: {mean_width:.2f}")

z_val = 1.96
known_sigma_width = 2 * z_val * sigma / np.sqrt(n)
print(f"Średnia szerokość gdy sigma znane: {known_sigma_width:.2f}")

# b)
n_plot = 100
intervals_plot = [ci_t_student(mu, sigma, n) for _ in range(n_plot)]
intervals_plot = np.array([[r[0], r[1], r[2]] for r in intervals_plot])

plt.figure(figsize=(6, 12))
for i, (l, u, cover) in enumerate(intervals_plot):
    color = 'blue' if cover else 'red'
    plt.plot([l, u], [i, i], color=color, lw=2)

plt.axvline(mu, color='black', linestyle='--')
plt.xlabel('Wartość µ')
plt.ylabel('Numer przedziału')
plt.title('Przedziały ufności dla µ')
plt.show()

percent_red = np.mean(intervals_plot[:,2] == 0) * 100
print(f"Procent odcinków czerwonych: {percent_red:.2f}%")

# c)
sample_sizes = [5, 9, 30, 100]

print("Podpunkt c)")

for n in sample_sizes:
    results = [ci_t_student(mu, sigma, n) for _ in range(1000)]
    covers = np.array([r[2] for r in results])
    coverage_empirical = np.mean(covers) * 100
    print(f"n = {n}: Empiryczna częstość pokrycia = {coverage_empirical:.2f}%")