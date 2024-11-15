#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "grafo.h"

bool ComprobarParametros(const int argc, char* argv[], const int kNumeroCorrecto);

void is_open(std::ifstream &intput, std::ofstream &output);

#endif // TOOLS_H