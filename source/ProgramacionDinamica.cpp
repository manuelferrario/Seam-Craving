#include "ProgramacionDinamica.h"
#include <limits>

std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia) {
    
    // Uso una funcion auxiliar que haga recursion para pasar mas parametros.
    int fila = 0; 
    int columna_optima = -1;
    double mejor_costo = std::numeric_limits<double>::infinity();
    
    std::vector<std::vector<double>> memo(energia.size(), std::vector<double>(energia[0].size(), -1)); //Creo una matriz en la cual voy a guardar la energia optima de cada pixel. 
    std::vector<std::vector<int>> parent(energia.size(), std::vector<int>(energia[0].size(), -1)); //Creo una matriz en la cual voy a guardar la decision optima de cada pixel.
    
    for(int columna = 0; columna < energia[0].size(); columna++) { 
        double costo = aux_PD(energia, memo, parent, fila, columna);
        if(costo < mejor_costo){
            mejor_costo = costo;
            columna_optima = columna;
        }
    }
    int j = columna_optima;
    std::vector<int> camino;
    for(int i = 0; i < energia.size(); i++){ // juntamos todas las decisiones optimas puntuales de cada pixel para obtener el camino
        camino.push_back(j); 
        j = parent[i][j]; //lo que hacemos aca es ver del parent la decision optima para ver que columna es y poder seguir el hilo de las decisiones
    }
    return camino;
}

double aux_PD(const std::vector<std::vector<double>>& energia, std::vector<std::vector<double>>& memo, std::vector<std::vector<int>>& parent, int fila, int columna){
        
        if (memo[fila][columna] != -1) {  // Si ya esta en memoria devolvela
            return memo[fila][columna];
        }

        if( fila == energia.size()-1){ // Si es la ultima asignale su energia y devolvela 
            memo[fila][columna] = energia[fila][columna]; 
            return memo[fila][columna];
        }
        
        int columna_optima = -1;
        double mejor_costo = std::numeric_limits<double>::infinity();

        for(int opciones = -1; opciones <= 1; opciones++ ) { // Calculo todos los casos 
            int columnaactual = columna + opciones;
            if(columnaactual < 0 || columnaactual >= energia[0].size()) { // Chequeo segmentation fault
            // No hago nada.
            }
            else{
                double costo = aux_PD(energia, memo, parent, fila + 1, columnaactual); // Calculo el costo de las tres (si es posible) posibilidades desde la columna
                if(costo < mejor_costo){ // Guardo el mejor costo y el camino
                    mejor_costo = costo;
                    columna_optima = columnaactual;
            }
            }
}
        parent[fila][columna] = columna_optima; // Guardo la columna de la fila que sigue, asi voy guardando la decision optima en cada celda
        memo[fila][columna] = energia[fila][columna] + mejor_costo; // Guardo la energia de la celda mas el costo de lo de abajo asi me ahorro recursiones

        return memo[fila][columna];
}