import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("results.csv")

metrics = ["avgComp", "avgPointerOps", "avgHeight"]

for s in [1, 2]:
    part = df[df["scenario"] == s]

    for metric in metrics:
        plt.figure()

        plt.plot(part["n"], part[metric], marker="o")

        plt.title(f"Scenario {s} - {metric}")
        plt.xlabel("n")
        plt.ylabel(metric)

        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f"SplayTree_Scenario{s}_{metric}", dpi=300)
        plt.show()

max_metrics = ["maxComp", "maxPointerOps", "maxHeight"]

for s in [1, 2]:
    part = df[df["scenario"] == s].copy()

    if s == 1:
        normalization = part["n"]
        norm_label = "n"
    else:
        normalization = np.log2(part["n"])
        norm_label = "log₂(n)"

    for metric in max_metrics:
        plt.figure()

        normalized = part[metric] / normalization

        plt.plot(
            part["n"],
            normalized,
            marker="o",
            label=f"{metric}/{norm_label}"
        )

        mean_value = normalized.mean()
        plt.axhline(
            mean_value,
            linestyle="--",
            label=f"mean = {mean_value:.3f}"
        )

        ymin = normalized.min()
        ymax = normalized.max()

        if ymax > ymin:
            margin = (ymax - ymin) * 0.1
            plt.ylim(ymin - 100*margin, ymax + 100*margin)
        else:
            plt.ylim(ymin - 0.1, ymax + 0.1)

        plt.title(f"Scenario {s} - {metric}/{norm_label}")
        plt.xlabel("n")
        plt.ylabel(f"{metric}/{norm_label}")

        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        plt.savefig(f"SplayTree_Scenario{s}_{metric}_normalized_by_{norm_label}", dpi=300)
        plt.show()
