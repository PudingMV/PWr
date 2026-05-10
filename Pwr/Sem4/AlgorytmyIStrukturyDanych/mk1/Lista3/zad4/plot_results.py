import pandas as pd
import matplotlib.pyplot as plt

def smooth(series, window=5):
    return series.rolling(window=window, center=True).mean()

def plot_case(file, title):
    df = pd.read_csv(file)

    # zabezpieczenie typów
    df = df.dropna()
    df["n"] = df["n"].astype(int)

    # sortowanie (ważne!)
    df = df.sort_values("n")

    # smoothing (żeby nie było „szumu”)
    smart_s = smooth(df["smart"])
    binary_s = smooth(df["binary"])

    plt.figure(figsize=(10, 6))

    plt.plot(df["n"], df["smart"], alpha=0.25, label="SMART (raw)")
    plt.plot(df["n"], df["binary"], alpha=0.25, label="BINARY (raw)")

    plt.plot(df["n"], smart_s, label="SMART (smoothed)", linewidth=2)
    plt.plot(df["n"], binary_s, label="BINARY (smoothed)", linewidth=2)

    plt.title(title)
    plt.xlabel("n (rozmiar danych)")
    plt.ylabel("liczba porównań")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":

    plot_case(
        "case1_results.csv",
        "CASE 1: f(i) = (log2(i+1))^3"
    )

    plot_case(
        "case2_results.csv",
        "CASE 2: f(i) = sqrt(i) + log2(i+1)"
    )