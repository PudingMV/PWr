import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
import statsmodels.formula.api as smf

# =========================
# 1. Wczytanie danych
# =========================
df = pd.read_csv("Auto.csv")

df = df.replace("?", np.nan)
df = df.dropna()

df["horsepower"] = df["horsepower"].astype(float)
df["origin"] = df["origin"].astype("category")
df["year"] = df["year"].astype(int)

# =========================
# 2. Train / test split
# =========================
train_df, test_df = train_test_split(df, test_size=0.3, random_state=42)


# ==========================================================
# a) Prosty model: mpg ~ horsepower
# ==========================================================
model_a = smf.ols("mpg ~ horsepower", data=train_df).fit()

print("\n===== MODEL A =====")
print(model_a.summary())

pred_a = model_a.predict(test_df)
mse_a = np.mean((test_df["mpg"] - pred_a) ** 2)
print("MSE model a:", mse_a)


# ==========================================================
# b) Modele wielowymiarowe (różne formy)
# ==========================================================

model_b1 = smf.ols(
    "mpg ~ horsepower + weight + displacement + cylinders",
    data=train_df
).fit()

model_b2 = smf.ols(
    "mpg ~ horsepower * weight + cylinders",
    data=train_df
).fit()

model_b3 = smf.ols(
    "mpg ~ I(np.log(horsepower)) + I(np.sqrt(weight)) + cylinders",
    data=train_df
).fit()

model_b4 = smf.ols(
    "mpg ~ horsepower + weight + C(origin)",
    data=train_df
).fit()

print("\n===== MODEL B1 =====")
print(model_b1.summary())

print("\n===== MODEL B2 =====")
print(model_b2.summary())

print("\n===== MODEL B3 =====")
print(model_b3.summary())

print("\n===== MODEL B4 =====")
print(model_b4.summary())


# ==========================================================
# c) MODELE Z YEAR (NOWE ZADANIE)
# ==========================================================

# MODEL 1: year jako zmienna liczbowa (trend liniowy)
model_year_num = smf.ols(
    "mpg ~ horsepower + weight + year",
    data=train_df
).fit()

# MODEL 2: year jako zmienna kategoryczna (oddzielny efekt dla każdego roku)
model_year_cat = smf.ols(
    "mpg ~ horsepower + weight + C(year)",
    data=train_df
).fit()

print("\n===== MODEL YEAR (NUMERIC) =====")
print(model_year_num.summary())

print("\n===== MODEL YEAR (CATEGORICAL) =====")
print(model_year_cat.summary())


# ==========================================================
# d) MSE – porównanie wszystkich modeli
# ==========================================================
def mse(model, test_data):
    pred = model.predict(test_data)
    return np.mean((test_data["mpg"] - pred) ** 2)

models = {
    "b1": model_b1,
    "b2": model_b2,
    "b3": model_b3,
    "b4": model_b4,
    "year_numeric": model_year_num,
    "year_categorical": model_year_cat
}

print("\n===== MSE COMPARISON =====")
mse_results = {}

for name, model in models.items():
    mse_results[name] = mse(model, test_df)
    print(name, ":", mse_results[name])

best_model = min(mse_results, key=mse_results.get)

print("\n===== BEST MODEL =====")
print("Najlepszy model:", best_model)