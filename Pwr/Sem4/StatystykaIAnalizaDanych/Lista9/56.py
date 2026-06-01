import numpy as np
import matplotlib
matplotlib.use("QtAgg")

import matplotlib.pyplot as plt
import statsmodels.api as sm

rng = np.random.default_rng(10)

x1 = rng.uniform(0, 1, size=100)
x2 = 0.5 * x1 + rng.normal(size=100) / 10
y = 2 + 2 * x1 + 0.3 * x2 + rng.normal(size=100)

print("Model teoretyczny:")
print("y = 2 + 2*x1 + 0.3*x2 + ε\n")

corr = np.corrcoef(x1, x2)[0, 1]
print(f"Korelacja x1 i x2: {corr:.4f}")

plt.figure()
plt.scatter(x1, x2)
plt.xlabel("x1")
plt.ylabel("x2")
plt.title("Relacja między x1 i x2")
plt.show()

def run_ols(X, y, label):
    X = sm.add_constant(X)
    model = sm.OLS(y, X).fit()
    print(label)
    print(model.summary())
    return model

X_full = np.column_stack([x1, x2])
model_full = run_ols(X_full, y, "Model: y ~ x1 + x2")

model_x1 = run_ols(x1, y, "Model: y ~ x1")

model_x2 = run_ols(x2, y, "Model: y ~ x2")

x1_new = np.concatenate([x1, [0.1]])
x2_new = np.concatenate([x2, [0.8]])
y_new  = np.concatenate([y,  [6]])

print("\nDodano obserwację odstającą: (0.1, 0.8, 6)\n")

X_full_new = np.column_stack([x1_new, x2_new])

model_full_new = run_ols(X_full_new, y_new, "NOWY model: y ~ x1 + x2")
model_x1_new = run_ols(x1_new, y_new, "NOWY model: y ~ x1")
model_x2_new = run_ols(x2_new, y_new, "NOWY model: y ~ x2")

influence = model_full_new.get_influence()
leverage = influence.hat_matrix_diag
cooks_d = influence.cooks_distance[0]

print("\nNajwiększe leverage:")
print(np.argsort(leverage)[-5:])

print("\nNajwiększe Cook's distance:")
print(np.argsort(cooks_d)[-5:])

plt.figure()
plt.stem(cooks_d)
plt.title("Cook's distance")
plt.xlabel("Observation")
plt.ylabel("Influence")
plt.show()