#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Imagen.h"
#include "stb_image.h"
#include "stb_image_write.h"

#include <stdexcept>
#include <cmath>
#include <algorithm>

Imagen::Imagen() : _ancho(0), _alto(0) {}

Imagen::Imagen(const std::string& ruta) : _ancho(0), _alto(0) {
    cargar(ruta);
}

Imagen::~Imagen() {}

void Imagen::cargar(const std::string& ruta) {
    int canales;
    unsigned char* datos = stbi_load(ruta.c_str(), &_ancho, &_alto, &canales, 3);
    if (!datos) {
        throw std::runtime_error("No se pudo cargar la imagen: " + ruta);
    }

    _pixeles.assign(_alto, std::vector<std::vector<unsigned char>>(_ancho, std::vector<unsigned char>(3)));
    for (int f = 0; f < _alto; f++) {
        for (int c = 0; c < _ancho; c++) {
            int idx = (f * _ancho + c) * 3;
            _pixeles[f][c][0] = datos[idx];
            _pixeles[f][c][1] = datos[idx + 1];
            _pixeles[f][c][2] = datos[idx + 2];
        }
    }
    stbi_image_free(datos);
}

void Imagen::guardar(const std::string& ruta) const {
    if (estaVacia()) throw std::runtime_error("La imagen está vacía.");

    std::vector<unsigned char> datos(_alto * _ancho * 3);
    for (int f = 0; f < _alto; f++) {
        for (int c = 0; c < _ancho; c++) {
            int idx = (f * _ancho + c) * 3;
            datos[idx]     = _pixeles[f][c][0];
            datos[idx + 1] = _pixeles[f][c][1];
            datos[idx + 2] = _pixeles[f][c][2];
        }
    }

    // Detectar formato por extensión
    std::string ext = ruta.substr(ruta.find_last_of('.') + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    int ok = 0;
    if (ext == "png") {
        ok = stbi_write_png(ruta.c_str(), _ancho, _alto, 3, datos.data(), _ancho * 3);
    } else if (ext == "jpg" || ext == "jpeg") {
        ok = stbi_write_jpg(ruta.c_str(), _ancho, _alto, 3, datos.data(), 90);
    } else if (ext == "bmp") {
        ok = stbi_write_bmp(ruta.c_str(), _ancho, _alto, 3, datos.data());
    } else {
        throw std::runtime_error("Formato no soportado: " + ext + ". Usar .png, .jpg o .bmp");
    }

    if (!ok) throw std::runtime_error("No se pudo guardar la imagen en: " + ruta);
}

int Imagen::ancho() const { return _ancho; }
int Imagen::alto() const  { return _alto;  }

bool Imagen::estaVacia() const { return _ancho == 0 || _alto == 0; }

double Imagen::calcularEnergiaPixel(int fila, int col) const {
    auto pixel = [&](int f, int c) -> std::vector<unsigned char> {
        f = std::max(0, std::min(_alto - 1, f));
        c = std::max(0, std::min(_ancho - 1, c));
        return _pixeles[f][c];
    };

    double energia = 0.0;
    for (int canal = 0; canal < 3; canal++) {
        double dx = (double)pixel(fila, col + 1)[canal] - (double)pixel(fila, col - 1)[canal];
        double dy = (double)pixel(fila + 1, col)[canal] - (double)pixel(fila - 1, col)[canal];
        energia += dx * dx + dy * dy;
    }
    return std::sqrt(energia);
}

std::vector<std::vector<double>> Imagen::obtenerMatrizEnergia() const {
    std::vector<std::vector<double>> energia(_alto, std::vector<double>(_ancho));
    for (int f = 0; f < _alto; f++) {
        for (int c = 0; c < _ancho; c++) {
            energia[f][c] = calcularEnergiaPixel(f, c);
        }
    }
    return energia;
}

void Imagen::eliminarSeam(const std::vector<int>& seam) {
    if ((int)seam.size() != _alto) {
        throw std::runtime_error("El seam debe tener exactamente una entrada por fila.");
    }

    for (int f = 0; f < _alto; f++) {
        int col = seam[f];
        _pixeles[f].erase(_pixeles[f].begin() + col);
    }
    _ancho--;
}
