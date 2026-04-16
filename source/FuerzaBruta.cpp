#include "FuerzaBruta.h"
#include <limits>

// Pensamiento inicial del problema:
// Cada puntero tiene una decision ir para abajo recto, a la derecha o a la izquierda (siempre para abajo no en linea recta)
// El vector devuelve el camino¿?
// Tiene que haber un valor que indique el tamaño de filas para ver donde termina el tramo
// El vector respuesta contiene: un vector con todas las columnas donde pasa el minimo
// Para fuerza bruta tenemos que ver todas las respuestas posibles y compararlas.
// No tengo en cuenta podas, ya que, eso lo tengo en cuenta en backtracking.

// Como represento el array 

void func_aux_FB(const std::vector<std::vector<double>>& energia, int fila, int columnaPrevia, std::vector<int>& caminoActual, double energiaActual, std::vector<int>& mejorCamino, double& mejorEnergia) {
    
    int n = energia.size();
    int m = energia[0].size();
    
    // Caso base (llegamos al final de la matriz)
    if(fila == n) {
        if(energiaActual < mejorEnergia) {
            mejorEnergia = energiaActual;
            mejorCamino = caminoActual;
            // La energia se modifica en su valor de referencia iteracion por iteracion.
        }
        return;
    }

    // Tres para moverse:
    // 1) Recto abajo (Sumo 0)
    // 2) Abajo derecha (Chequear que no estemos en borde) (Sumo 1)
    // 3) Abajo izquierda (Chequear que no estemos en borde) (Sumo -1)


    for( int opciones = -1; opciones <= 1; opciones++ ) { 
        int columna = columnaPrevia + opciones;
        if(columna < 0 || columna >= energia[fila].size()) { // Chequeo segmentation fault
            // No hago nada.
        }
        else { 
            caminoActual.push_back(columna);
            func_aux_FB(energia, fila+1, columna, caminoActual, energiaActual + energia[fila][columna], mejorCamino, mejorEnergia);
            caminoActual.pop_back(); // Saco el ultimo valor para volver a probar otro camino.
        }
    }

    

}

std::vector<int> encontrarSeamFuerzaBruta(const std::vector<std::vector<double>>& energia) {
    
    // Uso una funcion auxiliar que haga recursion para pasar mas parametros.
    
    int fila = 1; // Empezamos en la fila 1 porque la fila 0 ya la tenemos que recorrer en el for de abajo, y no queremos que se repita esa iteracion. 
    
    int columnaPrevia = 0;
    std::vector<int> caminoActual;
    double energiaActual = 0;  // Parametros que me permiten hacer una implementacion prolija.
    std::vector<int> mejorCamino;
    double mejorEnergia = std::numeric_limits<double>::infinity();
    
    for(int columna = 0; columna < energia[0].size(); columna++) { 
        caminoActual.clear();
        caminoActual.push_back(columna);
        double energiaActual = energia[0][columna];
        func_aux_FB(energia, fila, columna, caminoActual, energiaActual, mejorCamino, mejorEnergia);
    }
    return mejorCamino;
}
