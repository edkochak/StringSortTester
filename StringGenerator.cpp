#include "StringGenerator.h"
#include <iostream>
#include <chrono>
#include <set>

const std::string StringGenerator::ALPHABET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";

StringGenerator::StringGenerator()
    : gen(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
      char_dist(0, ALPHABET.length() - 1),
      length_dist(10, 200)
{
}

std::string StringGenerator::generateRandomString(int min_length, int max_length)
{
    std::uniform_int_distribution<int> len_dist(min_length, max_length);
    int length = len_dist(gen);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; ++i)
    {
        result += ALPHABET[char_dist(gen)];
    }

    return result;
}

std::vector<std::string> StringGenerator::generateRandomArray(int size)
{
    std::vector<std::string> result;
    result.reserve(size);

    for (int i = 0; i < size; ++i)
    {
        result.push_back(generateRandomString());
    }

    return result;
}

std::vector<std::string> StringGenerator::generateReverseSortedArray(int size)
{
    auto arr = generateRandomArray(size);
    std::sort(arr.begin(), arr.end());
    std::reverse(arr.begin(), arr.end());
    return arr;
}

std::vector<std::string> StringGenerator::generateNearlySortedArray(int size, double swap_ratio)
{
    auto arr = generateRandomArray(size);
    std::sort(arr.begin(), arr.end());

    int num_swaps = static_cast<int>(size * swap_ratio);
    shuffleWithSwaps(arr, num_swaps);

    return arr;
}

std::vector<std::string> StringGenerator::generatePrefixBasedArray(int size, int prefix_length)
{
    std::vector<std::string> result;
    result.reserve(size);

    int num_prefixes = std::max(1, size / 10);
    std::vector<std::string> prefixes;

    for (int i = 0; i < num_prefixes; ++i)
    {
        prefixes.push_back(generateRandomString(prefix_length, prefix_length));
    }

    std::uniform_int_distribution<int> prefix_dist(0, num_prefixes - 1);

    for (int i = 0; i < size; ++i)
    {
        std::string base_prefix = prefixes[prefix_dist(gen)];
        std::string suffix = generateRandomString(5, 50);
        result.push_back(base_prefix + suffix);
    }

    return result;
}

void StringGenerator::shuffleWithSwaps(std::vector<std::string> &arr, int num_swaps)
{
    std::uniform_int_distribution<int> index_dist(0, arr.size() - 1);

    for (int i = 0; i < num_swaps; ++i)
    {
        int idx1 = index_dist(gen);
        int idx2 = index_dist(gen);
        if (idx1 != idx2)
        {
            std::swap(arr[idx1], arr[idx2]);
        }
    }
}
