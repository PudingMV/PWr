import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# ============================================================
# Wczytanie danych
# ============================================================

df = pd.read_csv("Credit.csv")

# Jeśli w pliku istnieje kolumna z indeksami (np. "Unnamed: 0"),
# można ją usunąć:
df = df.loc[:, ~df.columns.str.contains("^Unnamed")]

x = df["Income"].to_numpy()
y = df["Limit"].to_numpy()

n = len(df)

# ============================================================
# a) Empiryczny współczynnik korelacji Pearsona
# ============================================================

rho_hat = np.corrcoef(x, y)[0, 1]

print("=" * 60)
print(f"Liczba obserwacji n = {n}")
print(f"Empiryczny współczynnik korelacji rho_hat = {rho_hat:.6f}")

# ============================================================
# b) Bootstrap
# ============================================================

B = 10_000
rng = np.random.default_rng(seed=123)

rho_boot = np.empty(B)

for b in range(B):
    indices = rng.integers(0, n, size=n)  # losowanie ze zwracaniem

    x_star = x[indices]
    y_star = y[indices]

    rho_boot[b] = np.corrcoef(x_star, y_star)[0, 1]

# Histogram bootstrapowych korelacji
plt.figure(figsize=(8, 5))
plt.hist(rho_boot, bins=40, density=True, edgecolor="black")
plt.axvline(rho_hat, color="red", linestyle="--",
            label=fr"$\hat{{\rho}}={rho_hat:.4f}$")
plt.xlabel("Bootstrapowe wartości korelacji")
plt.ylabel("Gęstość")
plt.title("Rozkład bootstrapowy estymatora korelacji Pearsona")
plt.legend()
plt.tight_layout()
plt.show()

# ============================================================
# c) Błąd standardowy i obciążenie
# ============================================================

bootstrap_se = np.std(rho_boot, ddof=1)
bootstrap_bias = np.mean(rho_boot) - rho_hat

print("=" * 60)
print(f"Bootstrapowy błąd standardowy = {bootstrap_se:.6f}")
print(f"Bootstrapowe obciążenie       = {bootstrap_bias:.6f}")

# ============================================================
# d) 95% bootstrapowy przedział ufności (percentylowy)
# ============================================================

ci_lower = np.percentile(rho_boot, 2.5)
ci_upper = np.percentile(rho_boot, 97.5)

print("=" * 60)
print("95% bootstrapowy przedział ufności (percentylowy):")
print(f"({ci_lower:.6f}, {ci_upper:.6f})")
print(f"Szerokość przedziału = {ci_upper - ci_lower:.6f}")

# ============================================================
# Badanie wpływu rozmiaru próby na szerokość przedziału
# ============================================================

sample_sizes = [25, 50, 100, 200, 300, n]
widths = []

for n_sub in sample_sizes:

    rho_boot_sub = np.empty(B)

    for b in range(B):

        # losujemy podpróbę bez zwracania z pełnego zbioru
        subset_idx = rng.choice(n, size=n_sub, replace=False)

        x_sub = x[subset_idx]
        y_sub = y[subset_idx]

        # bootstrap dla tej podpróby
        boot_idx = rng.integers(0, n_sub, size=n_sub)

        x_star = x_sub[boot_idx]
        y_star = y_sub[boot_idx]

        rho_boot_sub[b] = np.corrcoef(x_star, y_star)[0, 1]

    ci_l = np.percentile(rho_boot_sub, 2.5)
    ci_u = np.percentile(rho_boot_sub, 97.5)

    width = ci_u - ci_l
    widths.append(width)

    print(
        f"n = {n_sub:4d} | "
        f"CI = ({ci_l:.4f}, {ci_u:.4f}) | "
        f"szerokość = {width:.4f}"
    )

# Wykres szerokości przedziału ufności
plt.figure(figsize=(8, 5))
plt.plot(sample_sizes, widths, marker="o")
plt.xlabel("Rozmiar próby n")
plt.ylabel("Szerokość 95% CI")
plt.title("Wpływ rozmiaru próby na szerokość bootstrapowego CI")
plt.grid(True)
plt.tight_layout()
plt.show()