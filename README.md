# Seam Carving - TP1

Implementación de algoritmos para Seam Carving.

## Algoritmos
- Fuerza Bruta (FB)
- Backtracking (BT)
- Programación Dinámica (PD)

## Estructura
- `source/`: implementación principal
- `input/`: matrices e imágenes de entrada
- `output/`: resultados generados
- `tests/`: scripts y casos de prueba

## Ejecución
Modo numérico:

```bash
./seam --numerico input/ejemplo.txt --algoritmo pd
```

Modo imagen:

```bash
./seam --imagen input/imagenes_test/imagen.jpg --algoritmo pd --iteraciones 50
```