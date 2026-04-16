#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "Imagen.h"
#include "FuerzaBruta.h"
#include "Backtracking.h"
#include "ProgramacionDinamica.h"

// Lee una matriz de energía desde un archivo de texto.
// Formato esperado:
//   filas columnas
//   e00 e01 e02 ...
//   e10 e11 e12 ...
//   ...
std::vector<std::vector<double>> leerMatrizEnergia(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    int filas, columnas;
    archivo >> filas >> columnas;

    std::vector<std::vector<double>> energia(filas, std::vector<double>(columnas));
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            archivo >> energia[f][c];

    return energia;
}

// Ejecuta el algoritmo seleccionado y devuelve el seam encontrado
std::vector<int> ejecutarAlgoritmo(const std::vector<std::vector<double>>& energia, const std::string& algoritmo) {
    if (algoritmo == "fb")  return encontrarSeamFuerzaBruta(energia);
    if (algoritmo == "bt")  return encontrarSeamBacktracking(energia);
    if (algoritmo == "pd")  return encontrarSeamPD(energia);
    throw std::runtime_error("Algoritmo desconocido: " + algoritmo + ". Usar fb, bt o pd.");
}

void imprimirMatriz(const std::vector<std::vector<double>>& matriz) {
    for (const auto& fila : matriz) {
        for (double val : fila)
            std::cout << val << "\t";
        std::cout << "\n";
    }
}

void imprimirSeam(const std::vector<int>& seam, const std::vector<std::vector<double>>& energia) {
    std::cout << "Seam encontrado: ";
    double total = 0.0;
    for (int f = 0; f < (int)seam.size(); f++) {
        std::cout << "(" << f << "," << seam[f] << ") ";
        total += energia[f][seam[f]];
    }
    std::cout << "\nEnergía total: " << total << "\n";
}

void modoNumerico(const std::string& rutaEntrada, const std::string& algoritmo) {
    std::vector<std::vector<double>> energia = leerMatrizEnergia(rutaEntrada);

    std::cout << "Matriz de energía:\n";
    imprimirMatriz(energia);
    std::cout << "\n";

    std::vector<int> seam = ejecutarAlgoritmo(energia, algoritmo);
    imprimirSeam(seam, energia);

    std::string rutaSalida = "output/numericos/seam_" + algoritmo + ".txt";
    std::ofstream salida(rutaSalida);
    if (salida.is_open()) {
        for (int f = 0; f < (int)seam.size(); f++)
            salida << "fila " << f << " -> columna " << seam[f] << "\n";
        std::cout << "Resultado guardado en " << rutaSalida << "\n";
    }
}

void modoImagen(const std::string& rutaImagen, const std::string& algoritmo, int iteraciones) {
    Imagen img(rutaImagen);
    std::cout << "Imagen cargada: " << img.ancho() << "x" << img.alto() << " px\n";

    for (int i = 0; i < iteraciones; i++) {
        std::vector<int> seam = ejecutarAlgoritmo(img.obtenerMatrizEnergia(), algoritmo);
        img.eliminarSeam(seam);

        if ((i + 1) % 10 == 0 || i == iteraciones - 1)
            std::cout << "Iteración " << (i + 1) << "/" << iteraciones
                      << " - Ancho actual: " << img.ancho() << " px\n";
    }

    std::string rutaSalida = "output/imagenes/resultado_" + algoritmo + ".png";
    img.guardar(rutaSalida);
    std::cout << "Imagen guardada en " << rutaSalida << "\n";
}

void imprimirUso() {
    std::cout << "Uso:\n"
              << "  Modo numérico: ./seam --numerico <archivo> --algoritmo <fb|bt|pd>\n"
              << "  Modo imagen:   ./seam --imagen <archivo> --algoritmo <fb|bt|pd> --iteraciones <N>\n"
              << "\nEjemplos:\n"
              << "  ./seam --numerico input/ejemplo.txt --algoritmo pd\n"
              << "  ./seam --imagen img/foto.jpg --algoritmo pd --iteraciones 50\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        imprimirUso();
        return 1;
    }

    std::string modo, rutaArchivo, algoritmo = "pd";
    int iteraciones = 1;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--numerico" && i + 1 < argc) {
            modo = "numerico";
            rutaArchivo = argv[++i];
        } else if (arg == "--imagen" && i + 1 < argc) {
            modo = "imagen";
            rutaArchivo = argv[++i];
        } else if (arg == "--algoritmo" && i + 1 < argc) {
            algoritmo = argv[++i];
        } else if (arg == "--iteraciones" && i + 1 < argc) {
            iteraciones = std::stoi(argv[++i]);
        } else if (arg == "--ayuda" || arg == "--help") {
            imprimirUso();
            return 0;
        }
    }

    try {
        if (modo == "numerico") {
            modoNumerico(rutaArchivo, algoritmo);
        } else if (modo == "imagen") {
            modoImagen(rutaArchivo, algoritmo, iteraciones);
        } else {
            imprimirUso();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
