import pandas as pd
import matplotlib.pyplot as plt

GROUPS = [3, 5, 7, 9, 19, 21]


def clean(df):
    df["comparisons"] = pd.to_numeric(
        df["comparisons"],
        errors="coerce"
    )

    df["time"] = pd.to_numeric(
        df["time"],
        errors="coerce"
    )

    df["group"] = pd.to_numeric(
        df["group"],
        errors="coerce"
    )

    df["n"] = pd.to_numeric(
        df["n"],
        errors="coerce"
    )

    df = df.dropna()

    return df


def plot_metric(df, metric, title, ylabel):

    plt.figure(figsize=(12, 7))

    for g in GROUPS:

        sub = df[df["group"] == g]

        # grupowanie po n → średnia z prób
        agg = (
            sub.groupby("n")[metric]
            .mean()
            .reset_index()
        )

        # lekkie wygładzenie
        agg[metric] = (
            agg[metric]
            .rolling(window=5, center=True)
            .mean()
        )

        plt.plot(
            agg["n"],
            agg[metric],
            label=f"group={g}",
            linewidth=2
        )

    plt.title(title)

    plt.xlabel("n (rozmiar danych)")

    plt.ylabel(ylabel)

    plt.grid(True)

    plt.legend()

    plt.tight_layout()

    plt.show()


def plot_avg_comparisons_vs_group(df):

    plt.figure(figsize=(10, 6))

    agg = (
        df.groupby("group")["comparisons"]
        .mean()
        .reset_index()
        .sort_values("group")
    )

    plt.plot(
        agg["group"],
        agg["comparisons"],
        marker="o",
        linewidth=2
    )

    plt.title(
        "Średnia liczba porównań vs rozmiar grupy"
    )

    plt.xlabel("Rozmiar grupy")

    plt.ylabel("Avg comparisons")

    plt.grid(True)

    plt.tight_layout()

    plt.show()


def plot_avg_time_vs_group(df):

    plt.figure(figsize=(10, 6))

    agg = (
        df.groupby("group")["time"]
        .mean()
        .reset_index()
        .sort_values("group")
    )

    plt.plot(
        agg["group"],
        agg["time"],
        marker="o",
        linewidth=2
    )

    plt.title(
        "Średni czas działania vs rozmiar grupy"
    )

    plt.xlabel("Rozmiar grupy")

    plt.ylabel("Avg time [s]")

    plt.grid(True)

    plt.tight_layout()

    plt.show()


def main():

    df = pd.read_csv("group_results.csv")

    df = clean(df)

    # DEBUG INFO
    print(df.head())

    print("rows:", len(df))

    # 1. comparisons vs n
    plot_metric(
        df,
        "comparisons",
        "SELECT: liczba porównań vs n (różne rozmiary grup)",
        "comparisons"
    )

    # 2. time vs n
    plot_metric(
        df,
        "time",
        "SELECT: czas działania vs n (różne rozmiary grup)",
        "time [s]"
    )

    # 3. avg comparisons vs group size
    plot_avg_comparisons_vs_group(df)

    # 4. avg time vs group size
    plot_avg_time_vs_group(df)


if __name__ == "__main__":
    main()