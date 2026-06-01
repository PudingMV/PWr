import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
import statsmodels.api as sm
import statsmodels.formula.api as smf

df = pd.read_csv("Auto.csv")

df = df.replace("?", np.nan)
df = df.dropna()

df["horsepower"] = df["horsepower"].astype(float)
df["origin"] = df["origin"].astype("category")

train_df, test_df = train_test_split(df, test_size=0.3, random_state=42)

# a)
model_a = smf.ols("mpg ~ horsepower", data=train_df).fit()
print("\nMODEL A")
print(model_a.summary())

# MSE
pred_a = model_a.predict(test_df)
mse_a = np.mean((test_df["mpg"] - pred_a) ** 2)
print("MSE model a:", mse_a)


# b)
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

print("\nMODEL B1")
print(model_b1.summary())

print("\nMODEL B2")
print(model_b2.summary())

print("\nMODEL B3")
print(model_b3.summary())

print("\nMODEL B4")
print(model_b4.summary())

# c)
def mse(model, test_data):
    pred = model.predict(test_data)
    return np.mean((test_data["mpg"] - pred) ** 2)

mse_b1 = mse(model_b1, test_df)
mse_b2 = mse(model_b2, test_df)
mse_b3 = mse(model_b3, test_df)
mse_b4 = mse(model_b4, test_df)

print("\nMSE COMPARISON")
print("MSE b1:", mse_b1)
print("MSE b2:", mse_b2)
print("MSE b3:", mse_b3)
print("MSE b4:", mse_b4)


mse_dict = {
    "b1": mse_b1,
    "b2": mse_b2,
    "b3": mse_b3,
    "b4": mse_b4
}

best_model = min(mse_dict, key=mse_dict.get)

print("\nBEST MODEL")
print("Najlepszy model:", best_model)