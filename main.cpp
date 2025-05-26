#include "StringGenerator.h"
#include "StringSortTester.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

class ExperimentRunner
{
private:
    StringGenerator generator;
    std::ofstream csv_file;

    void runExperimentForSize(int size)
    {
        std::cout << "Тестирование для размера массива: " << size << std::endl;

        auto random_data = generator.generateRandomArray(size);
        auto reverse_sorted_data = generator.generateReverseSortedArray(size);
        auto nearly_sorted_data = generator.generateNearlySortedArray(size);
        auto prefix_based_data = generator.generatePrefixBasedArray(size);

        std::map<std::string, std::vector<std::string>> test_data = {
            {"Random", random_data},
            {"ReverseSorted", reverse_sorted_data},
            {"NearlySorted", nearly_sorted_data},
            {"PrefixBased", prefix_based_data}};

        std::vector<std::pair<std::string, std::function<SortResult(std::vector<std::string>)>>> algorithms = {
            {"QuickSort", StringSortTester::testQuickSort},
            {"MergeSort", StringSortTester::testMergeSort},
            {"StringQuickSort", StringSortTester::testStringQuickSort},
            {"StringMergeSort", StringSortTester::testStringMergeSort},
            {"MSDRadixSort", StringSortTester::testMSDRadixSort},
            {"MSDRadixSortWithQuickSort", StringSortTester::testMSDRadixSortWithQuickSort}};

        for (const auto &data_pair : test_data)
        {
            const std::string &data_type = data_pair.first;
            const std::vector<std::string> &data = data_pair.second;

            std::cout << "  Тип данных: " << data_type << std::endl;

            for (const auto &algo_pair : algorithms)
            {
                const std::string &algo_name = algo_pair.first;
                const auto &algo_function = algo_pair.second;

                try
                {

                    SortResult result = algo_function(data);

                    std::cout << "    " << algo_name << ": "
                              << result.execution_time_ms << " мс, "
                              << result.character_comparisons << " сравнений" << std::endl;

                    csv_file << algo_name << "," << size << "," << data_type << ","
                             << std::fixed << std::setprecision(3) << result.execution_time_ms << ","
                             << result.character_comparisons << ","
                             << (result.is_sorted ? "true" : "false") << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "    Ошибка при тестировании " << algo_name << ": " << e.what() << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }

public:
    ExperimentRunner() : csv_file("experiment_results.csv")
    {

        csv_file << "Algorithm,ArraySize,ArrayType,ExecutionTime_ms,CharacterComparisons,IsSorted" << std::endl;
    }

    ~ExperimentRunner()
    {
        if (csv_file.is_open())
        {
            csv_file.close();
        }
    }

    void runFullExperiment()
    {
        std::cout << "Начинаем полное исследование алгоритмов сортировки строк..." << std::endl;
        std::cout << "Размеры массивов: от " << StringSortTester::MIN_ARRAY_SIZE
                  << " до " << StringSortTester::MAX_ARRAY_SIZE
                  << " с шагом " << StringSortTester::STEP_SIZE << std::endl;
        std::cout << "Результаты будут сохранены в файл experiment_results.csv" << std::endl
                  << std::endl;

        for (int size = StringSortTester::MIN_ARRAY_SIZE;
             size <= StringSortTester::MAX_ARRAY_SIZE;
             size += StringSortTester::STEP_SIZE)
        {
            runExperimentForSize(size);
        }

        std::cout << "Эксперимент завершен! Результаты сохранены в experiment_results.csv" << std::endl;
    }

    void runQuickTest()
    {
        std::cout << "Запуск быстрого теста с размером массива 500..." << std::endl;
        runExperimentForSize(500);
    }
};

int main(int argc, char *argv[])
{
    std::cout << "=== Исследование алгоритмов сортировки строк ===" << std::endl
              << std::endl;

    ExperimentRunner runner;

    if (argc > 1 && std::string(argv[1]) == "--quick")
    {
        runner.runQuickTest();
    }
    else
    {
        std::cout << "Запустить полный эксперимент? (y/n) [или используйте --quick для быстрого теста]: ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            runner.runFullExperiment();
        }
        else
        {
            runner.runQuickTest();
        }
    }

    return 0;
}
