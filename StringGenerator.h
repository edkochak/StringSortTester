#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <string>
#include <vector>
#include <random>
#include <algorithm>

class StringGenerator
{
private:
    std::mt19937 gen;
    std::uniform_int_distribution<int> char_dist;
    std::uniform_int_distribution<int> length_dist;

    static const std::string ALPHABET;

public:
    StringGenerator();

    // Генерация одной случайной строки
    std::string generateRandomString(int min_length = 10, int max_length = 200);

    // Генерация массива случайных строк
    std::vector<std::string> generateRandomArray(int size);

    // Генерация обратно отсортированного массива
    std::vector<std::string> generateReverseSortedArray(int size);

    // Генерация почти отсортированного массива
    std::vector<std::string> generateNearlySortedArray(int size, double swap_ratio = 0.05);

    // Генерация массива строк с общими префиксами
    std::vector<std::string> generatePrefixBasedArray(int size, int prefix_length = 5);

    // Перемешивание массива с заданным количеством перестановок
    void shuffleWithSwaps(std::vector<std::string> &arr, int num_swaps);
};

#endif // STRING_GENERATOR_H
