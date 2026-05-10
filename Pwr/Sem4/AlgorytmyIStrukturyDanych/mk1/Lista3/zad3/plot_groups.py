import pandas as pd
import matplotlib.pyplot as plt

GROUPS = [3, 5, 7, 9, 19, 21]

def clean(df):
    df["comparisons"] = pd.to_numeric(df["comparisons"], errors="coerce")
    df["time"] = pd.to_numeric(df["time"], errors="coerce")
    df = df.dropna()
    return df

def plot_metric(df, metric, title, ylabel):

    plt.figure(figsize=(12, 7))

    for g in GROUPS:
        sub = df[df["group"] == g]

        # grupowanie po n → średnia z 50 prób
        agg = sub.groupby("n")[metric].mean().reset_index()

        # lekkie wygładzenie (rolling mean)
        agg[metric] = agg[metric].rolling(window=5, center=True).mean()

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


def main():

    df = pd.read_csv("group_results.csv")
    df = clean(df)

    # DEBUG INFO
    print(df.head())
    print("rows:", len(df))

    # 1. comparisons
    plot_metric(
        df,
        "comparisons",
        "SELECT: liczba porównań vs n (różne rozmiary grup)",
        "comparisons"
    )

    # 2. time
    plot_metric(
        df,
        "time",
        "SELECT: czas działania vs n (różne rozmiary grup)",
        "time [s]"
    )


if __name__ == "__main__":
    main()