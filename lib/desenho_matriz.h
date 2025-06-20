#ifndef DESENHO_MATRIZ_H
#define DESENHO_MATRIZ_H

#include "matrixws.h"

//Exemplo de desenho de matriz
void number1()
{
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {0, BRILHO, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, BRILHO, 10}, {0, BRILHO, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, BRILHO, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, BRILHO, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, BRILHO, 10}, {0, BRILHO, 10}, {0, BRILHO, 10}, {0, 0, 0}}
    };
    desenhaMatriz(mat1);
}

//função "desliga();" é utilizada para apagar a matriz

#endif