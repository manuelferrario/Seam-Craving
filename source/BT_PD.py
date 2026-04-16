import math

# Bactracking

def _func_aux_bt(energia, fila, columna_previa, camino_actual, energia_actual, mejor):
    
    """
    mejor es una lista de un elemento [mejor_energia, mejor_camino]
    para poder modificarla dentro de la recursion (simula pasar por referencia).
    """
    
    # Caso base: llegamos al final de la matriz
    if fila == len(energia):
        if energia_actual < mejor[0]:
            mejor[0] = energia_actual
            mejor[1] = camino_actual[:]  # copia del camino actual
        return

    # Tres opciones: izquierda (-1), recto (0), derecha (+1)
    for i in [-1, 0, 1]:
        columna = columna_previa + i

        # Chequeo de bordes
        if columna < 0 or columna >= len(energia[fila]):
            continue

        # Poda por optimalidad
        if mejor[0] <= energia_actual + energia[fila][columna]:
            continue

        camino_actual.append(columna)
        _func_aux_bt(energia, fila + 1, columna, camino_actual, energia_actual + energia[fila][columna], mejor)
        camino_actual.pop()  # Backtrack

def encontrar_seam_backtracking(energia):
    if not energia or not energia[0]:
        return []

    mejor = [math.inf, []]  # [mejor_energia, mejor_camino]

    for i in range(len(energia[0])):
        camino_actual = [i]
        energia_actual = energia[0][i]
        _func_aux_bt(energia, 1, i, camino_actual, energia_actual, mejor)

    return mejor[1]


# Programacion Dinamica

def _aux_pd(energia, memo, parent, fila, columna):
    # Si ya está en memoria, devolver
    if memo[fila][columna] != -1:
        return memo[fila][columna]

    # Caso base: ultima fila
    if fila == len(energia) - 1:
        memo[fila][columna] = energia[fila][columna]
        return memo[fila][columna]

    mejor_costo = math.inf
    columna_optima = -1

    for i in [-1, 0, 1]:
        columna_siguiente = columna + i

        # Chequeo de bordes
        if columna_siguiente < 0 or columna_siguiente >= len(energia[0]):
            continue

        costo = _aux_pd(energia, memo, parent, fila + 1, columna_siguiente)
        if costo < mejor_costo:
            mejor_costo = costo
            columna_optima = columna_siguiente

    parent[fila][columna] = columna_optima
    memo[fila][columna] = energia[fila][columna] + mejor_costo
    return memo[fila][columna]

def encontrar_programacion_dinamica(energia):
    n = len(energia)
    m = len(energia[0])

    memo   = [[-1.0] * m for _ in range(n)]
    parent = [[-1]   * m for _ in range(n)]

    mejor_costo = math.inf
    columna_optima = -1

    for i in range(m):
        costo = _aux_pd(energia, memo, parent, 0, i)
        if costo < mejor_costo:
            mejor_costo = costo
            columna_optima = i

    # Reconstruir el camino siguiendo la tabla parent
    camino = []
    j = columna_optima
    for i in range(n):
        camino.append(j)
        j = parent[i][j]

    return camino

# ─────────────────────────────────────────────
#  Ayuda para los tests
# ─────────────────────────────────────────────
def es_camino_valido(camino, energia):
    """Verifica que el camino tenga n pasos y movimientos legales (|ci+1 - ci| <= 1)."""
    n = len(energia)
    m = len(energia[0])
    if len(camino) != n:
        return False
    for col in camino:
        if col < 0 or col >= m:
            return False
    for i in range(len(camino) - 1):
        if abs(camino[i+1] - camino[i]) > 1:
            return False
    return True

def energia_del_camino(camino, energia):
    return sum(energia[i][camino[i]] for i in range(len(energia)))

def ambos_algoritmos(energia):
    """Corre BT y PD y devuelve (camino_bt, camino_pd)."""
    return encontrar_seam_backtracking(energia), encontrar_programacion_dinamica(energia)


# ─────────────────────────────────────────────
#  Tests
# ─────────────────────────────────────────────

def test_ejemplo_del_enunciado():
    """Caso del PDF: 5x6, costura optima con energia 4.3."""
    energia = [
        [9.0, 3.0, 1.0, 2.0, 8.0, 7.0],
        [5.0, 2.0, 0.5, 4.0, 6.0, 3.0],
        [7.0, 1.0, 2.0, 0.8, 5.0, 4.0],
        [3.0, 4.0, 1.5, 1.0, 2.0, 6.0],
        [8.0, 2.0, 3.0, 1.5, 1.0, 5.0],
    ]
    bt, pd = ambos_algoritmos(energia)

    # Ambos deben encontrar energia 4.3
    assert abs(energia_del_camino(bt, energia) - 4.3) < 1e-9, \
        f"BT: energia esperada 4.3, obtenida {energia_del_camino(bt, energia)}"
    assert abs(energia_del_camino(pd, energia) - 4.3) < 1e-9, \
        f"PD: energia esperada 4.3, obtenida {energia_del_camino(pd, energia)}"

    # Ambos deben ser caminos validos
    assert es_camino_valido(bt, energia), f"BT: camino invalido {bt}"
    assert es_camino_valido(pd, energia), f"PD: camino invalido {pd}"

    print("PASS  test_ejemplo_del_enunciado")

