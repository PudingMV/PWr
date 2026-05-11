import pandas as pd
import matplotlib.pyplot as plt

GROUPS = [3, 5, 7, 9, 19, 21]


def clean(df):

    numeric_cols = [
        "group",
        "n",
        "comparisons",
        "swaps",
        "time"
    ]

    for col in numeric_cols:
        df[col] = pd.to_numeric(
            df[col],
            errors="coerce"
        )

    df = df.dropna()

    return df


def plot_metric(df, metric, title, ylabel):

    plt.figure(figsize=(12, 7))

    for g in GROUPS:

        sub = df[df["group"] == g]

        # średnia po próbach
        agg = (
            sub.groupby("n")[metric]
            .mean()
            .reset_index()
        )

        # wygładzenie
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


def plot_avg_metric_vs_group(df,
                             metric,
                             title,
                             ylabel):

    plt.figure(figsize=(10, 6))

    agg = (
        df.groupby("group")[metric]
        .mean()
        .reset_index()
        .sort_values("group")
    )

    plt.plot(
        agg["group"],
        agg[metric],
        marker="o",
        linewidth=2
    )

    plt.title(title)

    plt.xlabel("Rozmiar grupy")

    plt.ylabel(ylabel)

    plt.grid(True)

    plt.tight_layout()

    plt.show()


def main():

    df = pd.read_csv("group_results.csv")

    df = clean(df)

    # DEBUG
    print(df.head())

    print("rows:", len(df))

    # 1. comparisons vs n
    plot_metric(
        df,
        "comparisons",
        "SELECT: liczba porównań vs n",
        "comparisons"
    )

    # 2. swaps vs n
    plot_metric(
        df,
        "swaps",
        "SELECT: liczba przestawień vs n",
        "swaps"
    )

    # 3. time vs n
    plot_metric(
        df,
        "time",
        "SELECT: czas działania vs n",
        "time [us]"
    )

    # 4. avg comparisons vs group
    plot_avg_metric_vs_group(
        df,
        "comparisons",
        "Średnia liczba porównań vs rozmiar grupy",
        "avg comparisons"
    )

    # 5. avg swaps vs group
    plot_avg_metric_vs_group(
        df,
        "swaps",
        "Średnia liczba przestawień vs rozmiar grupy",
        "avg swaps"
    )

    # 6. avg time vs group
    plot_avg_metric_vs_group(
        df,
        "time",
        "Średni czas działania vs rozmiar grupy",
        "avg time [us]"
    )


if __name__ == "__main__":
    main()