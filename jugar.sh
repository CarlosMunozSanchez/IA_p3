#!/bin/bash

echo -e "Partido contra Valoración test\n"

./bin/Parchis --p1 AI 0 Master --p2 AI 1 ValoracionTest --no-gui > p1
./bin/Parchis --p1 AI 1 ValoracionTest --p2 AI 0 Master --no-gui > p2

echo -e "Partido contra Ninja1\n"

./bin/Parchis --p1 AI 0 Master --p2 Ninja 1 ninja1 --no-gui > p3
./bin/Parchis --p1 Ninja 1 ninja1 --p2 AI 0 Master --no-gui > p4

echo -e "Partido contra Ninja2\n"

./bin/Parchis --p1 AI 0 Master --p2 Ninja 2 ninja2 --no-gui > p5
./bin/Parchis --p1 Ninja 2 ninja2 --p2 AI 0 Master --no-gui > p6

echo -e "Partido contra Ninja3\n"

./bin/Parchis --p1 AI 0 Master --p2 Ninja 3 ninja3 --no-gui > p7
./bin/Parchis --p1 Ninja 3 ninja3 --p2 AI 0 Master --no-gui > p8

echo "Simulación completada"
