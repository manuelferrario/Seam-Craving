#pragma once

#include <vector>

std::vector<int> encontrarSeamFuerzaBruta(const std::vector<std::vector<double>>& energia);
void func_aux_FB(const std::vector<std::vector<double>>& energia, int fila, int columnaPrevia, std::vector<int>& caminoActual, double energiaActual, std::vector<int>& mejorCamino, double& mejorEnergia);