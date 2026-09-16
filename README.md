# Rescue Squad

Rescue Squad es un juego de estrategia y supervivencia en una cuadrícula 20x20 donde el jugador controla a un rescatista que debe llegar a una víctima atrapada y llevarla a la zona de evacuación.

## ¿De qué trata?

El rescatista debe recorrer un mapa lleno de diferentes tipos de terreno, evitar muros y fuego, y encontrar la ruta más segura para llegar a la víctima y luego llevarla hasta la salida. El fuego se expande cada pocos segundos, por lo que el camino puede volverse peligroso y hay que recalcular rutas en tiempo real.

## Objetivo

- Llegar a la víctima.
- Cargarla.
- Llevarla a la zona de evacuación.
- Conseguir la mayor cantidad de puntos posible mientras se evita el fuego y los obstáculos.

## Requisitos

- Compilador C++17 (MinGW/MSYS2 recomendado en Windows)
- Biblioteca SFML 2.6 instalada en `C:/msys64/mingw64`

## Compilación

Windows (PowerShell):

```powershell
& "C:\msys64\mingw64\bin\g++.exe" -std=c++17 -O2 -Wall -Wextra -I".\include" -I"C:/msys64/mingw64/include" .\src\AStar.cpp .\src\Game.cpp .\src\main.cpp .\src\Map.cpp .\src\Rescuer.cpp -o .\bin\rescue-squad.exe -lsfml-graphics -lsfml-window -lsfml-system -mwindows
```

## Ejecución

```powershell
.\bin\rescue-squad.exe
```

## Controles

- Click izquierdo: seleccionar destino del rescatista
- Click derecho: alternar fuego o pared sobre la casilla
- El fuego se expande cada 3 segundos
- El rescatista recalcula la ruta si el camino queda bloqueado
