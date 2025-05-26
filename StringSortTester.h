#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <string>
#include <vector>
#include <chrono>
#include <functional>

struct SortResult
{
    double execution_time_ms;
    long long character_comparisons;
    bool is_sorted;
};

class StringSortTester
{
private:
    static long long comparison_count;

    // Функция для сравнения строк с подсчетом операций
    static int compareStrings(const std::string &a, const std::string &b);

    // Проверка отсортированности массива
    static bool isSorted(const std::vector<std::string> &arr);

    // Стандартные алгоритмы сортировки
    static void quickSort(std::vector<std::string> &arr, int low, int high);
    static int partition(std::vector<std::string> &arr, int low, int high);
    static void mergeSort(std::vector<std::string> &arr, int left, int right);
    static void merge(std::vector<std::string> &arr, int left, int mid, int right);

    // Специализированные алгоритмы сортировки строк
    static void stringQuickSort(std::vector<std::string> &arr, int low, int high, int depth = 0);
    static void stringMergeSort(std::vector<std::string> &arr, int left, int right, int depth = 0);
    static void msdRadixSort(std::vector<std::string> &arr, int low, int high, int depth = 0);
    static void msdRadixSortWithQuickSort(std::vector<std::string> &arr, int low, int high, int depth = 0);

    // Вспомогательные функции для специализированных алгоритмов
    static int charAt(const std::string &s, int index);
    static int getLCP(const std::string &a, const std::string &b, int depth = 0);
    static void stringMergeWithLCP(std::vector<std::string> &arr, int left, int mid, int right, int depth);

    // Тернарная быстрая сортировка строк
    static void ternaryStringQuickSort(std::vector<std::string> &arr, int low, int high, int depth = 0);

public:
    // Константы для размеров массивов
    static const int MIN_ARRAY_SIZE = 100;
    static const int MAX_ARRAY_SIZE = 3000;
    static const int STEP_SIZE = 100;
    static const int ALPHABET_SIZE = 74;

    // Методы тестирования стандартных алгоритмов
    static SortResult testQuickSort(std::vector<std::string> arr);
    static SortResult testMergeSort(std::vector<std::string> arr);

    // Методы тестирования специализированных алгоритмов
    static SortResult testStringQuickSort(std::vector<std::string> arr);
    static SortResult testStringMergeSort(std::vector<std::string> arr);
    static SortResult testMSDRadixSort(std::vector<std::string> arr);
    static SortResult testMSDRadixSortWithQuickSort(std::vector<std::string> arr);

    // Общий метод для измерения производительности
    static SortResult measurePerformance(
        std::vector<std::string> arr,
        std::function<void(std::vector<std::string> &)> sortFunction);

    // Метод для множественных замеров с усреднением
    static SortResult averageResults(
        std::vector<std::string> arr,
        std::function<void(std::vector<std::string> &)> sortFunction,
        int num_runs = 5);

    // Утилиты для вывода результатов
    static void printResult(const std::string &algorithm_name, const SortResult &result);
    static void printCSVHeader();
    static void printCSVResult(const std::string &algorithm_name, int array_size,
                               const std::string &array_type, const SortResult &result);
};

#endif // STRING_SORT_TESTER_H
