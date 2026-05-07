import numpy as np
import matplotlib.pyplot as plt

from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import cross_val_score

np.random.seed(69)

n = 200
X = np.random.rand(n, 1)
noise = np.random.normal(0, 0.3, size=n)
y = np.sin(2 * np.pi * X[:, 0]) + noise

degrees = range(1, 101)

cv_mse = []
train_mse = []

for d in degrees:
    model = make_pipeline(
        PolynomialFeatures(d),
        LinearRegression()
    )
    
    scores = cross_val_score(
        model,
        X,
        y,
        cv=5,
        scoring="neg_mean_squared_error"
    )
    
    mse_cv = -np.mean(scores)
    cv_mse.append(mse_cv)
    
    model.fit(X, y)
    y_pred = model.predict(X)
    mse_train = np.mean((y - y_pred) ** 2)
    train_mse.append(mse_train)

best_d = degrees[np.argmin(cv_mse)]
print("Optimal degree (CV):", best_d)

plt.figure(figsize=(10,6))
plt.plot(degrees, cv_mse, label="CV MSE")
plt.plot(degrees, train_mse, label="Training MSE")

plt.axvline(best_d, linestyle="--", color="red", label=f"best d={best_d}")

plt.xlabel("Degree d")
plt.ylabel("MSE")
plt.title("Polynomial regression: bias-variance tradeoff")
plt.legend()
plt.grid(True)

plt.savefig("poly_cv.png", dpi=200)
print("Saved plot -> poly_cv.png")