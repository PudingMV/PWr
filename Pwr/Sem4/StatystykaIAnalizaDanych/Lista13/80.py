import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.compose import ColumnTransformer
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import OneHotEncoder
from sklearn.model_selection import GridSearchCV
from sklearn.ensemble import RandomForestClassifier

# ==========================================================
# Wczytanie danych
# ==========================================================

df = pd.read_csv("Credit.csv")

# usunięcie ewentualnej kolumny indeksowej
df = df.loc[:, ~df.columns.str.contains("^Unnamed")]

# ==========================================================
# Zadanie 73a
# Income > 50
# ==========================================================

df["Income_gt_50"] = (df["Income"] > 50).astype(int)

y = df["Income_gt_50"]

X = df.drop(columns=["Income", "Income_gt_50"])

# ==========================================================
# Zmienne numeryczne i kategoryczne
# ==========================================================

categorical_cols = X.select_dtypes(include=["object"]).columns
numeric_cols = X.select_dtypes(exclude=["object"]).columns

preprocessor = ColumnTransformer(
    transformers=[
        (
            "cat",
            OneHotEncoder(drop="first", handle_unknown="ignore"),
            categorical_cols
        ),
        (
            "num",
            "passthrough",
            numeric_cols
        )
    ]
)

# ==========================================================
# Random Forest
# ==========================================================

rf = RandomForestClassifier(
    random_state=42
)

pipeline = Pipeline(
    [
        ("prep", preprocessor),
        ("model", rf)
    ]
)

# ==========================================================
# Grid Search
# ==========================================================

param_grid = {
    "model__n_estimators": [100, 200, 500],
    "model__max_depth": [None, 5, 10, 20],
    "model__max_features": ["sqrt", "log2", 0.5],
    "model__min_samples_split": [2, 5, 10],
    "model__min_samples_leaf": [1, 2, 4]
}

grid = GridSearchCV(
    pipeline,
    param_grid,
    cv=5,
    scoring="accuracy",
    n_jobs=-1,
    verbose=1
)

grid.fit(X, y)

# ==========================================================
# Wyniki
# ==========================================================

print("\n" + "="*60)
print("NAJLEPSZE PARAMETRY")
print("="*60)

print(grid.best_params_)

print("\nNajlepsza dokładność CV:")
print(f"{grid.best_score_:.4f}")

# ==========================================================
# Ważności cech
# ==========================================================

best_model = grid.best_estimator_

feature_names = (
    best_model.named_steps["prep"]
    .get_feature_names_out()
)

importances = (
    best_model.named_steps["model"]
    .feature_importances_
)

importance_df = pd.DataFrame(
    {
        "Feature": feature_names,
        "Importance": importances
    }
)

importance_df = importance_df.sort_values(
    "Importance",
    ascending=False
)

print("\n" + "="*60)
print("TOP 15 CECH")
print("="*60)

print(importance_df.head(15))

# ==========================================================
# Wykres ważności
# ==========================================================

top_k = 15

plt.figure(figsize=(10, 6))

plt.barh(
    importance_df["Feature"].head(top_k)[::-1],
    importance_df["Importance"].head(top_k)[::-1]
)

plt.xlabel("MDI Importance")
plt.title("15 najważniejszych cech (Random Forest)")
plt.tight_layout()
plt.show()