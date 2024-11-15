# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Computabilidad y Algoritmia
#
# @author Lucas Hernández Dévora alu0101551371@ull.edu.es
# @date 07/10/2024
# @brief A Makefile
#

CXX = g++
CXXFLAGS = -std=c++17 -Wall

SOURCES = main.cc code.cc node.cc maze.cc data_table.cc
TARGET = maze

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: all clean
