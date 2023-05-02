#!/bin/bash

name_file="JuegoElectro"

echo -e "\nThe Name of the file is: $name_file\n"

g++ -c main.cpp

g++ main.o -o $name_file -lsfml-graphics -lsfml-window -lsfml-system

rm *.o


./$name_file