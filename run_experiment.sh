#!/bin/bash

# Скрипт для автоматизации исследования алгоритмов сортировки строк

set -e  # Остановка при ошибке

echo "=== Автоматизированное исследование алгоритмов сортировки строк ==="
echo

# Функция для проверки наличия команды
check_command() {
    if ! command -v $1 &> /dev/null; then
        echo "Ошибка: $1 не найден. Пожалуйста, установите $1."
        return 1
    fi
}

# Проверка зависимостей
echo "Проверка зависимостей..."
check_command g++
check_command make

# Компиляция проекта
echo "Компиляция проекта..."
make clean
make release

if [ $? -ne 0 ]; then
    echo "Ошибка компиляции. Проверьте код."
    exit 1
fi

echo "Компиляция успешна!"
echo

# Выбор типа эксперимента
echo "Выберите тип эксперимента:"
echo "1) Быстрый тест (размер 500, ~1 минута)"
echo "2) Средний тест (размеры 100-1000, ~5 минут)"
echo "3) Полный тест (размеры 100-3000, ~20 минут)"
echo "4) Пользовательский тест"
echo

read -p "Введите номер (1-4): " choice

case $choice in
    1)
        echo "Запуск быстрого теста..."
        ./string_sort_experiment --quick
        ;;
    2)
        echo "Запуск среднего теста..."
        # Создаем временную версию с меньшим диапазоном
        sed 's/MAX_ARRAY_SIZE = 3000/MAX_ARRAY_SIZE = 1000/' StringSortTester.h > StringSortTester_temp.h
        mv StringSortTester.h StringSortTester_orig.h
        mv StringSortTester_temp.h StringSortTester.h
        make release
        echo "y" | ./string_sort_experiment
        mv StringSortTester_orig.h StringSortTester.h
        ;;
    3)
        echo "Запуск полного теста..."
        echo "Это может занять до 20 минут. Продолжить? (y/n)"
        read -p "> " confirm
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            echo "y" | ./string_sort_experiment
        else
            echo "Тест отменен."
            exit 0
        fi
        ;;
    4)
        echo "Запуск пользовательского теста..."
        ./string_sort_experiment
        ;;
    *)
        echo "Неверный выбор. Запуск быстрого теста..."
        ./string_sort_experiment --quick
        ;;
esac

# Проверка результатов
if [ ! -f "experiment_results.csv" ]; then
    echo "Ошибка: файл результатов не создан."
    exit 1
fi

echo
echo "Эксперимент завершен! Результаты сохранены в experiment_results.csv"
echo "Количество записей: $(wc -l < experiment_results.csv)"

# Анализ результатов
echo
echo "Хотите выполнить анализ результатов? (требует Python и библиотеки)"
read -p "Анализировать результаты? (y/n): " analyze

if [ "$analyze" = "y" ] || [ "$analyze" = "Y" ]; then
    # Проверка Python
    if command -v python3 &> /dev/null; then
        echo "Проверка зависимостей Python..."

        # Попытка импортировать необходимые библиотеки
        python3 -c "import pandas, matplotlib, seaborn, numpy" 2>/dev/null

        if [ $? -eq 0 ]; then
            echo "Запуск анализа..."
            python3 analyze_results.py
        else
            echo "Не найдены необходимые Python библиотеки."
            echo "Установите их командой: pip install pandas matplotlib seaborn numpy"
            echo "Или запустите анализ позже: python3 analyze_results.py"
        fi
    else
        echo "Python3 не найден. Установите Python для анализа результатов."
    fi
fi

echo
echo "=== Сводка ==="
echo "Исходный код: $(pwd)"
echo "Результаты: $(pwd)/experiment_results.csv"
if [ -f "sorting_analysis.png" ]; then
    echo "Графики: $(pwd)/sorting_analysis.png"
fi

echo
echo "Для повторного анализа используйте:"
echo "  python3 analyze_results.py"
echo
echo "Для очистки используйте:"
echo "  make clean"

echo
echo "Исследование завершено!"