def test_camino_trivial_1x1():
    """Matriz de un solo pixel: el unico camino posible."""
    energia = [[5.0]]
    bt, pd = ambos_algoritmos(energia)
    assert bt == [0], f"BT: {bt}"
    assert pd == [0], f"PD: {pd}"
    print("PASS  test_camino_trivial_1x1")

def test_una_sola_fila():
    """Una fila: el camino es simplemente el pixel de menor energia."""
    energia = [[3.0, 1.0, 4.0, 1.5]]
    bt, pd = ambos_algoritmos(energia)
    assert bt == [1], f"BT: {bt}"
    assert pd == [1], f"PD: {pd}"
    print("PASS  test_una_sola_fila")

def test_una_sola_columna():
    """Una columna: no hay opciones de movimiento, el camino siempre es la columna 0."""
    energia = [[2.0], [3.0], [1.0], [4.0]]
    bt, pd = ambos_algoritmos(energia)
    assert bt == [0, 0, 0, 0], f"BT: {bt}"
    assert pd == [0, 0, 0, 0], f"PD: {pd}"
    print("PASS  test_una_sola_columna")

def test_camino_recto_obvio():
    """Columna del medio tiene energia 0 en todas las filas: el camino optimo es recto."""
    energia = [
        [9.0, 0.0, 9.0],
        [9.0, 0.0, 9.0],
        [9.0, 0.0, 9.0],
    ]
    bt, pd = ambos_algoritmos(energia)
    assert bt == [1, 1, 1], f"BT: {bt}"
    assert pd == [1, 1, 1], f"PD: {pd}"
    print("PASS  test_camino_recto_obvio")

def test_camino_diagonal():
    """
    El camino optimo obliga a moverse en diagonal.
    La energia minima baja por la diagonal principal.
    """
    energia = [
        [1.0, 9.0, 9.0],
        [9.0, 1.0, 9.0],
        [9.0, 9.0, 1.0],
    ]
    bt, pd = ambos_algoritmos(energia)
    assert energia_del_camino(bt, energia) == 3.0, \
        f"BT: energia {energia_del_camino(bt, energia)}"
    assert energia_del_camino(pd, energia) == 3.0, \
        f"PD: energia {energia_del_camino(pd, energia)}"
    assert es_camino_valido(bt, energia)
    assert es_camino_valido(pd, energia)
    print("PASS  test_camino_diagonal")

def test_todos_valores_iguales():
    """Todos los pixeles tienen la misma energia: cualquier camino valido es optimo."""
    energia = [[1.0] * 4 for _ in range(4)]
    bt, pd = ambos_algoritmos(energia)
    assert es_camino_valido(bt, energia), f"BT invalido: {bt}"
    assert es_camino_valido(pd, energia), f"PD invalido: {pd}"
    assert energia_del_camino(bt, energia) == 4.0
    assert energia_del_camino(pd, energia) == 4.0
    print("PASS  test_todos_valores_iguales")

def test_bt_y_pd_coinciden_en_energia():
    """
    BT y PD pueden devolver caminos distintos si hay empate,
    pero la energia optima SIEMPRE debe ser la misma.
    """
    energia = [
        [1.0, 2.0, 3.0, 4.0],
        [4.0, 1.0, 2.0, 3.0],
        [3.0, 4.0, 1.0, 2.0],
        [2.0, 3.0, 4.0, 1.0],
    ]
    bt, pd = ambos_algoritmos(energia)
    e_bt = energia_del_camino(bt, energia)
    e_pd = energia_del_camino(pd, energia)
    assert abs(e_bt - e_pd) < 1e-9, \
        f"Energias distintas: BT={e_bt}, PD={e_pd}"
    assert es_camino_valido(bt, energia)
    assert es_camino_valido(pd, energia)
    print("PASS  test_bt_y_pd_coinciden_en_energia")

def test_borde_izquierdo():
    """El camino optimo esta pegado al borde izquierdo: prueba que no haya out-of-bounds."""
    energia = [
        [0.0, 9.0, 9.0, 9.0],
        [0.0, 9.0, 9.0, 9.0],
        [0.0, 9.0, 9.0, 9.0],
    ]
    bt, pd = ambos_algoritmos(energia)
    assert energia_del_camino(bt, energia) == 0.0
    assert energia_del_camino(pd, energia) == 0.0
    assert es_camino_valido(bt, energia)
    assert es_camino_valido(pd, energia)
    print("PASS  test_borde_izquierdo")

