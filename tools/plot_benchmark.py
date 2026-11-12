#!/usr/bin/env python3
"""Build comparison charts from benchmark_results.csv.

Usage:
    python tools/plot_benchmark.py benchmark_results.csv
Requires: matplotlib (python -m pip install matplotlib)
"""
import csv
import sys
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt


def main():
    csv_path = Path(sys.argv[1] if len(sys.argv) > 1 else "benchmark_results.csv")
    if not csv_path.is_file():
        raise SystemExit(f"CSV fayli topilmadi: {csv_path}")

    rows = []
    with csv_path.open("r", newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle):
            if row["result"] == "skipped_large_input":
                continue
            try:
                median = float(row["median_us"])
            except (KeyError, ValueError):
                continue
            rows.append((int(row["number"]), row["algorithm"], median))

    if not rows:
        raise SystemExit("Diagramma uchun yaroqli benchmark qatori topilmadi.")

    grouped = defaultdict(list)
    for number, algorithm, median in rows:
        grouped[algorithm].append((number, median))

    output = csv_path.parent / "benchmark_charts"
    output.mkdir(exist_ok=True)

    for algorithm, points in grouped.items():
        points.sort()
        x = [str(number) for number, _ in points]
        y = [median for _, median in points]
        plt.figure(figsize=(10, 5))
        plt.bar(x, y)
        plt.title(f"Median bajarilish vaqti: {algorithm}")
        plt.xlabel("Tekshirilgan son")
        plt.ylabel("Vaqt (mikrosekund)")
        plt.xticks(rotation=35, ha="right")
        plt.tight_layout()
        safe_name = "".join(ch if ch.isalnum() or ch in "-_" else "_" for ch in algorithm)
        plt.savefig(output / f"{safe_name}.png", dpi=160)
        plt.close()

    print(f"{len(grouped)} ta diagramma saqlandi: {output}")


if __name__ == "__main__":
    main()
