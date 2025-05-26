#include "StringSortTester.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

long long StringSortTester::comparison_count = 0;

int StringSortTester::compareStrings(const std::string &a, const std::string &b)
{
    size_t min_len = std::min(a.length(), b.length());

    for (size_t i = 0; i < min_len; ++i)
    {
        comparison_count++;
        if (a[i] < b[i])
            return -1;
        if (a[i] > b[i])
            return 1;
    }

    if (a.length() < b.length())
        return -1;
    if (a.length() > b.length())
        return 1;
    return 0;
}

bool StringSortTester::isSorted(const std::vector<std::string> &arr)
{
    for (size_t i = 1; i < arr.size(); ++i)
    {
        if (arr[i - 1] > arr[i])
        {
            return false;
        }
    }
    return true;
}

// Стандартная быстрая сортировка
void StringSortTester::quickSort(std::vector<std::string> &arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int StringSortTester::partition(std::vector<std::string> &arr, int low, int high)
{
    std::string pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        if (compareStrings(arr[j], pivot) <= 0)
        {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Стандартная сортировка слиянием
void StringSortTester::mergeSort(std::vector<std::string> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void StringSortTester::merge(std::vector<std::string> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::string> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (compareStrings(L[i], R[j]) <= 0)
        {
            arr[k] = L[i];
            ++i;
        }
        else
        {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Вспомогательная функция для получения символа на позиции
int StringSortTester::charAt(const std::string &s, int index)
{
    if (index >= static_cast<int>(s.length()))
    {
        return -1; // Конец строки
    }
    return static_cast<unsigned char>(s[index]);
}

// Получение длины наибольшего общего префикса
int StringSortTester::getLCP(const std::string &a, const std::string &b, int depth)
{
    int lcp = 0;
    size_t min_len = std::min(a.length(), b.length());

    for (size_t i = depth; i < min_len; ++i)
    {
        comparison_count++;
        if (a[i] != b[i])
            break;
        lcp++;
    }

    return lcp;
}

// Тернарная быстрая сортировка строк
void StringSortTester::ternaryStringQuickSort(std::vector<std::string> &arr, int low, int high, int depth)
{
    if (high <= low)
        return;

    int lt = low, gt = high;
    int pivot = charAt(arr[low], depth);
    int i = low + 1;

    while (i <= gt)
    {
        int ch = charAt(arr[i], depth);
        comparison_count++;

        if (ch < pivot)
        {
            std::swap(arr[lt++], arr[i++]);
        }
        else if (ch > pivot)
        {
            std::swap(arr[i], arr[gt--]);
        }
        else
        {
            i++;
        }
    }

    ternaryStringQuickSort(arr, low, lt - 1, depth);
    if (pivot >= 0)
    {
        ternaryStringQuickSort(arr, lt, gt, depth + 1);
    }
    ternaryStringQuickSort(arr, gt + 1, high, depth);
}

// String MergeSort с использованием LCP
void StringSortTester::stringMergeSort(std::vector<std::string> &arr, int left, int right, int depth)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        stringMergeSort(arr, left, mid, depth);
        stringMergeSort(arr, mid + 1, right, depth);
        stringMergeWithLCP(arr, left, mid, right, depth);
    }
}

void StringSortTester::stringMergeWithLCP(std::vector<std::string> &arr, int left, int mid, int right, int depth)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::string> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        // Используем стандартное сравнение строк с подсчетом сравнений
        if (compareStrings(L[i], R[j]) <= 0)
        {
            arr[k] = L[i];
            ++i;
        }
        else
        {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// MSD Radix Sort
void StringSortTester::msdRadixSort(std::vector<std::string> &arr, int low, int high, int depth)
{
    if (high <= low)
        return;

    const int R = 256; // Размер алфавита ASCII
    std::vector<int> count(R + 2, 0);
    std::vector<std::string> aux(high - low + 1);

    // Подсчет частот
    for (int i = low; i <= high; ++i)
    {
        int ch = charAt(arr[i], depth);
        count[ch + 2]++;
        comparison_count++;
    }

    // Вычисление позиций
    for (int r = 0; r < R + 1; ++r)
    {
        count[r + 1] += count[r];
    }

    // Распределение
    for (int i = low; i <= high; ++i)
    {
        int ch = charAt(arr[i], depth);
        aux[count[ch + 1]++] = arr[i];
    }

    // Копирование обратно
    for (int i = low; i <= high; ++i)
    {
        arr[i] = aux[i - low];
    }

    // Рекурсивная сортировка для каждого символа
    for (int r = 0; r < R; ++r)
    {
        int start = low + count[r];
        int end = low + count[r + 1] - 1;
        if (start < end)
        {
            msdRadixSort(arr, start, end, depth + 1);
        }
    }
}

// MSD Radix Sort с переключением на String QuickSort
void StringSortTester::msdRadixSortWithQuickSort(std::vector<std::string> &arr, int low, int high, int depth)
{
    if (high <= low)
        return;

    // Переключаемся на String QuickSort если размер массива меньше размера алфавита
    if (high - low + 1 < ALPHABET_SIZE)
    {
        ternaryStringQuickSort(arr, low, high, depth);
        return;
    }

    const int R = 256;
    std::vector<int> count(R + 2, 0);
    std::vector<std::string> aux(high - low + 1);

    for (int i = low; i <= high; ++i)
    {
        int ch = charAt(arr[i], depth);
        count[ch + 2]++;
        comparison_count++;
    }

    for (int r = 0; r < R + 1; ++r)
    {
        count[r + 1] += count[r];
    }

    for (int i = low; i <= high; ++i)
    {
        int ch = charAt(arr[i], depth);
        aux[count[ch + 1]++] = arr[i];
    }

    for (int i = low; i <= high; ++i)
    {
        arr[i] = aux[i - low];
    }

    for (int r = 0; r < R; ++r)
    {
        int start = low + count[r];
        int end = low + count[r + 1] - 1;
        if (start < end)
        {
            msdRadixSortWithQuickSort(arr, start, end, depth + 1);
        }
    }
}

// Методы тестирования
SortResult StringSortTester::measurePerformance(
    std::vector<std::string> arr,
    std::function<void(std::vector<std::string> &)> sortFunction)
{

    comparison_count = 0;

    auto start = std::chrono::high_resolution_clock::now();
    sortFunction(arr);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double execution_time = duration.count() / 1000.0; // Переводим в миллисекунды

    return {execution_time, comparison_count, isSorted(arr)};
}

SortResult StringSortTester::averageResults(
    std::vector<std::string> arr,
    std::function<void(std::vector<std::string> &)> sortFunction,
    int num_runs)
{

    double total_time = 0.0;
    long long total_comparisons = 0;
    bool all_sorted = true;

    for (int i = 0; i < num_runs; ++i)
    {
        std::vector<std::string> arr_copy = arr;
        SortResult result = measurePerformance(arr_copy, sortFunction);

        total_time += result.execution_time_ms;
        total_comparisons += result.character_comparisons;
        all_sorted = all_sorted && result.is_sorted;
    }

    return {
        total_time / num_runs,
        total_comparisons / num_runs,
        all_sorted};
}

// Методы тестирования конкретных алгоритмов
SortResult StringSortTester::testQuickSort(std::vector<std::string> arr)
{
    return averageResults(arr, [](std::vector<std::string> &a)
                          { quickSort(a, 0, a.size() - 1); }, 3);
}

SortResult StringSortTester::testMergeSort(std::vector<std::string> arr)
{
    return averageResults(arr, [](std::vector<std::string> &a)
                          { mergeSort(a, 0, a.size() - 1); }, 3);
}

SortResult StringSortTester::testStringQuickSort(std::vector<std::string> arr)
{
    return averageResults(arr, [](std::vector<std::string> &a)
                          { ternaryStringQuickSort(a, 0, a.size() - 1, 0); }, 3);
}

SortResult StringSortTester::testStringMergeSort(std::vector<std::string> arr)
{
    return averageResults(arr, [](std::vector<std::string> &a)
                          { stringMergeSort(a, 0, a.size() - 1, 0); }, 3);
}

SortResult StringSortTester::testMSDRadixSort(std::vector<std::string> arr)
{
    return averageResults(arr, [](std::vector<std::string> &a)
                          { msdRadixSort(a, 0, a.size() - 1, 0); }, 3);
}

SortResult StringSortTester::testMSDRadixSortWithQuickSort(std::vector<std::string> arr)
{
    return averageResults(arr, [](std::vector<std::string> &a)
                          { msdRadixSortWithQuickSort(a, 0, a.size() - 1, 0); }, 3);
}

// Утилиты для вывода
void StringSortTester::printResult(const std::string &algorithm_name, const SortResult &result)
{
    std::cout << std::fixed << std::setprecision(3);
    std::cout << algorithm_name << ":\n";
    std::cout << "  Время выполнения: " << result.execution_time_ms << " мс\n";
    std::cout << "  Посимвольных сравнений: " << result.character_comparisons << "\n";
    std::cout << "  Отсортирован: " << (result.is_sorted ? "Да" : "Нет") << "\n\n";
}

void StringSortTester::printCSVHeader()
{
    std::cout << "Algorithm,ArraySize,ArrayType,ExecutionTime_ms,CharacterComparisons,IsSorted\n";
}

void StringSortTester::printCSVResult(const std::string &algorithm_name, int array_size,
                                      const std::string &array_type, const SortResult &result)
{
    std::cout << algorithm_name << "," << array_size << "," << array_type << ","
              << std::fixed << std::setprecision(3) << result.execution_time_ms << ","
              << result.character_comparisons << "," << (result.is_sorted ? "true" : "false") << "\n";
}
