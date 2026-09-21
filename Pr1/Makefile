CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Ilib

BUILD_DIR := build
SOURCES := src/Nodo.cpp src/Mapa.cpp src/AEstrella.cpp
OBJECTS := $(SOURCES:src/%.cpp=$(BUILD_DIR)/%.o)
TARGET := $(BUILD_DIR)/pr1

.PHONY: all clean test ejemplo

all: $(TARGET)

$(TARGET): $(OBJECTS) $(BUILD_DIR)/main.o
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	@set -e; for mapa in test/entorno*.txt; do echo "== $$mapa =="; $(TARGET) "$$mapa" "$$mapa.salida" "$$mapa.resultados" || resultado=$$?; test "$${resultado:-0}" -le 1; unset resultado; done

ejemplo: $(TARGET)
	$(TARGET) test/entorno01.txt entorno01_salida.txt resultados.txt ID1

clean:
	rm -rf $(BUILD_DIR)