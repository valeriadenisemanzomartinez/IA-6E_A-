# Documento de diseño

Este proyecto implementa una cuadrícula 20x20 para Rescue Squad utilizando SFML.

## Sistema de terreno

- Suelo limpio: gris claro, costo 1
- Escombros: marrón, costo 3
- Fuego: rojo, costo 10
- Paredes: gris oscuro, infranqueables

## Pathfinding

Se usa A* con costo acumulado + heurística Manhattan.

## Controles

- Izquierdo: mover al rescatista a una casilla
- Derecho: alternar fuego o pared

## Reglas del juego

- Cada 3 segundos el fuego se expande aleatoriamente
- Si el camino queda bloqueado, se recalcula
- La víctima se recoge al entrar en su casilla y se lleva a la zona de evacuación
