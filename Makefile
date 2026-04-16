CXX      = g++
CXXFLAGS = -std=c++17 -O2
SRCDIR   = source
OUTDIR   = output

SOURCES  = $(SRCDIR)/main.cpp \
           $(SRCDIR)/Imagen.cpp \
           $(SRCDIR)/FuerzaBruta.cpp \
           $(SRCDIR)/Backtracking.cpp \
           $(SRCDIR)/ProgramacionDinamica.cpp

TARGET = seam

# ─── Targets principales ──────────────────────────────────────────────────────

all: setup $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Descarga stb_image.h y stb_image_write.h (requiere curl)
setup:
	@if [ ! -f $(SRCDIR)/stb_image.h ]; then \
		echo "Descargando stb_image.h..."; \
		curl -sL https://raw.githubusercontent.com/nothings/stb/master/stb_image.h \
		     -o $(SRCDIR)/stb_image.h; \
	fi
	@if [ ! -f $(SRCDIR)/stb_image_write.h ]; then \
		echo "Descargando stb_image_write.h..."; \
		curl -sL https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h \
		     -o $(SRCDIR)/stb_image_write.h; \
	fi

# ─── Ejecución ────────────────────────────────────────────────────────────────

run-numerico: $(TARGET)
	./$(TARGET) --numerico input/ejemplo.txt --algoritmo pd

run-imagen: $(TARGET)
	@if [ -z "$(IMG)" ]; then \
		echo "Uso: make run-imagen IMG=img/foto.jpg N=50 ALG=pd"; \
	else \
		./$(TARGET) --imagen $(IMG) --algoritmo $(or $(ALG),pd) --iteraciones $(or $(N),1); \
	fi

# ─── Limpieza ─────────────────────────────────────────────────────────────────

clean:
	rm -f $(TARGET)

clean-output:
	rm -f $(OUTDIR)/numericos/* $(OUTDIR)/imagenes/*

# Windows (MinGW): usar `mingw32-make` en lugar de `make`.
# Si curl no está disponible, descargar manualmente:
#   stb_image.h       -> https://github.com/nothings/stb/blob/master/stb_image.h
#   stb_image_write.h -> https://github.com/nothings/stb/blob/master/stb_image_write.h
# y colocarlos en la carpeta source/.

.PHONY: all setup run-numerico run-imagen clean clean-output
