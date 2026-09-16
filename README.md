# Rescue Squad

Aplicación gráfica nativa de escritorio en C++ con SFML 2.6 para el juego Rescue Squad.

## Requisitos

- Compilador C++17 (MinGW/MSYS2 recomendado en Windows)
- Biblioteca SFML 2.6 instalada en `C:/msys64/mingw64`

## Compilación

Windows (PowerShell):

```powershell
g++ -std=c++17 -Iinclude -I"C:/msys64/mingw64/include" src/*.cpp -o bin/rescue-squad.exe -lsfml-graphics -lsfml-window -lsfml-system -mwindows
```

Linux/macOS:

```bash
make
```

## Ejecución

Ejecuta el archivo generado en `bin/rescue-squad.exe` o el binario generado por Make.

## Controles

- Click izquierdo: seleccionar destino del rescatista
- Click derecho: alternar fuego/pared sobre la casilla
- El fuego se expande cada 3 segundos
- El rescatista recalcula la ruta si el camino queda bloqueado
