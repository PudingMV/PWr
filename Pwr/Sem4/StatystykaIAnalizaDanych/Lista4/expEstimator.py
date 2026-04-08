import numpy as np
import matplotlib.pyplot as plt

np.random.seed(123)


# a)
n = 30
lam0 = 2

sample = np.random.exponential(scale=1/lam0, size=n)

# b)
lam = np.linspace(0.5, 4, 200)
loglik = n * np.log(lam) - lam * np.sum(sample)

lam_est = 1 / np.mean(sample)

plt.plot(lam, loglik, label="log-wiarygodność")
plt.axvline(lam_est, color='red', linestyle='--', label=f"MLE = {lam_est:.2f}")
plt.xlabel("λ")
plt.ylabel("ℓ(λ)")
plt.legend()
plt.title("Log-wiarygodność")
plt.show()


# c)
ns = [5, 30, 200]
lam = np.linspace(0.5, 4, 200)

plt.figure()

for n in ns:
    sample = np.random.exponential(scale=1/lam0, size=n)
    loglik = n * np.log(lam) - lam * np.sum(sample)
    plt.plot(lam, loglik / n, label=f"n={n}")

plt.xlabel("λ")
plt.ylabel("ℓ(λ)/n")
plt.legend()
plt.title("Porównanie log-wiarygodności / n")
plt.show()


# d)
B = 1000
ns = [5, 30, 200]

results = {}

for n in ns:
    estimates = []
    
    for _ in range(B):
        sample = np.random.exponential(scale=1/lam0, size=n)
        lam_hat = 1 / np.mean(sample)
        estimates.append(lam_hat)
    
    estimates = np.array(estimates)
    results[n] = {
        "mean": np.mean(estimates),
        "bias": np.mean(estimates) - lam0
    }

for n in results:
    print(f"n = {n}")
    print(f"  średnia λ̂ = {results[n]['mean']:.4f}")
    print(f"  bias = {results[n]['bias']:.4f}")
    print()