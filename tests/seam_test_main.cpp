#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../source/FuerzaBruta.h"
#include "../source/Backtracking.h"

std::vector<std::vector<double>> leerMatrizEnergia(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    int filas, columnas;
    archivo >> filas >> columnas;

    std::vector<std::vector<double>> energia(filas, std::vector<double>(columnas));
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            archivo >> energia[f][c];
        }
    }

    return energia;
}

std::vector<int> ejecutarAlgoritmo(const std::vector<std::vector<double>>& energia, const std::string& algoritmo) {
    if (algoritmo == "fb") return encontrarSeamFuerzaBruta(energia);
    if (algoritmo == "bt") return encontrarSeamBacktracking(energia);
    throw std::runtime_error("Algoritmo desconocido: " + algoritmo + ". Usar fb o bt.");
}

double energiaTotal(const std::vector<std::vector<double>>& energia, const std::vector<int>& seam) {
    double total = 0.0;
    for (int f = 0; f < static_cast<int>(seam.size()); f++) {
        total += energia[f][seam[f]];
    }
    return total;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 5) {
            std::cerr << "Uso: seam_test --numerico <archivo> --algoritmo <fb|bt>\n";
            return 1;
        }

        std::string modo = argv[1];
        std::string ruta = argv[2];
        std::string flagAlg = argv[3];
        std::string algoritmo = argv[4];

        if (modo != "--numerico" || flagAlg != "--algoritmo") {
            std::cerr << "Uso: seam_test --numerico <archivo> --algoritmo <fb|bt>\n";
            return 1;
        }

        auto energia = leerMatrizEnergia(ruta);
        auto seam = ejecutarAlgoritmo(energia, algoritmo);
        double total = energiaTotal(energia, seam);

        std::cout << "seam=";
        for (int i = 0; i < static_cast<int>(seam.size()); i++) {
            if (i) std::cout << ' ';
            std::cout << seam[i];
        }
        std::cout << "\n";
        std::cout << std::setprecision(17) << "energy=" << total << "\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
