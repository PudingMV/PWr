# ============================================
# Symulacja danych i regresja liniowa
# Zadanie ISLR — Linear Regression
# ============================================

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import statsmodels.api as sm
from statsmodels.formula.api import ols

# ------------------------------------------------
# Ustawienie generatora liczb losowych
# ------------------------------------------------
np.random.seed(1)

# =========================================================
# (a) Generowanie x ~ N(0,1)
# =========================================================
x = np.random.normal(loc=0, scale=1, size=100)

# =========================================================
# (b) Generowanie eps ~ N(0, 0.25)
# Variance = 0.25 => std = sqrt(0.25)=0.5
# =========================================================
eps = np.random.normal(loc=0, scale=np.sqrt(0.25), size=100)

# =========================================================
# (c) Generowanie y = 1 + 0.5x + eps
# =========================================================
beta0 = 1
beta1 = 0.5

y = beta0 + beta1 * x + eps

print("=" * 60)
print("(c)")
print("Długość wektora y:", len(y))
print(f"Prawdziwe beta0 = {beta0}")
print(f"Prawdziwe beta1 = {beta1}")

# =========================================================
# (d) Scatterplot
# =========================================================
plt.figure(figsize=(8, 6))
plt.scatter(x, y, alpha=0.7)
plt.xlabel("x")
plt.ylabel("y")
plt.title("Scatterplot: x vs y")
plt.grid(True)
plt.show()

print("\n(d)")
print("Wykres pokazuje dodatnią zależność liniową między x i y.")
print("Punkty są rozproszone wokół prostej regresji z powodu szumu eps.")

# =========================================================
# (e) Regresja liniowa y ~ x
# =========================================================
X = sm.add_constant(x)
model = sm.OLS(y, X).fit()

print("\n" + "=" * 60)
print("(e) Model liniowy")
print(model.summary())

beta0_hat = model.params[0]
beta1_hat = model.params[1]

print("\nPorównanie parametrów:")
print(f"beta0_hat = {beta0_hat:.4f} vs beta0 = {beta0}")
print(f"beta1_hat = {beta1_hat:.4f} vs beta1 = {beta1}")

# =========================================================
# (f) Dodanie linii regresji i prawdziwej linii
# =========================================================
x_sorted = np.sort(x)

# Predykcje modelu
y_pred = beta0_hat + beta1_hat * x_sorted

# Prawdziwa linia populacyjna
y_true = beta0 + beta1 * x_sorted

plt.figure(figsize=(8, 6))
plt.scatter(x, y, alpha=0.7, label="Dane")

plt.plot(x_sorted, y_pred,
         color="red",
         linewidth=2,
         label="Least Squares Line")

plt.plot(x_sorted, y_true,
         color="green",
         linewidth=2,
         linestyle="--",
         label="Population Regression Line")

plt.xlabel("x")
plt.ylabel("y")
plt.title("Regresja liniowa")
plt.legend()
plt.grid(True)
plt.show()

# =========================================================
# (g) Regresja wielomianowa: y ~ x + x^2
# =========================================================
df = pd.DataFrame({
    "y": y,
    "x": x,
    "x2": x**2
})

poly_model = ols("y ~ x + x2", data=df).fit()

print("\n" + "=" * 60)
print("(g) Model wielomianowy")
print(poly_model.summary())

print("\nInterpretacja:")
pval_quad = poly_model.pvalues["x2"]

if pval_quad < 0.05:
    print("Istnieją dowody, że składnik kwadratowy poprawia model.")
else:
    print("Brak istotnych dowodów, że składnik kwadratowy poprawia model.")

# =========================================================
# Funkcja pomocnicza dla (h), (i), (j)
# =========================================================
def simulate_and_fit(noise_variance, title):
    """
    Symulacja danych z różnym poziomem szumu
    """
    eps_new = np.random.normal(
        loc=0,
        scale=np.sqrt(noise_variance),
        size=100
    )

    y_new = beta0 + beta1 * x + eps_new

    X_new = sm.add_constant(x)
    model_new = sm.OLS(y_new, X_new).fit()

    beta0_hat_new = model_new.params[0]
    beta1_hat_new = model_new.params[1]

    # Confidence intervals
    ci = model_new.conf_int()

    print("\n" + "=" * 60)
    print(title)
    print("=" * 60)

    print(model_new.summary())

    print("\nConfidence intervals:")
    print(ci)

    # Wykres
    x_sorted = np.sort(x)

    y_pred_new = beta0_hat_new + beta1_hat_new * x_sorted
    y_true_new = beta0 + beta1 * x_sorted

    plt.figure(figsize=(8, 6))
    plt.scatter(x, y_new, alpha=0.7)

    plt.plot(
        x_sorted,
        y_pred_new,
        color="red",
        linewidth=2,
        label="Estimated Regression Line"
    )

    plt.plot(
        x_sorted,
        y_true_new,
        color="green",
        linestyle="--",
        linewidth=2,
        label="True Regression Line"
    )

    plt.xlabel("x")
    plt.ylabel("y")
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.show()

    return model_new


# =========================================================
# (h) Mniej szumu
# =========================================================
# variance = 0.05
less_noise_model = simulate_and_fit(
    noise_variance=0.05,
    title="(h) Less Noise"
)

print("\n(h) Wnioski:")
print("""
Przy mniejszym szumie:
- punkty są bliżej linii regresji,
- R² wzrasta,
- estymatory beta0 i beta1 są bliższe prawdziwym wartościom,
- confidence intervals są węższe.
""")

# =========================================================
# (i) Więcej szumu
# =========================================================
# variance = 1
more_noise_model = simulate_and_fit(
    noise_variance=1,
    title="(i) More Noise"
)

print("\n(i) Wnioski:")
print("""
Przy większym szumie:
- dane są bardziej rozproszone,
- R² maleje,
- estymatory parametrów są mniej dokładne,
- confidence intervals stają się szersze.
""")

# =========================================================
# (j) Confidence intervals
# =========================================================
print("\n" + "=" * 60)
print("(j) Confidence Intervals")
print("=" * 60)

print("\n--- Original data ---")
print(model.conf_int())

print("\n--- Less noisy data ---")
print(less_noise_model.conf_int())

print("\n--- More noisy data ---")
print(more_noise_model.conf_int())

print("""
Komentarz:
- Mniejszy szum => węższe przedziały ufności.
- Większy szum => szersze przedziały ufności.
- Wszystkie przedziały powinny zawierać prawdziwe wartości:
  beta0 = 1 oraz beta1 = 0.5.
""")
