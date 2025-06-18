#!/bin/bash

# Скрипт для создания архива с результатами исследования

PROJECT_NAME="string_sorting_research"
ARCHIVE_NAME="${PROJECT_NAME}_$(date +%Y%m).tar.gz"

echo "=== Создание архива результатов исследования ==="
echo "Имя архива: $ARCHIVE_NAME"
echo

# Создаем временную директорию для архива
TEMP_DIR="/tmp/${PROJECT_NAME}_temp"
mkdir -p "$TEMP_DIR"

echo "Копирование файлов..."

# Копируем исходный код
cp *.h *.cpp *.py Makefile "$TEMP_DIR/"


# Копируем скрипты
cp run_experiment.sh run_medium_test.sh "$TEMP_DIR/"

# Создаем структуру результатов
mkdir -p "$TEMP_DIR/results"
cp experiment_results.csv sorting_analysis.png "$TEMP_DIR/results/"

# Создаем директорию с документацией
mkdir -p "$TEMP_DIR/docs"
cp README.md REPORT.md "$TEMP_DIR/docs/"

# Создаем архив
cd /tmp
tar -czf "$ARCHIVE_NAME" "${PROJECT_NAME}_temp"

# Перемещаем архив в рабочую директорию
CURRENT_DIR=$(pwd)
cd - > /dev/null
mv "/tmp/$ARCHIVE_NAME" "./"

# Очищаем временные файлы
rm -rf "$TEMP_DIR"

echo "Архив создан: $ARCHIVE_NAME"
echo

# Показываем содержимое архива
echo "Содержимое архива:"
tar -tzf "$ARCHIVE_NAME" | head -20

echo
echo "Размер архива: $(du -h "$ARCHIVE_NAME" | cut -f1)"

echo
echo "=== Сводка проекта ==="
echo "Исходные файлы:"
echo "  - StringGenerator.h/cpp - генерация тестовых данных"
echo "  - StringSortTester.h/cpp - тестирование алгоритмов"
echo "  - main.cpp - основная программа"
echo "  - analyze_results.py - анализ результатов"
echo
echo "Результаты:"
echo "  - experiment_results.csv - сырые данные ($(wc -l < experiment_results.csv) записей)"
echo "  - sorting_analysis.png - графики анализа"
echo "  - REPORT.md - итоговый отчет"
echo
echo "Компиляция и запуск:"
echo "  make && ./string_sort_experiment"
echo "  python3 analyze_results.py"
echo
echo "Исследование завершено успешно!"
