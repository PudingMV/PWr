import numpy as np
import matplotlib.pyplot as plt
from sklearn.neighbors import KNeighborsClassifier

np.random.seed(42)

n = 1000
m = 500
dimensions = [1, 2, 5, 10, 20, 50, 100]

def run_experiment(d):
    #TRAIN
    X_train = np.random.randn(n, d)
    norms_train = np.linalg.norm(X_train, axis=1)
    rd = np.median(norms_train)

    y_train = (norms_train <= rd).astype(int)

    #TEST
    X_test = np.random.randn(m, d)
    norms_test = np.linalg.norm(X_test, axis=1)
    y_test = (norms_test <= rd).astype(int)

    #KNN
    model = KNeighborsClassifier(n_neighbors=1)
    model.fit(X_train, y_train)

    y_pred = model.predict(X_test)
    acc = np.mean(y_pred == y_test)

    return acc

accuracies = []

for d in dimensions:
    acc = run_experiment(d)
    accuracies.append(acc)
    print(f"d={d}, accuracy={acc:.4f}")

# wykres
plt.figure(figsize=(8,5))
plt.plot(dimensions, accuracies, marker='o')
plt.xscale('log')
plt.ylim(0, 1)
plt.axhline(0.5, linestyle='--', color='red', label='random classifier')
plt.xlabel("Wymiar d (log scale)")
plt.ylabel("Accuracy KNN (K=1)")
plt.title("Degradacja KNN w wysokich wymiarach")
plt.grid(True)
plt.legend()
plt.savefig("knn_degrade.png", dpi=200)
print("Zapisano wykres jako knn_degrade.png")