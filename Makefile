CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET = string_sort_experiment
SOURCES = main.cpp StringGenerator.cpp StringSortTester.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean run quick_test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) experiment_results.csv

run: $(TARGET)
	./$(TARGET)

quick_test: $(TARGET)
	./$(TARGET) --quick

results: experiment_results.csv
	@echo "Результаты эксперимента:"
	@head -20 experiment_results.csv

install_deps:
	@echo "Для анализа данных рекомендуется установить Python с библиотеками:"
	@echo "pip install pandas matplotlib seaborn numpy"

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)
