#ifndef CODE_H
#define CODE_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "maze.h"

bool ComprobarParametros(const int argc, char* argv[], const int kNumeroCorrecto);

void is_open(std::ifstream &intput, std::ofstream &output);

#endif // CODE_H