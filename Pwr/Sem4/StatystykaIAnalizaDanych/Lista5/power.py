import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

mu0 = 200
sigma = 18
alpha = 0.05
z_crit = norm.ppf(1 - alpha/2)   # ≈ 1.96

B = 1000
n = 36

rng = np.random.default_rng(123)  # replikowalność


# (a) Błąd I rodzaju (μ = μ0)
mu = mu0
rejections = 0

for _ in range(B):
    sample = rng.normal(mu, sigma, n)
    xbar = np.mean(sample)

    Z = (xbar - mu0) / (sigma / np.sqrt(n))

    if abs(Z) > z_crit:
        rejections += 1

type_I_error = rejections / B

print("(a) Błąd I rodzaju")
print(f"Liczba odrzuceń H0: {rejections}")
print(f"Empiryczny poziom istotności: {type_I_error:.3f}")
print()


# (b) Empiryczna moc dla różnych μ
mus = [202, 205, 210, 220]
power_empirical = []
power_theoretical = []

def theoretical_power(mu):
    delta = (mu - mu0) / (sigma / np.sqrt(n))
    return norm.cdf(-z_crit - delta) + 1 - norm.cdf(z_crit - delta)

print("(b) Moc testu")

for mu in mus:
    rejections = 0

    for _ in range(B):
        sample = rng.normal(mu, sigma, n)
        xbar = np.mean(sample)

        Z = (xbar - mu0) / (sigma / np.sqrt(n))

        if abs(Z) > z_crit:
            rejections += 1

    emp_power = rejections / B
    th_power = theoretical_power(mu)

    power_empirical.append(emp_power)
    power_theoretical.append(th_power)

    print(f"μ = {mu}: empiryczna = {emp_power:.3f}, teoretyczna = {th_power:.3f}")

print()


# (c) Krzywa mocy (empiryczna vs teoretyczna)
mu_grid = np.linspace(185, 215, 60)

emp_curve = []

for mu in mu_grid:
    rejections = 0

    for _ in range(B):
        sample = rng.normal(mu, sigma, n)
        xbar = np.mean(sample)

        Z = (xbar - mu0) / (sigma / np.sqrt(n))

        if abs(Z) > z_crit:
            rejections += 1

    emp_curve.append(rejections / B)

theo_curve = [theoretical_power(mu) for mu in mu_grid]

plt.figure()
plt.plot(mu_grid, emp_curve, label="Empiryczna moc")
plt.plot(mu_grid, theo_curve, label="Teoretyczna moc")
plt.axvline(mu0, linestyle="--")
plt.title("Krzywa mocy testu (n=36)")
plt.xlabel("μ")
plt.ylabel("Moc")
plt.legend()
plt.show()


# (d) Krzywe mocy dla różnych n
mu_grid = np.linspace(185, 215, 100)
n_values = [9, 36, 100]

plt.figure()

for n_val in n_values:

    def power(mu):
        delta = (mu - mu0) / (sigma / np.sqrt(n_val))
        return norm.cdf(-z_crit - delta) + 1 - norm.cdf(z_crit - delta)

    curve = [power(mu) for mu in mu_grid]
    plt.plot(mu_grid, curve, label=f"n = {n_val}")

plt.axvline(mu0, linestyle="--")
plt.title("Krzywe mocy dla różnych n")
plt.xlabel("μ")
plt.ylabel("Moc")
plt.legend()
plt.show()