#pragma once

#include <string>
#include <vector>

// Clase que representa una imagen y permite cargarla, guardarla,
// calcular su matriz de energía y eliminar seams.
class Imagen {
public:
    Imagen();
    Imagen(const std::string& ruta);
    ~Imagen();

    // Carga una imagen desde disco (JPG, PNG, BMP, etc.)
    void cargar(const std::string& ruta);

    // Guarda la imagen en disco. El formato se deduce de la extensión (.png, .jpg, .bmp)
    void guardar(const std::string& ruta) const;

    int ancho() const;
    int alto() const;

    // Devuelve la matriz de energía: energia[fila][col] = magnitud del gradiente en ese pixel
    std::vector<std::vector<double>> obtenerMatrizEnergia() const;

    // Elimina un seam vertical de la imagen.
    // seam[fila] = índice de columna del pixel a eliminar en esa fila
    void eliminarSeam(const std::vector<int>& seam);

    // Devuelve true si no hay imagen cargada
    bool estaVacia() const;

private:
    int _ancho;
    int _alto;

    // Píxeles almacenados como [fila][col][canal] con canales R, G, B
    std::vector<std::vector<std::vector<unsigned char>>> _pixeles;

    // Calcula la energía de un pixel individual usando diferencias finitas
    double calcularEnergiaPixel(int fila, int col) const;
};
