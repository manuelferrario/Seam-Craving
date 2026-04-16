#pragma once

#include <vector>

std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia);
double aux_PD(const std::vector<std::vector<double>>& energia, std::vector<std::vector<double>>& memo, std::vector<std::vector<int>>& parent, int fila, int columna);