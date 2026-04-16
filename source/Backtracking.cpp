#include "Backtracking.h"
#include <limits>


void func_aux_BT(const std::vector<std::vector<double>>& energia, int fila, int columnaPrevia, std::vector<int>& caminoActual, double energiaActual, std::vector<int>& mejorCamino, double& mejorEnergia) {
    
    // Caso base (llegamos al final de la matriz)
    if(fila == static_cast<int>(energia.size())) {
        if(energiaActual < mejorEnergia) {
            mejorEnergia = energiaActual;
            mejorCamino = caminoActual;
            // No returneo nada y la energia se modifica en su valor de referencia iteracion por iteracion.
        }
        return;
    }

    // Tres para moverse:
    // 1) Recto abajo (Sumo 0)
    // 2) Abajo derecha (Chequear que no estemos en borde) (Sumo 1)
    // 3) Abajo izquierda (Chequear que no estemos en borde) (Sumo -1)


    for(int opciones = -1; opciones <= 1; opciones++ ) { 
        int columna = columnaPrevia + opciones;
            if(columna < 0 || columna >= static_cast<int>(energia[fila].size())) { // Chequeo segmentation fault
                // No hago nada.
            }
            else { 
                // Poda por optimalidad: Si tengo mas energia actual que la mejor energia no sigo calculando.
                if( mejorEnergia <= energiaActual + energia[fila][columna] ) { 
                    continue;
                } else {
                    caminoActual.push_back(columna);
                    func_aux_BT(energia, fila+1, columna, caminoActual, energiaActual + energia[fila][columna], mejorCamino, mejorEnergia);
                    caminoActual.pop_back(); // Saco el ultimo valor para volver a probar otro camino.
                }
                    
            }
        }

}

std::vector<int> encontrarSeamBacktracking(const std::vector<std::vector<double>>& energia) {
    if (energia.empty() || energia[0].empty()) return {};
    
    // Uso una funcion auxiliar que haga recursion para pasar mas parametros.
    int fila = 1; 
    std::vector<int> caminoActual;
    std::vector<int> mejorCamino;
    double mejorEnergia = std::numeric_limits<double>::infinity();
    
    for(int columna = 0; columna < static_cast<int>(energia[0].size()); columna++) { 
        caminoActual.clear();
        caminoActual.push_back(columna);
        double energiaActual = energia[0][columna];
        func_aux_BT(energia, fila, columna, caminoActual, energiaActual, mejorCamino, mejorEnergia);
    }
    return mejorCamino;
}
