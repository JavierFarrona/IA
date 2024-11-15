#include "tools.h"

bool ComprobarParametros(const int argc, char *argv[], const int kCorrectNumber) {
  if (argc != kCorrectNumber) {
    std::cerr << "Error: Número de parámetros incorrecto.\n" << std::endl;
    return false;
  }
  return true;
}

void is_open(std::ifstream &intput, std::ofstream &output) {
  if (!intput.is_open()) {
    std::cerr << "Error: file not found" << std::endl;
    exit(1);
  }
  if (!output.is_open()) {
    std::cerr << "Error: file not found" << std::endl;
    exit(1);
  }
}