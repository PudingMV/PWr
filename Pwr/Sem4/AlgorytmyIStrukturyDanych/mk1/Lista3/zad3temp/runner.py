import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

df = df.groupby(["group", "n"], as_index=False).mean()
df = df.sort_values(by=["group", "n"])

groups = sorted(df["group"].unique())

metrics = ["comparisons", "swaps", "rec_calls", "time_ms"]

titles = {
    "comparisons": "Liczba porównań",
    "swaps": "Liczba swapów",
    "rec_calls": "Liczba wywołań rekurencji",
    "time_ms": "Czas wykonania [ms]"
}

for metric in metrics:
    plt.figure()

    for g in groups:
        data = df[df["group"] == g]
        plt.plot(data["n"], data[metric], label=f"k={g}")

    plt.xlabel("n")
    plt.ylabel(metric)
    plt.title(titles[metric])
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()