def test_borde_derecho():
    """El camino optimo esta pegado al borde derecho."""
    energia = [
        [9.0, 9.0, 9.0, 0.0],
        [9.0, 9.0, 9.0, 0.0],
        [9.0, 9.0, 9.0, 0.0],
    ]
    bt, pd = ambos_algoritmos(energia)
    assert energia_del_camino(bt, energia) == 0.0
    assert energia_del_camino(pd, energia) == 0.0
    assert es_camino_valido(bt, energia)
    assert es_camino_valido(pd, energia)
    print("PASS  test_borde_derecho")

def test_energia_cero_en_todo_el_camino():
    """Camino con energia total 0 posible: ambos deben encontrarlo."""
    energia = [
        [5.0, 0.0, 5.0],
        [5.0, 0.0, 5.0],
        [5.0, 0.0, 5.0],
        [5.0, 0.0, 5.0],
    ]
    bt, pd = ambos_algoritmos(energia)
    assert energia_del_camino(bt, energia) == 0.0
    assert energia_del_camino(pd, energia) == 0.0
    print("PASS  test_energia_cero_en_todo_el_camino")

def test_matriz_mediana_no_explota():
    """
    Matriz 10x10: no verifica optimalidad exhaustiva,
    solo que ambos devuelvan un camino valido sin errores.
    """
    import random
    random.seed(42)
    n, m = 10, 10
    energia = [[random.uniform(0, 10) for _ in range(m)] for _ in range(n)]
    bt, pd = ambos_algoritmos(energia)
    assert es_camino_valido(bt, energia), f"BT invalido"
    assert es_camino_valido(pd, energia), f"PD invalido"
    # Ambos deben encontrar la misma energia optima
    assert abs(energia_del_camino(bt, energia) - energia_del_camino(pd, energia)) < 1e-9
    print("PASS  test_matriz_mediana_no_explota")

def test_matriz_grande_bt_vs_pd():
    """
    Matriz 22x30: demuestra que BT es exponencialmente más lento que PD.
    BT encontrará la solución pero tardará significativamente más tiempo.
    PD debe ser prácticamente instantáneo.
    """
    import random
    import time
    
    random.seed(42)
    n, m = 22, 30
    energia = [[random.uniform(1, 10) for _ in range(m)] for _ in range(n)]
    
    # Correr PD primero (rápido)
    print(f"\n  Matriz {n}x{m}:")
    start_pd = time.time()
    pd = encontrar_programacion_dinamica(energia)
    time_pd = time.time() - start_pd
    
    print(f"    PD: {time_pd:.6f}s, camino válido: {es_camino_valido(pd, energia)}, "
          f"energía: {energia_del_camino(pd, energia):.2f}")
    
    # Correr BT (lento)
    start_bt = time.time()
    bt = encontrar_seam_backtracking(energia)
    time_bt = time.time() - start_bt
    
    print(f"    BT: {time_bt:.6f}s, camino válido: {es_camino_valido(bt, energia)}, "
          f"energía: {energia_del_camino(bt, energia):.2f}")
    print(f"    Ratio BT/PD: {time_bt/time_pd:.1f}x más lento")
    
    # Verificaciones
    assert es_camino_valido(bt, energia), "BT: camino inválido"
    assert es_camino_valido(pd, energia), "PD: camino inválido"
    
    # Ambos deben encontrar el mismo coste óptimo
    energia_bt = energia_del_camino(bt, energia)
    energia_pd = energia_del_camino(pd, energia)
    assert abs(energia_bt - energia_pd) < 1e-9, \
        f"BT y PD encontraron diferentes óptimos: BT={energia_bt}, PD={energia_pd}"
    
    print("PASS  test_matriz_grande_bt_vs_pd")

if __name__ == "__main__":
    tests = [
        test_ejemplo_del_enunciado,
        test_camino_trivial_1x1,
        test_una_sola_fila,
        test_una_sola_columna,
        test_camino_recto_obvio,
        test_camino_diagonal,
        test_todos_valores_iguales,
        test_bt_y_pd_coinciden_en_energia,
        test_borde_izquierdo,
        test_borde_derecho,
        test_energia_cero_en_todo_el_camino,
        test_matriz_mediana_no_explota,
        test_matriz_grande_bt_vs_pd,
    ]

    passed, failed = 0, 0
    for t in tests:
        try:
            t()
            passed += 1
        except AssertionError as e:
            print(f"FAIL  {t.__name__}: {e}")
            failed += 1

    print(f"\n{'='*40}")
    print(f"Resultado: {passed}/{passed+failed} tests pasaron")
    if failed == 0:
        print("Todo OK!")
