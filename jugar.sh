#!/bin/bash

echo -e "Partido contra Valoración test\n"

echo -e "Jugando como rojo/amarillo\n"
./bin/Parchis --p1 AI 0 Master --p2 AI 1 ValoracionTest --no-gui > p1.txt
echo -e "Jugando como verde/azul\n"
./bin/Parchis --p1 AI 1 ValoracionTest --p2 AI 0 Master --no-gui > p2.txt

echo -e "Partido contra Ninja1\n"

echo -e "Jugando como rojo/amarillo\n"
./bin/Parchis --p1 AI 0 Master --p2 Ninja 1 ninja1 --no-gui > p3.txt
echo -e "Jugando como verde/azul\n"
./bin/Parchis --p1 Ninja 1 ninja1 --p2 AI 0 Master --no-gui > p4.txt

echo -e "Partido contra Ninja2\n"

echo -e "Jugando como rojo/amarillo\n"
./bin/Parchis --p1 AI 0 Master --p2 Ninja 2 ninja2 --no-gui > p5.txt
echo -e "Jugando como verde/azul\n"
./bin/Parchis --p1 Ninja 2 ninja2 --p2 AI 0 Master --no-gui > p6.txt

echo -e "Partido contra Ninja3\n"

echo -e "Jugando como rojo/amarillo\n"
./bin/Parchis --p1 AI 0 Master --p2 Ninja 3 ninja3 --no-gui > p7.txt
echo -e "Jugando como verde/azul\n"
./bin/Parchis --p1 Ninja 3 ninja3 --p2 AI 0 Master --no-gui > p8.txt

echo "Simulación completada"
