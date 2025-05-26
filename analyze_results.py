"""
Анализ и визуализация результатов эксперимента по сортировке строк
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from pathlib import Path
import argparse


def load_data(csv_file="experiment_results.csv"):
    """Загрузка данных из CSV файла"""
    try:
        df = pd.read_csv(csv_file)
        print(f"Загружено {len(df)} записей из {csv_file}")
        return df
    except FileNotFoundError:
        print(f"Файл {csv_file} не найден. Запустите сначала эксперимент на C++.")
        return None


def create_plots(df):
    """Создание графиков для анализа результатов"""

    plt.style.use("seaborn-v0_8")
    sns.set_palette("husl")

    fig, axes = plt.subplots(2, 3, figsize=(18, 12))
    fig.suptitle("Анализ производительности алгоритмов сортировки строк", fontsize=16)

    random_data = df[df["ArrayType"] == "Random"]

    for algo in random_data["Algorithm"].unique():
        algo_data = random_data[random_data["Algorithm"] == algo]
        axes[0, 0].plot(
            algo_data["ArraySize"],
            algo_data["ExecutionTime_ms"],
            marker="o",
            label=algo,
            linewidth=2,
        )

    axes[0, 0].set_xlabel("Размер массива")
    axes[0, 0].set_ylabel("Время выполнения (мс)")
    axes[0, 0].set_title("Время выполнения (случайные данные)")
    axes[0, 0].legend()
    axes[0, 0].grid(True, alpha=0.3)

    for algo in random_data["Algorithm"].unique():
        algo_data = random_data[random_data["Algorithm"] == algo]
        axes[0, 1].plot(
            algo_data["ArraySize"],
            algo_data["CharacterComparisons"],
            marker="s",
            label=algo,
            linewidth=2,
        )

    axes[0, 1].set_xlabel("Размер массива")
    axes[0, 1].set_ylabel("Количество сравнений")
    axes[0, 1].set_title("Посимвольные сравнения (случайные данные)")
    axes[0, 1].legend()
    axes[0, 1].grid(True, alpha=0.3)
    axes[0, 1].set_yscale("log")

    size_1000 = df[df["ArraySize"] == 1000]
    if not size_1000.empty:
        pivot_time = size_1000.pivot(
            index="Algorithm", columns="ArrayType", values="ExecutionTime_ms"
        )
        sns.heatmap(pivot_time, annot=True, fmt=".2f", cmap="YlOrRd", ax=axes[0, 2])
        axes[0, 2].set_title("Время выполнения по типам данных (размер 1000)")

    standard_algos = ["QuickSort", "MergeSort"]
    adaptive_algos = [
        "StringQuickSort",
        "StringMergeSort",
        "MSDRadixSort",
        "MSDRadixSortWithQuickSort",
    ]

    ratios = []
    sizes = []
    algo_names = []

    for size in sorted(df["ArraySize"].unique()):
        size_data = df[(df["ArraySize"] == size) & (df["ArrayType"] == "Random")]

        if len(size_data) > 0:
            quicksort_time = size_data[size_data["Algorithm"] == "QuickSort"][
                "ExecutionTime_ms"
            ].iloc[0]
            mergesort_time = size_data[size_data["Algorithm"] == "MergeSort"][
                "ExecutionTime_ms"
            ].iloc[0]

            for algo in adaptive_algos:
                algo_data = size_data[size_data["Algorithm"] == algo]
                if not algo_data.empty:
                    algo_time = algo_data["ExecutionTime_ms"].iloc[0]
                    if "Quick" in algo:
                        ratio = algo_time / quicksort_time
                    else:
                        ratio = algo_time / mergesort_time

                    ratios.append(ratio)
                    sizes.append(size)
                    algo_names.append(algo)

    if ratios:
        ratio_df = pd.DataFrame(
            {"Size": sizes, "Algorithm": algo_names, "Ratio": ratios}
        )

        for algo in adaptive_algos:
            algo_ratios = ratio_df[ratio_df["Algorithm"] == algo]
            if not algo_ratios.empty:
                axes[1, 0].plot(
                    algo_ratios["Size"],
                    algo_ratios["Ratio"],
                    marker="D",
                    label=algo,
                    linewidth=2,
                )

        axes[1, 0].axhline(
            y=1,
            color="red",
            linestyle="--",
            alpha=0.7,
            label="Равная производительность",
        )
        axes[1, 0].set_xlabel("Размер массива")
        axes[1, 0].set_ylabel("Отношение времени (адаптированный/стандартный)")
        axes[1, 0].set_title("Эффективность адаптированных алгоритмов")
        axes[1, 0].legend()
        axes[1, 0].grid(True, alpha=0.3)

    random_1500 = df[(df["ArraySize"] == 1500) & (df["ArrayType"] == "Random")]
    if not random_1500.empty:
        sns.boxplot(
            data=random_1500, x="Algorithm", y="ExecutionTime_ms", ax=axes[1, 1]
        )
        axes[1, 1].set_xticklabels(axes[1, 1].get_xticklabels(), rotation=45)
        axes[1, 1].set_title("Распределение времени (размер 1500, случайные данные)")

    type_comparison = (
        df.groupby(["Algorithm", "ArrayType"])["ExecutionTime_ms"].mean().reset_index()
    )
    pivot_comparison = type_comparison.pivot(
        index="Algorithm", columns="ArrayType", values="ExecutionTime_ms"
    )

    normalized_pivot = pivot_comparison.div(pivot_comparison.min(axis=1), axis=0)

    sns.heatmap(normalized_pivot, annot=True, fmt=".2f", cmap="RdYlBu_r", ax=axes[1, 2])
    axes[1, 2].set_title("Относительная производительность по типам данных")

    plt.tight_layout()
    plt.savefig("sorting_analysis.png", dpi=300, bbox_inches="tight")
    plt.show()


def generate_summary_report(df):
    """Генерация сводного отчета"""
    print("\n" + "=" * 60)
    print("СВОДНЫЙ ОТЧЕТ ПО РЕЗУЛЬТАТАМ ЭКСПЕРИМЕНТА")
    print("=" * 60)

    print(f"Количество тестов: {len(df)}")
    print(f"Алгоритмы: {', '.join(df['Algorithm'].unique())}")
    print(f"Типы данных: {', '.join(df['ArrayType'].unique())}")
    print(f"Диапазон размеров: {df['ArraySize'].min()} - {df['ArraySize'].max()}")

    print("\nСРЕДНЯЯ ПРОИЗВОДИТЕЛЬНОСТЬ ПО АЛГОРИТМАМ:")
    avg_performance = (
        df.groupby("Algorithm")
        .agg({"ExecutionTime_ms": "mean", "CharacterComparisons": "mean"})
        .round(2)
    )
    print(avg_performance)

    print("\nЛУЧШИЕ АЛГОРИТМЫ ПО ТИПАМ ДАННЫХ (среднее время):")
    for data_type in df["ArrayType"].unique():
        type_data = df[df["ArrayType"] == data_type]
        best_algo = type_data.groupby("Algorithm")["ExecutionTime_ms"].mean().idxmin()
        best_time = type_data.groupby("Algorithm")["ExecutionTime_ms"].mean().min()
        print(f"{data_type}: {best_algo} ({best_time:.2f} мс)")

    print("\nАНАЛИЗ МАСШТАБИРУЕМОСТИ:")
    for algo in df["Algorithm"].unique():
        algo_data = df[(df["Algorithm"] == algo) & (df["ArrayType"] == "Random")]
        if len(algo_data) >= 2:
            min_size_data = algo_data[
                algo_data["ArraySize"] == algo_data["ArraySize"].min()
            ]
            max_size_data = algo_data[
                algo_data["ArraySize"] == algo_data["ArraySize"].max()
            ]

            if not min_size_data.empty and not max_size_data.empty:
                min_time = min_size_data["ExecutionTime_ms"].iloc[0]
                max_time = max_size_data["ExecutionTime_ms"].iloc[0]
                min_size = min_size_data["ArraySize"].iloc[0]
                max_size = max_size_data["ArraySize"].iloc[0]

                scale_factor = (max_time / min_time) / (max_size / min_size)
                print(f"{algo}: коэффициент масштабирования {scale_factor:.2f}")


def main():
    parser = argparse.ArgumentParser(description="Анализ результатов сортировки строк")
    parser.add_argument(
        "--csv",
        default="experiment_results.csv",
        help="Путь к CSV файлу с результатами",
    )
    parser.add_argument("--no-plots", action="store_true", help="Не создавать графики")

    args = parser.parse_args()

    df = load_data(args.csv)
    if df is None:
        return

    required_columns = [
        "Algorithm",
        "ArraySize",
        "ArrayType",
        "ExecutionTime_ms",
        "CharacterComparisons",
    ]
    if not all(col in df.columns for col in required_columns):
        print(f"Ошибка: отсутствуют необходимые столбцы: {required_columns}")
        return

    generate_summary_report(df)

    if not args.no_plots:
        try:
            create_plots(df)
            print("\nГрафики сохранены в файл 'sorting_analysis.png'")
        except Exception as e:
            print(f"Ошибка при создании графиков: {e}")


if __name__ == "__main__":
    main()
